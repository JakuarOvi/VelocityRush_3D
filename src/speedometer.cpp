// =============================================================================
//  speedometer.cpp
//  Speedometer gauge (needle, ticks, km/h readout).
// =============================================================================
#include "speedometer.h"
#include "game_state.h"
#include "hud_util.h"

void drawSpeedo(float cx, float cy, float R) {
    float kmh = carV * 3.6f; const float maxK = 260.0f; char buf[32];
    glColor4f(0, 0, 0, 0.55f); glBegin(GL_TRIANGLE_FAN); glVertex2f(cx, cy);
    for (int i = 0; i <= 48; i++) { float a = i * 2 * PI / 48; glVertex2f(cx + cosf(a) * R, cy + sinf(a) * R); } glEnd();
    glLineWidth(3); glColor4f(0.9f, 0.9f, 0.9f, 0.9f); glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 60; i++) { float a = (225.0f - 270.0f * i / 60.0f) * PI / 180; glVertex2f(cx + cosf(a) * R * 0.94f, cy + sinf(a) * R * 0.94f); } glEnd();
    glLineWidth(2);
    for (int k = 0; k <= 260; k += 20) {
        float a = (225.0f - 270.0f * k / maxK) * PI / 180; bool major = (k % 40 == 0); float l = major ? 0.17f : 0.09f;
        if (k >= 200) glColor3f(1, 0.3f, 0.2f); else glColor3f(0.95f, 0.95f, 0.95f);
        glBegin(GL_LINES); glVertex2f(cx + cosf(a) * R * 0.93f, cy + sinf(a) * R * 0.93f); glVertex2f(cx + cosf(a) * R * (0.93f - l), cy + sinf(a) * R * (0.93f - l)); glEnd();
        if (major) { sprintf(buf, "%d", k); glColor3f(1, 1, 1); textC(cx + cosf(a) * R * 0.66f, cy + sinf(a) * R * 0.66f - 4, buf, GLUT_BITMAP_HELVETICA_12); }
    }
    float a = (225.0f - 270.0f * clampf(kmh / maxK, 0, 1)) * PI / 180;
    glLineWidth(4); glColor3f(1, 0.2f, 0.15f); glBegin(GL_LINES); glVertex2f(cx - cosf(a) * R * 0.12f, cy - sinf(a) * R * 0.12f); glVertex2f(cx + cosf(a) * R * 0.82f, cy + sinf(a) * R * 0.82f); glEnd();
    glColor3f(0.8f, 0.8f, 0.8f); glBegin(GL_TRIANGLE_FAN); glVertex2f(cx, cy); for (int i = 0; i <= 16; i++) { float b = i * 2 * PI / 16; glVertex2f(cx + cosf(b) * 7, cy + sinf(b) * 7); } glEnd();
    sprintf(buf, "%d", (int)kmh); glColor3f(1, 1, 1); textC(cx, cy - R * 0.50f, buf, GLUT_BITMAP_TIMES_ROMAN_24);
    textC(cx, cy - R * 0.70f, "km/h", GLUT_BITMAP_HELVETICA_12);
    glLineWidth(1);
}

// Tachometer: engine RPM arc (red zone above 6000) with the current gear.
void drawTacho(float cx, float cy, float R) {
    char buf[16];
    float frac = clampf(rpm / 8000.0f, 0, 1);
    glColor4f(0, 0, 0, 0.55f); glBegin(GL_TRIANGLE_FAN); glVertex2f(cx, cy);
    for (int i = 0; i <= 40; i++) { float a = i * 2 * PI / 40; glVertex2f(cx + cosf(a) * R, cy + sinf(a) * R); } glEnd();
    glLineWidth(3);
    glColor4f(0.9f, 0.9f, 0.9f, 0.9f); glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 60; i++) { float a = (225.0f - 270.0f * i / 60.0f) * PI / 180; glVertex2f(cx + cosf(a) * R * 0.94f, cy + sinf(a) * R * 0.94f); } glEnd();
    glColor3f(1, 0.2f, 0.15f); glBegin(GL_LINE_STRIP);
    for (int i = 51; i <= 60; i++) { float a = (225.0f - 270.0f * i / 60.0f) * PI / 180; glVertex2f(cx + cosf(a) * R * 0.94f, cy + sinf(a) * R * 0.94f); } glEnd();
    glLineWidth(2);
    for (int k = 0; k <= 8; k++) {
        float a = (225.0f - 270.0f * k / 8.0f) * PI / 180;
        glColor3f(0.95f, 0.95f, 0.95f);
        glBegin(GL_LINES); glVertex2f(cx + cosf(a) * R * 0.93f, cy + sinf(a) * R * 0.93f); glVertex2f(cx + cosf(a) * R * 0.80f, cy + sinf(a) * R * 0.80f); glEnd();
        if (k > 0) { sprintf(buf, "%d", k); glColor3f(1, 1, 1); textC(cx + cosf(a) * R * 0.64f, cy + sinf(a) * R * 0.64f - 4, buf, GLUT_BITMAP_HELVETICA_12); }
    }
    float a = (225.0f - 270.0f * frac) * PI / 180;
    glLineWidth(3); glColor3f(1, 0.2f, 0.15f); glBegin(GL_LINES);
    glVertex2f(cx, cy); glVertex2f(cx + cosf(a) * R * 0.85f, cy + sinf(a) * R * 0.85f); glEnd();
    sprintf(buf, "%d", gear + 1); glColor3f(1, 1, 1); textC(cx, cy - R * 0.45f, buf, GLUT_BITMAP_TIMES_ROMAN_24);
    textC(cx, cy - R * 0.72f, "x1000 rpm", GLUT_BITMAP_HELVETICA_12);
    glLineWidth(1);
}
