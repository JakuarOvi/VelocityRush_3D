// =============================================================================
//  lighting.cpp
//  Scene lighting (sun/moon) and fog setup.
// =============================================================================
#include "lighting.h"
#include "game_state.h"

void setupLighting() {
    bool sun = env.elev > -0.02f;
    V3 L = sun ? env.sunDir : env.sunDir * -1.0f;
    GLfloat pos[4] = { L.x, L.y, L.z, 0 };
    const float moon[3] = { 0.10f, 0.13f, 0.27f }, day[3] = { 1.0f, 0.97f, 0.90f };
    float col[3]; mix3(moon, day, env.dayF, col);
    col[0] += 0.55f * env.duskF * env.dayF * 0.6f; col[1] += 0.18f * env.duskF * env.dayF * 0.6f;
    float wk = 1.0f - 0.65f * cloudCur;
    GLfloat dif[4] = { col[0] * wk, col[1] * wk, col[2] * wk, 1 };
    GLfloat spc[4] = { col[0] * 0.9f * (1 - cloudCur), col[1] * 0.9f * (1 - cloudCur), col[2] * 0.9f * (1 - cloudCur), 1 };
    const float na[3] = { 0.09f, 0.10f, 0.19f }, da[3] = { 0.36f, 0.40f, 0.48f }; float am[3]; mix3(na, da, env.dayF, am);
    GLfloat amb[4] = { am[0], am[1], am[2], 1 }; GLfloat zero[4] = { 0, 0, 0, 1 };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
    glLightfv(GL_LIGHT0, GL_AMBIENT, zero); glLightfv(GL_LIGHT0, GL_DIFFUSE, dif); glLightfv(GL_LIGHT0, GL_SPECULAR, spc);
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    glEnable(GL_LIGHT0);
    // fog
    GLfloat fc[4] = { env.hor[0], env.hor[1], env.hor[2], 1 };
    glEnable(GL_FOG); glFogi(GL_FOG_MODE, GL_EXP2); glFogf(GL_FOG_DENSITY, fogCur); glFogfv(GL_FOG_COLOR, fc);
    glHint(GL_FOG_HINT, GL_NICEST);
}
