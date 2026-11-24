// =============================================================================
//  road.cpp
//  The endless road: a parametric curve P(s) = (x(s), y(s), -s), plus terrain height.
//  v2: added an "envelope" wiggle that creates sharper bends in stretches of road.
// =============================================================================
#include "road.h"
#include "noise.h"

static float roadEnv(float s) { return 0.5f + 0.5f * sinf(s * 0.0011f + 2.0f); }

float roadX(float s) {
    return 120.0f * sinf(s * 0.0030f) + 40.0f * sinf(s * 0.0085f + 1.3f)
         + 22.0f * roadEnv(s) * sinf(s * 0.021f + 0.4f);
}
float roadY(float s) { return 7.0f * sinf(s * 0.0026f + 0.5f) + 3.0f * sinf(s * 0.0069f); }
float roadDX(float s) { return roadX(s + 0.5f) - roadX(s - 0.5f); }
float roadDY(float s) { return roadY(s + 0.5f) - roadY(s - 0.5f); }
float roadK(float s) { return (roadDX(s + 2.0f) - roadDX(s - 2.0f)) / 4.0f; }   // d2x/ds2
V3 roadP(float s) { return v3(roadX(s), roadY(s), -s); }
V3 roadR(float s) { return norm(v3(1, 0, roadDX(s))); }

float terrH(float s, float lat, int side) {
    float t = clampf(lat / 230.0f, 0, 1);
    float roll = sstep(3.0f, 30.0f, lat) * (fbm(s * 0.012f + side * 40.0f, lat * 0.025f) - 0.35f) * 14.0f;
    float mtn = t * t * 120.0f * (0.35f + fbm(s * 0.0045f + side * 17.0f, lat * 0.008f) * 1.1f);
    return roadY(s) - 0.1f - sstep(0, 6, lat) * 0.4f + roll + mtn;
}
