// =============================================================================
//  simulation.cpp
//  Simulation: automatic gearbox + engine torque, drag, slope, speed-sensitive
//  steering, autodrive (slows for bends), boost, off-road reset, coin pickup.
// =============================================================================
#include "simulation.h"
#include "game_state.h"
#include "road.h"
#include "noise.h"
#include "environment.h"
#include "engine.h"

static const float GEAR_RATIO[5] = { 3.4f, 2.2f, 1.6f, 1.25f, 1.0f };
static const float IDLE_RPM = 900.0f, REDLINE = 6800.0f, UPSHIFT_RPM = 5600.0f, DOWNSHIFT_RPM = 2000.0f;

static float rpmFor(float v, int g) { return IDLE_RPM + v * GEAR_RATIO[g] * 80.0f; }
// engine torque curve: peaks around 3800 rpm
static float torqueCurve(float r) { float x = (r - 3800.0f) / 3000.0f; return clampf(1.0f - 0.45f * x * x, 0.25f, 1.0f); }
// drag coefficient chosen so the car tops out near its vmax in top gear
static float dragCoef(const CarSpec& cs) {
    float tq = torqueCurve(rpmFor(cs.vmax, 4));
    float need = cs.acc * tq * (GEAR_RATIO[4] / 1.6f) - 0.9f;
    return fmaxf(0.0002f, need / (cs.vmax * cs.vmax));
}

void resetRun() {
    carS = 0; carD = 0; carV = 0; carVd = 0; steerVis = 0; dist = 0; coins = 0; boostCh = 0; coinGot.clear(); camInit = false;
    gear = 0; shiftT = 0; rpm = IDLE_RPM; bodyPitch = 0;
}

void updateSim(float dt) {
    const CarSpec& cs = cars[carType];
    bool wet = rainCur > 0.5f;
    float kappa = roadK(carS);
    float K = cs.grip * (wet ? 1.4f : 1.0f);
    float gripMul = wet ? 0.85f : 1.0f;
    float sens = clampf(1.0f / (1.0f + carV / 30.0f), 0.45f, 1.0f);   // speed-sensitive steering
    float thr = 0, brk = 0, str = 0;
    bool spaceKey = keys[' '];
    if (autodrive) {
        float target = fminf(cs.vmax * 0.7f, 34.0f) * (wet ? 0.8f : 1.0f) * (weather == 1 ? 0.7f : 1.0f);
        // slow down before bends: keep lateral acceleration under ~6.5 m/s^2
        float kA = fmaxf(fabsf(roadK(carS + 40.0f)), fabsf(roadK(carS + 90.0f)));
        if (kA > 1e-4f) target = fminf(target, sqrtf(6.5f / kA));
        thr = carV < target ? 1.0f : 0.0f; brk = carV > target + 4.0f ? 1.0f : 0.0f;
        float vdNeed = -carD * 0.9f + K * kappa * carV * carV;
        str = clampf(vdNeed / (cs.steer * gripMul * clampf(carV / 10.0f, 0.2f, 1.0f) * sens), -1, 1);
    } else {
        thr = (keys['w'] || sp[GLUT_KEY_UP]) ? 1.0f : 0.0f; brk = (keys['s'] || sp[GLUT_KEY_DOWN]) ? 1.0f : 0.0f;
        str = ((keys['d'] || sp[GLUT_KEY_RIGHT]) ? 1.0f : 0.0f) - ((keys['a'] || sp[GLUT_KEY_LEFT]) ? 1.0f : 0.0f);
    }
    boosting = (spaceKey && boostCh > 0.01f && carV > 5.0f);
    if (boosting) boostCh = fmaxf(0, boostCh - 0.22f * dt);

    // automatic gearbox (short torque dip while shifting)
    if (shiftT > 0) shiftT -= dt;
    if (shiftT <= 0) {
        float r = rpmFor(carV, gear);
        if (r > UPSHIFT_RPM && gear < 4) { gear++; shiftT = 0.25f; }
        else if (r < DOWNSHIFT_RPM && gear > 0) { gear--; shiftT = 0.20f; }
    }
    rpm = rpmFor(carV, gear);

    float shiftCut = shiftT > 0 ? 0.0f : 1.0f;
    float limiter = rpm >= REDLINE ? 0.0f : 1.0f;
    float drive = cs.acc * thr * torqueCurve(rpm) * (GEAR_RATIO[gear] / 1.6f) * shiftCut * limiter;
    if (boosting) drive *= 1.7f;
    float kd = dragCoef(cs) * (boosting ? 0.6f : 1.0f);
    float a = drive - 0.9f - kd * carV * carV;
    if (thr < 0.5f && brk < 0.5f) a -= 0.6f * clampf((rpm - IDLE_RPM) / (REDLINE - IDLE_RPM), 0, 1);  // engine braking
    a -= 9.8f * roadDY(carS);                    // slope: uphill slows, downhill speeds up
    a -= 32.0f * brk;
    if (carV <= 0.0f && a < 0.0f) a = 0.0f;
    carV = fmaxf(0, carV + a * dt);
    braking = (brk > 0.1f && carV > 0.5f);

    // weight transfer: nose dips under braking, lifts under acceleration
    float targetPitch = clampf(a * 0.6f, -1.6f, 1.6f);
    bodyPitch += (targetPitch - bodyPitch) * (1 - expf(-5.0f * dt));

    steerVis += (str - steerVis) * (1 - expf(-9.0f * dt));
    float vd = steerVis * cs.steer * gripMul * clampf(carV / 10.0f, 0, 1) * sens;
    float push = -kappa * carV * carV * K;
    carVd += ((vd + push) - carVd) * (1 - expf(-10.0f * dt));
    carD += carVd * dt;
    float dx = roadDX(carS), dy = roadDY(carS);
    carS += carV * dt / sqrtf(1 + dx * dx + dy * dy);
    dist += carV * dt;
    wheelRot = fmodf(wheelRot + carV * dt / 0.4f * 57.29578f, 360.0f);
    if (fabsf(carD) > HALFW - 0.3f) carV = fmaxf(0, carV - 14.0f * dt);   // grass drag
    if (fabsf(carD) > HALFW + 2.2f) { carD = 0; carV = 0; carVd = 0; steerVis = 0; showMsg("OFF ROAD!  Back on the track"); }
    engineSetState(rpm, thr, true, boosting);
    if (dist > bestDist) bestDist = dist;
    // coins
    int k0 = (int)floorf((carS - 20.0f - 4.0f) / 14.0f);
    for (int k = k0; k <= k0 + 1; k++) {
        if (k < 0 || coinGot.count(k)) continue; int seg = k / 8;
        if (seg == 0 || hash01(seg, 77) < 0.5f) continue;
        float lane = (hash01(seg, 91) * 2 - 1) * 4.2f, s = k * 14.0f + 20.0f;
        if (fabsf(s - carS) < 2.4f && fabsf(carD - lane) < 1.9f) { coinGot.insert(k); coins++; boostCh = fminf(1.0f, boostCh + 0.08f); }
    }
}

void update(float dt) {
    frameDt = dt;
    if (state != ST_PAUSE) {
        simTime += dt; if (dayRun) tod = fmodf(tod + dt / DAYLEN, 1.0f);
    }
    updateEnv(state == ST_PAUSE ? 0.0f : dt);
    static bool audioUp = false;
    if (!audioUp) { engineInit(); audioUp = true; }
    if (state == ST_PLAY) updateSim(dt);
    else engineSetState(rpm, 0.0f, false, false);      // menu / pause: silent
    if (state == ST_MENU) menuSpin += dt * 0.35f;
    if (msgTimer > 0) msgTimer -= dt;
}
