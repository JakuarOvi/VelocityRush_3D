// =============================================================================
//  rain.cpp
//  Rain particles.
// =============================================================================
#include "rain.h"
#include "game_state.h"
#include "noise.h"

void drawRain() {
    if (rainCur < 0.03f) return;
    glDisable(GL_LIGHTING); glDisable(GL_FOG); glDisable(GL_TEXTURE_2D); glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); glDepthMask(GL_FALSE);
    glLineWidth(1.3f);
    int n = (int)(NRAIN * rainCur);
    float lum = 0.35f + 0.65f * env.dayF;
    glColor4f(0.75f * lum, 0.80f * lum, 0.92f * lum, 0.40f * rainCur);
    glBegin(GL_LINES);
    for (int i = 0; i < n; i++) {
        rainP[i].y -= 38.0f * frameDt * (0.8f + 0.4f * hash01(i, 3));
        float x = eyePos.x + wrapf(rainP[i].x - eyePos.x, 60), z = eyePos.z + wrapf(rainP[i].z - eyePos.z, 60);
        float y = eyePos.y + 10 + wrapf(rainP[i].y - eyePos.y - 10, 40);
        glVertex3f(x, y, z); glVertex3f(x - 0.05f, y + 1.0f + carV * 0.02f, z + carV * 0.03f);
    }
    glEnd();
    glDepthMask(GL_TRUE); glDisable(GL_BLEND); glEnable(GL_LIGHTING); glEnable(GL_FOG);
}
