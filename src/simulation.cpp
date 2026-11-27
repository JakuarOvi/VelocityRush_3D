// =============================================================================
//  simulation.cpp
//  Vehicle simulation: gearbox, acceleration, braking, grip, drift and stability.
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
static float torqueCurve(float r) { float x = (r - 3800.0f) / 3000.0f; return clampf(1.0f - 0.45f * x * x, 0.25f, 1.0f); }
static float dragCoef(const CarSpec& cs) {
    float need = cs.acc * torqueCurve(rpmFor(cs.vmax, 4)) * (GEAR_RATIO[4] / 1.6f) - physics.rollingResistance;
    return fmaxf(0.0002f, need / (cs.vmax * cs.vmax));
}

void resetRun() {
    carS = 0; carD = 0; carV = 0; carVd = 0; steerVis = 0; dist = 0; coins = 0; boostCh = 0; coinGot.clear(); camInit = false;
    gear = 0; shiftT = 0; rpm = IDLE_RPM; bodyPitch = 0;
}

void updateSim(float dt) {
    dt = clampf(dt, 0.0f, 0.05f);
    if (dt <= 0.0f) return;
    const CarSpec& cs = cars[carType];
    const bool wet = rainCur > 0.5f;
    const float kappa = roadK(carS);
    const float gripScale = physics.dryGripScale * (wet ? physics.wetGripScale : 1.0f);
    const float grip = cs.grip * gripScale;
    const float speedSteer = clampf(1.0f / (1.0f + carV / 30.0f), 0.45f, 1.0f);
    float thr = 0.0f, brk = 0.0f, str = 0.0f;

    if (autodrive) {
        float target = fminf(cs.vmax * 0.7f, 34.0f) * (wet ? 0.8f : 1.0f) * (weather == 1 ? 0.7f : 1.0f);
        float curve = fmaxf(fabsf(roadK(carS + 40.0f)), fabsf(roadK(carS + 90.0f)));
        if (curve > 1e-4f) target = fminf(target, sqrtf(physics.maxLateralAccel * 0.76f / curve));
        thr = carV < target ? 1.0f : 0.0f;
        brk = carV > target + 4.0f ? 1.0f : 0.0f;
        float need = -carD * 0.9f + grip * kappa * carV * carV;
        str = clampf(need / fmaxf(0.1f, cs.steer * clampf(carV / 10.0f, physics.lowSpeedSteerFloor, 1.0f) * speedSteer), -1.0f, 1.0f);
    } else {
        thr = (keys['w'] || sp[GLUT_KEY_UP]) ? 1.0f : 0.0f;
        brk = (keys['s'] || sp[GLUT_KEY_DOWN]) ? 1.0f : 0.0f;
        str = ((keys['d'] || sp[GLUT_KEY_RIGHT]) ? 1.0f : 0.0f) - ((keys['a'] || sp[GLUT_KEY_LEFT]) ? 1.0f : 0.0f);
        str = clampf(str + controllerSteer, -1.0f, 1.0f);
    }
    if (shiftT > 0.0f) shiftT -= dt;
    if (shiftT <= 0.0f) {
        float r = rpmFor(carV, gear);
        if (r > UPSHIFT_RPM && gear < 4) { gear++; shiftT = 0.25f; }
        else if (r < DOWNSHIFT_RPM && gear > 0) { gear--; shiftT = 0.20f; }
    }
    rpm = rpmFor(carV, gear);
    boosting = keys[' '] && boostCh > 0.01f && carV > 5.0f;
    if (boosting) boostCh = fmaxf(0.0f, boostCh - 0.22f * dt);

    const float shiftCut = shiftT > 0.0f ? 0.0f : 1.0f;
    const float limiter = rpm >= REDLINE ? 0.0f : 1.0f;
    float drive = cs.acc * thr * torqueCurve(rpm) * (GEAR_RATIO[gear] / 1.6f) * shiftCut * limiter;
    if (boosting) drive *= 1.7f;
    const float drag = dragCoef(cs) * physics.aeroDragScale * (boosting ? 0.6f : 1.0f);
    float accel = drive - physics.rollingResistance - drag * carV * carV;
    if (thr < 0.05f && brk < 0.05f) accel -= 0.6f * clampf((rpm - IDLE_RPM) / (REDLINE - IDLE_RPM), 0.0f, 1.0f);
    accel -= 9.8f * roadDY(carS);
    accel -= physics.brakeDecel * brk;
    if (carV <= 0.0f && accel < 0.0f) accel = 0.0f;
    carV = fmaxf(0.0f, carV + accel * dt);
    braking = brk > 0.1f && carV > 0.5f;

    // Damped body response models pitch under braking/acceleration and roll in turns.
    float pitchTarget = clampf(accel * 0.055f, -1.6f, 1.6f);
    bodyPitch += (pitchTarget - bodyPitch) * (1.0f - expf(-physics.suspensionStiffness * dt));
    float rollTarget = clampf(-str * carV / fmaxf(1.0f, cs.vmax) * 7.0f, -4.0f, 4.0f);
    steerVis += (str - steerVis) * (1.0f - expf(-physics.steeringResponse * dt));

    float speedFactor = clampf(carV / 10.0f, physics.lowSpeedSteerFloor, 1.0f);
    float steerAccel = steerVis * physics.maxLateralAccel * speedFactor * speedSteer * gripScale;
    float curveAccel = -kappa * carV * carV * grip;
    float slip = fabsf(atan2f(carVd, fmaxf(carV, 2.0f)));
    float driftGrip = slip > physics.driftThreshold ? physics.driftGripScale : 1.0f;
    float lateralTarget = (steerAccel + curveAccel) * driftGrip;
    carVd += (lateralTarget - carVd) * (1.0f - expf(-physics.lateralResponse * gripScale * dt));
    carD += carVd * dt;

    float dx = roadDX(carS), dy = roadDY(carS);
    carS += carV * dt / sqrtf(1.0f + dx * dx + dy * dy);
    dist += carV * dt;
    wheelRot = fmodf(wheelRot + carV * dt / 0.4f * 57.29578f, 360.0f);
    if (fabsf(carD) > HALFW - 0.3f) carV = fmaxf(0.0f, carV - physics.offroadDecel * dt);
    if (fabsf(carD) > HALFW + 2.2f) {
        carD = clampf(carD, -HALFW - 1.0f, HALFW + 1.0f);
        carV = fmaxf(0.0f, carV - physics.collisionDecel * dt);
        carVd *= 0.65f;
        showMsg("OFF ROAD!  Back on the track");
    }
    engineSetState(rpm, thr, true, boosting);
    if (dist > bestDist) bestDist = dist;

    int k0 = (int)floorf((carS - 24.0f) / 14.0f);
    for (int k = k0; k <= k0 + 1; k++) {
        if (k < 0 || coinGot.count(k)) continue;
        int seg = k / 8;
        if (seg == 0 || hash01(seg, 77) < 0.5f) continue;
        float lane = (hash01(seg, 91) * 2.0f - 1.0f) * 4.2f, s = k * 14.0f + 20.0f;
        if (fabsf(s - carS) < 2.4f && fabsf(carD - lane) < 1.9f) { coinGot.insert(k); coins++; boostCh = fminf(1.0f, boostCh + 0.08f); }
    }
}

void update(float dt) {
    frameDt = dt;
    if (state != ST_PAUSE) { simTime += dt; if (dayRun) tod = fmodf(tod + dt / DAYLEN, 1.0f); }
    updateEnv(state == ST_PAUSE ? 0.0f : dt);
    static bool audioUp = false;
    if (!audioUp) { engineInit(); audioUp = true; }
    if (state == ST_PLAY) updateSim(dt); else engineSetState(rpm, 0.0f, false, false);
    if (state == ST_MENU) menuSpin += dt * 0.35f;
    if (msgTimer > 0.0f) msgTimer -= dt;
}
