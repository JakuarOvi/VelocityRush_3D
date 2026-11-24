// =============================================================================
//  headlights.cpp
//  Headlights: GL spot lights and the visible beam cones.
// =============================================================================
#include "headlights.h"
#include "game_state.h"
#include "car.h"

bool headlightsOn() { return headForce || env.elev < 0.12f || weather >= 1; }

void setupHeadlights() {
    bool on = headlightsOn();
    for (int i = 0; i < 2; i++) {
        GLenum L = GL_LIGHT1 + i;
        if (!on) { glDisable(L); continue; }
        glPushMatrix(); carTransform();
        GLfloat pos[4] = { i ? 0.7f : -0.7f, 0.65f, -2.0f, 1 }, dir[3] = { 0, -0.10f, -1 };
        glLightfv(L, GL_POSITION, pos); glLightfv(L, GL_SPOT_DIRECTION, dir);
        glPopMatrix();
        GLfloat dif[4] = { 1.5f, 1.4f, 1.1f, 1 }, spc[4] = { 1, 1, 0.9f, 1 }, amb[4] = { 0, 0, 0, 1 };
        glLightfv(L, GL_DIFFUSE, dif); glLightfv(L, GL_SPECULAR, spc); glLightfv(L, GL_AMBIENT, amb);
        glLightf(L, GL_SPOT_CUTOFF, 38.0f); glLightf(L, GL_SPOT_EXPONENT, 10.0f);
        glLightf(L, GL_CONSTANT_ATTENUATION, 0.6f); glLightf(L, GL_LINEAR_ATTENUATION, 0.02f); glLightf(L, GL_QUADRATIC_ATTENUATION, 0.0010f);
        glEnable(L);
    }
}

void drawBeams() {
    if (!headlightsOn()) return;
    float vis = fmaxf(env.nightF, clampf((fogCur - 0.004f) * 90.0f, 0, 1));
    if (vis < 0.05f) return;
    glDisable(GL_LIGHTING); glDisable(GL_FOG); glDisable(GL_TEXTURE_2D); glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE); glDepthMask(GL_FALSE);
    glPushMatrix(); carTransform();
    for (int i = 0; i < 2; i++) {
        float lx = i ? 0.7f : -0.7f;
        glBegin(GL_TRIANGLE_FAN);
        glColor4f(1.0f, 0.95f, 0.75f, 0.20f * vis); glVertex3f(lx, 0.65f, -2.1f);
        glColor4f(1.0f, 0.95f, 0.75f, 0);
        glVertex3f(lx - 4.5f, -0.3f, -32); glVertex3f(lx + 4.5f, -0.3f, -32); glVertex3f(lx + 4.5f, 5.0f, -32); glVertex3f(lx - 4.5f, 5.0f, -32); glVertex3f(lx - 4.5f, -0.3f, -32);
        glEnd();
    }
    glPopMatrix();
    glDepthMask(GL_TRUE); glDisable(GL_BLEND); glEnable(GL_LIGHTING); glEnable(GL_FOG);
}
