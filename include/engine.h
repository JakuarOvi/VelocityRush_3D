// =============================================================================
//  engine.h
//  Procedural engine sound: a 4-cylinder tone whose pitch follows RPM and whose
//  loudness follows throttle. Output: Windows waveOut (other platforms: silent).
// =============================================================================
#ifndef ENGINE_H
#define ENGINE_H
#include <cstdint>

struct EngineSynth { double phase; float lp; float nlp; float vol; uint32_t rng; };

// Pure DSP: fills n mono 16-bit samples at 22050 Hz (no OS calls, testable).
void engineRender(EngineSynth& s, int16_t* out, int n, float rpm, float thr, bool boost, float volTarget);

// Game-side API
void engineSetState(float rpm, float throttle, bool active, bool boosting);
void engineInit();
void engineShutdown();

#endif
