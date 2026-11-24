// =============================================================================
//  minimap.cpp
//  Top-down map of the road ahead: the car is at the bottom, road curves upward.
// =============================================================================
#include "minimap.h"
#include "game_state.h"
#include "road.h"
#include "hud_util.h"

static const float MX = 14.0f, MY = 380.0f, MW = 190.0f, MH = 200.0f;
static const float MSC = 0.6f;   // pixels per metre

void drawMinimap() {
    float cx = MX + MW * 0.5f, cy = MY + MH * 0.2f;
    glColor4f(0, 0, 0, 0.55f); rect(MX, MY, MW, MH);

    glEnable(GL_SCISSOR_TEST); glScissor((GLint)MX, (GLint)MY, (GLsizei)MW, (GLsizei)MH);
    // road surface (horizontal half-width = HALFW * R.x in world x)
    glColor3f(0.42f, 0.42f, 0.44f);
    glBegin(GL_TRIANGLE_STRIP);
    for (float s = carS - 40.0f; s <= carS + 380.0f; s += 4.0f) {
        float px = cx + (roadX(s) - roadX(carS)) * MSC;
        float py = cy + (s - carS) * MSC;
        float hw = HALFW * roadR(s).x * MSC;
        glVertex2f(px - hw, py); glVertex2f(px + hw, py);
    }
    glEnd();
    // dashed centre line
    glColor4f(1, 1, 1, 0.7f); glBegin(GL_LINES);
    for (float s = floorf((carS - 40.0f) / 12.0f) * 12.0f; s < carS + 380.0f; s += 12.0f) {
        float a = s, b = s + 6.0f;
        glVertex2f(cx + (roadX(a) - roadX(carS)) * MSC, cy + (a - carS) * MSC);
        glVertex2f(cx + (roadX(b) - roadX(carS)) * MSC, cy + (b - carS) * MSC);
    }
    glEnd();
    // car marker (points up the road)
    glColor3f(1, 0.25f, 0.2f); glBegin(GL_TRIANGLES);
    glVertex2f(cx, cy + 10); glVertex2f(cx - 7, cy - 7); glVertex2f(cx + 7, cy - 7);
    glEnd();
    glDisable(GL_SCISSOR_TEST);

    glColor4f(1, 1, 1, 0.3f); glBegin(GL_LINE_LOOP);
    glVertex2f(MX, MY); glVertex2f(MX + MW, MY); glVertex2f(MX + MW, MY + MH); glVertex2f(MX, MY + MH);
    glEnd();

    // turn warning: change of heading over the next 150 m (+x is screen right)
    float dh = atanf(roadDX(carS + 150.0f)) - atanf(roadDX(carS));
    float ad = fabsf(dh);
    char buf[64];
    if (ad < 0.08f) sprintf(buf, "Road straight ahead");
    else sprintf(buf, "%s%s turn ahead", ad > 0.35f ? "Sharp " : "", dh > 0 ? "right" : "left");
    glColor4f(1, 1, 0.6f, 0.95f);
    text(MX, MY + MH + 8.0f, buf, GLUT_BITMAP_HELVETICA_12);
}
