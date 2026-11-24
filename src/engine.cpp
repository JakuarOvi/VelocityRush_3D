// =============================================================================
//  engine.cpp
//  Engine synth. Firing frequency = rpm / 30 (4 cylinders, 2 pulses per rev).
//  Tone = band-limited sawtooth-like harmonics, low-passed more as RPM rises.
//  Intake noise is added with throttle. Streamed through waveOut (Windows).
// =============================================================================
#include "engine.h"
#include <cmath>
#include <cstdlib>

namespace {
const int kRate = 22050;
const int kBuf = 1024;

volatile float gRpm = 900.0f, gThr = 0.0f, gVolTarget = 0.0f;
volatile bool gBoost = false;
EngineSynth gSynth = { 0.0, 0.0f, 0.0f, 0.0f, 12345u };
}

void engineRender(EngineSynth& s, int16_t* out, int n, float rpm, float thr, bool boost, float volTarget) {
    const double kTwoPi = 6.283185307179586;
    float f = rpm / 30.0f;                                     // Hz
    float cutoff = fminf(0.14f + rpm / 7000.0f, 0.85f);   // keep harmonics so low idle is audible on small speakers
    float revLoad = fminf(rpm / 7000.0f, 1.0f);
    for (int i = 0; i < n; i++) {
        s.vol += (volTarget - s.vol) * 0.0008f;               // smooth fade in/out
        s.phase += f / kRate;
        if (s.phase >= 1.0) s.phase -= 1.0;

        double tone = 0;
        for (int k = 1; k <= 8; k++) tone += sin(kTwoPi * k * s.phase) / k;   // sawtooth-like
        s.lp += cutoff * ((float)tone * 0.35f - s.lp);

        s.rng = s.rng * 1664525u + 1013904223u;                 // LCG white noise
        float white = ((s.rng >> 8) / 16777216.0f) * 2.0f - 1.0f;
        s.nlp += 0.08f * (white - s.nlp);

        float load = 0.25f + 0.75f * thr + (boost ? 0.35f : 0.0f);
        float amp = (0.22f + 0.78f * revLoad) * load * s.vol;
        float sample = (s.lp * 0.7f + s.nlp * 0.22f * (0.35f + thr)) * amp;
        if (sample > 1.0f) sample = 1.0f;
        if (sample < -1.0f) sample = -1.0f;
        out[i] = (int16_t)(sample * 0.85f * 32767.0f);
    }
}

#ifdef _WIN32
#include <windows.h>
#include <mmsystem.h>

namespace {
HWAVEOUT gWave = nullptr;
WAVEHDR gHdr[3];
int16_t gBufs[3][kBuf];
bool gReady = false;

void renderInto(WAVEHDR* h) {
    engineRender(gSynth, (int16_t*)h->lpData, kBuf, gRpm, gThr, gBoost, gVolTarget);
}

void CALLBACK waveProc(HWAVEOUT hw, UINT msg, DWORD_PTR, DWORD_PTR p1, DWORD_PTR) {
    if (msg == WOM_DONE) {
        WAVEHDR* h = (WAVEHDR*)p1;
        renderInto(h);
        waveOutWrite(hw, h, sizeof(WAVEHDR));
    }
}

void shutdownAudio() {
    if (!gWave) return;
    waveOutReset(gWave);
    for (int i = 0; i < 3; i++) waveOutUnprepareHeader(gWave, &gHdr[i], sizeof(WAVEHDR));
    waveOutClose(gWave);
    gWave = nullptr;
}
}

void engineInit() {
    if (gReady) return;
    gReady = true;
    WAVEFORMATEX fmt = {};
    fmt.wFormatTag = WAVE_FORMAT_PCM;
    fmt.nChannels = 1;
    fmt.nSamplesPerSec = kRate;
    fmt.wBitsPerSample = 16;
    fmt.nBlockAlign = fmt.nChannels * fmt.wBitsPerSample / 8;
    fmt.nAvgBytesPerSec = fmt.nSamplesPerSec * fmt.nBlockAlign;
    if (waveOutOpen(&gWave, WAVE_MAPPER, &fmt, (DWORD_PTR)waveProc, 0, CALLBACK_FUNCTION) != MMSYSERR_NOERROR) {
        gWave = nullptr;                       // no audio device: the game runs silently
        return;
    }
    for (int i = 0; i < 3; i++) {
        gHdr[i] = {};
        gHdr[i].lpData = (LPSTR)gBufs[i];
        gHdr[i].dwBufferLength = kBuf * sizeof(int16_t);
        waveOutPrepareHeader(gWave, &gHdr[i], sizeof(WAVEHDR));
        renderInto(&gHdr[i]);
        waveOutWrite(gWave, &gHdr[i], sizeof(WAVEHDR));
    }
    atexit(shutdownAudio);
}

void engineShutdown() { shutdownAudio(); }

#else   // non-Windows: no audio backend yet

void engineInit() {}
void engineShutdown() {}

#endif

void engineSetState(float rpm, float throttle, bool active, bool boosting) {
    gRpm = rpm;
    gThr = throttle;
    gBoost = boosting;
    gVolTarget = active ? 1.0f : 0.0f;
}
