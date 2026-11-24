// =============================================================================
//  noise.cpp
//  Hash / value-noise / fbm (procedural randomness used everywhere).
// =============================================================================
#include "noise.h"
#include "mathutil.h"

unsigned hashU(unsigned x) { x ^= x >> 16; x *= 0x7feb352dU; x ^= x >> 15; x *= 0x846ca68bU; x ^= x >> 16; return x; }
float hash01(int a, int b) {
    unsigned h = hashU(((unsigned)a * 73856093u) ^ hashU((unsigned)b + 0x9e3779b9u));
    return (h & 0xFFFFFF) / 16777216.0f;
}
float vnoise(float x, float y) {
    int xi = (int)floorf(x), yi = (int)floorf(y);
    float fx = x - xi, fy = y - yi; fx = fx * fx * (3 - 2 * fx); fy = fy * fy * (3 - 2 * fy);
    float a = hash01(xi, yi), b = hash01(xi + 1, yi), c = hash01(xi, yi + 1), d = hash01(xi + 1, yi + 1);
    return mixf(mixf(a, b, fx), mixf(c, d, fx), fy);
}
float hashP(int a, int b, int P) { a = ((a % P) + P) % P; b = ((b % P) + P) % P; return hash01(a, b); }
float vnoiseP(float x, float y, int P) {   // tileable
    int xi = (int)floorf(x), yi = (int)floorf(y);
    float fx = x - xi, fy = y - yi; fx = fx * fx * (3 - 2 * fx); fy = fy * fy * (3 - 2 * fy);
    float a = hashP(xi, yi, P), b = hashP(xi + 1, yi, P), c = hashP(xi, yi + 1, P), d = hashP(xi + 1, yi + 1, P);
    return mixf(mixf(a, b, fx), mixf(c, d, fx), fy);
}
float fbm(float x, float y) {
    float v = 0, a = 0.5f;
    for (int i = 0; i < 4; i++) { v += a * vnoise(x, y); x = x * 2.03f + 17.0f; y = y * 2.03f + 31.0f; a *= 0.5f; }
    return v;
}
