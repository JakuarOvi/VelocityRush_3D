// =============================================================================
//  mathutil.h
//  Small math helpers: V3 vector type, mix/clamp/smoothstep.
// =============================================================================
#ifndef MATHUTIL_H
#define MATHUTIL_H
#include "common.h"

struct V3 { float x, y, z; };
static inline V3 v3(float x, float y, float z) { V3 r = { x, y, z }; return r; }
static inline V3 operator+(V3 a, V3 b) { return v3(a.x + b.x, a.y + b.y, a.z + b.z); }
static inline V3 operator-(V3 a, V3 b) { return v3(a.x - b.x, a.y - b.y, a.z - b.z); }
static inline V3 operator*(V3 a, float s) { return v3(a.x * s, a.y * s, a.z * s); }
static inline float dot(V3 a, V3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
static inline V3 cross(V3 a, V3 b) { return v3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }
static inline V3 norm(V3 a) { float l = sqrtf(dot(a, a)); if (l < 1e-6f) return v3(0, 1, 0); return a * (1.0f / l); }
static inline float mixf(float a, float b, float t) { return a + (b - a) * t; }
static inline float clampf(float x, float a, float b) { return x < a ? a : (x > b ? b : x); }
static inline float sstep(float a, float b, float x) { float t = clampf((x - a) / (b - a), 0, 1); return t * t * (3 - 2 * t); }
static inline float wrapf(float v, float range) { v = fmodf(v + range * 0.5f, range); if (v < 0) v += range; return v - range * 0.5f; }
static inline void mix3(const float* a, const float* b, float t, float* o) { for (int i = 0; i < 3; i++) o[i] = mixf(a[i], b[i], t); }

#endif
