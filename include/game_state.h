// =============================================================================
//  game_state.h
//  Shared game data: car specs, game state enum and all global variables.
// =============================================================================
#ifndef GAME_STATE_H
#define GAME_STATE_H
#include "mathutil.h"

struct CarSpec { const char* name; float r, g, b; float vmax, acc, steer, grip; const char* desc; };
extern CarSpec cars[3];
enum State { ST_MENU, ST_PLAY, ST_PAUSE };
extern State state;
extern int winW, winH;
extern int carType, camMode, weather;
extern bool autodrive, headForce, dayRun, boosting, braking;
extern float tod;                       // time of day 0..1 (0.5 = noon)
static const float DAYLEN = 200.0f;             // seconds for a full day
extern float carS, carD, carV, carVd, steerVis, wheelRot, boostCh, dist;
extern int gear; extern float rpm, shiftT, bodyPitch;   // gearbox / engine / weight transfer
extern int coins; extern float bestDist;
extern std::set<int> coinGot;
extern float msgTimer; extern char msg[96];
extern float simTime, frameDt, menuSpin;
extern float freeYaw, freePitch, freeDist;
extern bool dragging; extern int lastMX, lastMY;
extern bool keys[256]; extern bool sp[256];
extern V3 camPos; extern bool camInit;
extern V3 eyePos;
extern V3 carPos, carFwd; extern float carYawDeg, carPitchDeg;
extern GLUquadric* quad;
extern GLuint texDetail, listTrunk, listPine, listRound;

// environment (computed each frame)
struct Env { V3 sunDir; float elev, dayF, duskF, nightF; float zen[3], hor[3]; };
extern Env env;
extern float cloudCur, fogCur, rainCur;
static const float wCloud[3] = { 0.05f, 0.75f, 0.88f };
static const float wFog[3] = { 0.0026f, 0.011f, 0.0045f };
static const float wRain[3] = { 0.0f, 0.0f, 1.0f };
extern const char* wName[3];

static const int NRAIN = 1800; extern V3 rainP[NRAIN];
static const int NSTAR = 500; extern V3 starP[NSTAR]; extern float starB[NSTAR];

void showMsg(const char* m);

#endif
