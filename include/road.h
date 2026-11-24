// =============================================================================
//  road.h
//  The endless road curve and terrain height function.
// =============================================================================
#ifndef ROAD_H
#define ROAD_H
#include "mathutil.h"

static const float HALFW = 6.5f;     // half road width (2 lanes)
float roadX(float s);
float roadY(float s);
float roadDX(float s);
float roadDY(float s);
float roadK(float s);   // d2x/ds2
V3 roadP(float s);
V3 roadR(float s);
float terrH(float s, float lat, int side);

#endif
