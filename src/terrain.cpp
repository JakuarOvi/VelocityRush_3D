// =============================================================================
//  terrain.cpp
//  Procedural terrain: hills, mountains, rock/snow colouring, detail texture.
// =============================================================================
#include "terrain.h"
#include "game_state.h"
#include "road.h"
#include "noise.h"
#include "gfx_util.h"

static V3 gP[2][NR][NC], gN[2][NR][NC]; static float gC[2][NR][NC][3];
float rowS[NR];

void drawTerrain() {
    float s0 = floorf(carS / STEP) * STEP;
    for (int r = 0; r < NR; r++) rowS[r] = s0 + (r - ROWS_BEHIND) * STEP;
    for (int si = 0; si < 2; si++) {
        int side = si ? 1 : -1;
        for (int r = 0; r < NR; r++) {
            V3 Ps = roadP(rowS[r]), R = roadR(rowS[r]);
            for (int c = 0; c < NC; c++) {
                V3 p = Ps + R * (side * (HALFW + latCol[c])); p.y = terrH(rowS[r], latCol[c], side); gP[si][r][c] = p;
            }
        }
        for (int r = 0; r < NR; r++) for (int c = 0; c < NC; c++) {
            int r0 = r > 0 ? r - 1 : 0, r1 = r < NR - 1 ? r + 1 : NR - 1, c0 = c > 0 ? c - 1 : 0, c1 = c < NC - 1 ? c + 1 : NC - 1;
            V3 n = norm(cross(gP[si][r1][c] - gP[si][r0][c], gP[si][r][c1] - gP[si][r][c0])); if (n.y < 0) n = n * -1.0f;
            gN[si][r][c] = n;
            V3 p = gP[si][r][c]; float lat = latCol[c];
            float patch = fbm(rowS[r] * 0.02f + si * 3.0f, lat * 0.05f);
            const float g1[3] = { 0.16f, 0.40f, 0.10f }, g2[3] = { 0.42f, 0.40f, 0.15f }, rock[3] = { 0.40f, 0.37f, 0.33f }, snow[3] = { 0.93f, 0.94f, 0.97f }, grav[3] = { 0.40f, 0.38f, 0.34f };
            float col[3]; mix3(g1, g2, sstep(0.42f, 0.62f, patch), col);
            float rockT = fmaxf(sstep(35, 70, p.y), sstep(0.82f, 0.5f, n.y)); mix3(col, rock, rockT, col);
            float snowT = sstep(75, 100, p.y) * sstep(0.55f, 0.8f, n.y); mix3(col, snow, snowT, col);
            mix3(col, grav, sstep(2.5f, 0.0f, lat), col);
            float wet = 1 - 0.22f * rainCur;
            for (int k = 0; k < 3; k++) gC[si][r][c][k] = col[k] * wet;
        }
    }
    glEnable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D, texDetail);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    setSpec(0.02f, 5);
    for (int si = 0; si < 2; si++) for (int c = 0; c < NC - 1; c++) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int r = 0; r < NR; r++) for (int k = 0; k < 2; k++) {
            int cc = c + k; V3 p = gP[si][r][cc], n = gN[si][r][cc];
            glColor3fv(gC[si][r][cc]); glNormal3f(n.x, n.y, n.z);
            glTexCoord2f(latCol[cc] * 0.08f + si * 0.37f, rowS[r] * 0.08f); glVertex3f(p.x, p.y, p.z);
        }
        glEnd();
    }
    glDisable(GL_TEXTURE_2D);
}
