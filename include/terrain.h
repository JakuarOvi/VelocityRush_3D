// =============================================================================
//  terrain.h
//  Terrain grid constants and terrain rendering (shared row table with road/trees).
// =============================================================================
#ifndef TERRAIN_H
#define TERRAIN_H
#include "mathutil.h"

static const float STEP = 8.0f;
static const int ROWS_BEHIND = 8, ROWS_AHEAD = 90, NR = ROWS_BEHIND + ROWS_AHEAD + 1;
static const int NC = 11;
static const float latCol[NC] = { 0, 2.5f, 6, 12, 22, 38, 60, 90, 130, 180, 240 };
extern float rowS[NR];
void drawTerrain();

#endif
