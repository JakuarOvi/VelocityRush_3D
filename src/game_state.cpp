// =============================================================================
//  game_state.cpp
//  Definitions of the shared game variables (see game_state.h).
// =============================================================================
#include "game_state.h"

CarSpec cars[3] = {
    { "Aurora GT  (Sedan)",  0.15f, 0.35f, 0.85f, 50.0f, 14.0f,  9.0f, 0.35f, "Balanced all-rounder" },
    { "Falcon RS  (Sports)", 0.85f, 0.12f, 0.10f, 66.0f, 20.0f, 11.0f, 0.27f, "Fastest, sharp steering" },
    { "Bison X  (SUV)",      0.15f, 0.60f, 0.25f, 42.0f, 11.0f,  8.0f, 0.30f, "Heavy and stable" }
};
State state = ST_MENU;
int winW = 1280, winH = 720;
int carType = 0, camMode = 0, weather = 0;
bool autodrive = false, headForce = false, dayRun = true, boosting = false, braking = false;
float tod = 0.32f;
float carS = 0, carD = 0, carV = 0, carVd = 0, steerVis = 0, wheelRot = 0, boostCh = 0, dist = 0;
int gear = 0; float rpm = 900.0f, shiftT = 0, bodyPitch = 0;
int coins = 0; float bestDist = 0;
std::set<int> coinGot;
float msgTimer = 0; char msg[96] = "";
float simTime = 0, frameDt = 0.016f, menuSpin = 0;
float freeYaw = 0.6f, freePitch = 0.35f, freeDist = 11.0f;
bool dragging = false; int lastMX = 0, lastMY = 0;
bool keys[256]; bool sp[256];
V3 camPos = { 0, 5, 10 }; bool camInit = false;
V3 eyePos = { 0, 0, 0 };
V3 carPos, carFwd; float carYawDeg = 0, carPitchDeg = 0;
GLUquadric* quad = NULL;
GLuint texDetail = 0, listTrunk, listPine, listRound;

Env env;
float cloudCur = 0.05f, fogCur = 0.0026f, rainCur = 0;
const char* wName[3] = { "Sunny", "Foggy", "Rainy" };

V3 rainP[NRAIN];
V3 starP[NSTAR]; float starB[NSTAR];

void showMsg(const char* m) { strncpy(msg, m, 95); msg[95] = 0; msgTimer = 2.2f; }
