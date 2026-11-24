// =============================================================================
//  environment.cpp
//  Environment: sun / moon / sky colours / weather blending.
// =============================================================================
#include "environment.h"
#include "game_state.h"

void updateEnv(float dt) {
    float k = 1.0f - expf(-1.2f * dt);
    cloudCur += (wCloud[weather] - cloudCur) * k;
    fogCur += (wFog[weather] - fogCur) * k;
    rainCur += (wRain[weather] - rainCur) * k;
    float ang = (tod - 0.25f) * 2.0f * PI;
    env.sunDir = norm(v3(cosf(ang) * 0.9f, sinf(ang), -0.45f));
    env.elev = env.sunDir.y;
    env.dayF = sstep(-0.08f, 0.30f, env.elev);
    env.duskF = expf(-powf(env.elev / 0.20f, 2.0f));
    env.nightF = 1.0f - sstep(-0.30f, 0.05f, env.elev);
    const float nz[3] = { 0.010f, 0.015f, 0.055f }, dz[3] = { 0.16f, 0.40f, 0.85f };
    const float nh[3] = { 0.030f, 0.040f, 0.090f }, dh[3] = { 0.62f, 0.80f, 0.95f };
    const float dusk[3] = { 1.0f, 0.52f, 0.25f };
    mix3(nz, dz, env.dayF, env.zen); mix3(nh, dh, env.dayF, env.hor);
    mix3(env.hor, dusk, env.duskF * 0.75f * (env.elev > -0.25f ? 1.0f : 0.0f), env.hor);
    float gl = 0.25f + 0.75f * env.dayF;
    const float grey[3] = { 0.55f * gl, 0.58f * gl, 0.62f * gl };
    mix3(env.zen, grey, cloudCur * 0.9f, env.zen);
    mix3(env.hor, grey, cloudCur * 0.85f, env.hor);
}
