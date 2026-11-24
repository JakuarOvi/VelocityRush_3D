// =============================================================================
//  road_render.cpp
//  Road rendering: asphalt (wet specular), edge lines, centre dashes, reflector posts.
// =============================================================================
#include "road_render.h"
#include "game_state.h"
#include "road.h"
#include "terrain.h"
#include "gfx_util.h"

void drawRoad() {
    float rc = 0.30f * (1 - 0.4f * rainCur);
    glEnable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D, texDetail);
    setSpec(0.05f + 0.55f * rainCur, 20 + 60 * rainCur);
    glColor3f(rc, rc, rc * 1.05f); glNormal3f(0, 1, 0);
    glBegin(GL_TRIANGLE_STRIP);
    for (int r = 0; r < NR; r++) {
        V3 P = roadP(rowS[r]), R = roadR(rowS[r]); V3 L = P - R * HALFW, Rr = P + R * HALFW;
        glTexCoord2f(0, rowS[r] * 0.12f); glVertex3f(L.x, L.y, L.z); glTexCoord2f(3, rowS[r] * 0.12f); glVertex3f(Rr.x, Rr.y, Rr.z);
    }
    glEnd(); glDisable(GL_TEXTURE_2D);
    glEnable(GL_POLYGON_OFFSET_FILL); glPolygonOffset(-2, -2);
    setSpec(0.1f, 10); setEmis(0.10f, 0.10f, 0.09f); glColor3f(0.92f, 0.92f, 0.86f);
    for (int side = -1; side <= 1; side += 2) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int r = 0; r < NR; r++) {
            V3 P = roadP(rowS[r]), R = roadR(rowS[r]);
            V3 a = P + R * (side * (HALFW - 0.60f)), b = P + R * (side * (HALFW - 0.36f));
            glVertex3f(a.x, a.y + 0.02f, a.z); glVertex3f(b.x, b.y + 0.02f, b.z);
        }
        glEnd();
    }
    float sd0 = floorf((rowS[0]) / 9.0f) * 9.0f;
    glBegin(GL_QUADS);
    for (float sd = sd0; sd < rowS[NR - 1]; sd += 9.0f) {
        V3 p0 = roadP(sd), p1 = roadP(sd + 4.5f), r0 = roadR(sd), r1 = roadR(sd + 4.5f);
        V3 a = p0 - r0 * 0.12f, b = p0 + r0 * 0.12f, c = p1 + r1 * 0.12f, d = p1 - r1 * 0.12f;
        glVertex3f(a.x, a.y + 0.03f, a.z); glVertex3f(b.x, b.y + 0.03f, b.z); glVertex3f(c.x, c.y + 0.03f, c.z); glVertex3f(d.x, d.y + 0.03f, d.z);
    }
    glEnd();
    setEmis(0, 0, 0); glDisable(GL_POLYGON_OFFSET_FILL);
    // reflector posts
    setSpec(0.3f, 20);
    float sp0 = floorf(rowS[0] / 24.0f) * 24.0f;
    for (float sd = sp0; sd < rowS[NR - 1]; sd += 24.0f) for (int side = -1; side <= 1; side += 2) {
        V3 p = roadP(sd) + roadR(sd) * (side * (HALFW + 0.9f));
        glColor3f(0.85f, 0.85f, 0.85f); box(p.x, p.y + 0.4f, p.z, 0.14f, 0.8f, 0.14f);
        setEmis(0.5f, 0.28f, 0.0f); glColor3f(1.0f, 0.6f, 0.1f); box(p.x, p.y + 0.62f, p.z, 0.16f, 0.14f, 0.16f); setEmis(0, 0, 0);
    }
}
