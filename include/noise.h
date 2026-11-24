// =============================================================================
//  noise.h
//  Hash / value-noise / fbm declarations.
// =============================================================================
#ifndef NOISE_H
#define NOISE_H

unsigned hashU(unsigned x);
float hash01(int a, int b = 0);
float vnoise(float x, float y);
float hashP(int a, int b, int P);
float vnoiseP(float x, float y, int P);   // tileable
float fbm(float x, float y);

#endif
