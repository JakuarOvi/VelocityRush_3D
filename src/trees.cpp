// =============================================================================
//  trees.cpp
//  Trees (pine + round) generated from the distance travelled.
// =============================================================================
#include "trees.h"
#include "game_state.h"
#include "road.h"
#include "terrain.h"
#include "noise.h"
#include "gfx_util.h"

void drawTrees() {
    setSpec(0.02f, 4);
    for (int r = 0; r < NR; r++) {
        int idx = (int)floorf(rowS[r] / STEP + 0.5f);
        for (int si = 0; si < 2; si++) for (int k = 0; k < 2; k++) {
            int key = si * 2 + k;
            if (hash01(idx * 7 + key, 5) > 0.62f) continue;
            float lat = 7.0f + hash01(idx, 100 + key) * 48.0f;
            float s = rowS[r] + hash01(idx, 200 + key) * STEP;
            int side = si ? 1 : -1;
            float h = terrH(s, lat, side); if (h > roadY(s) + 40.0f) continue;
            V3 p = roadP(s) + roadR(s) * (side * (HALFW + lat)); p.y = h - 0.15f;
            float sc = 0.8f + hash01(idx, 300 + key) * 0.9f;
            bool pine = hash01(idx, 400 + key) < 0.6f;
            float v = hash01(idx, 500 + key);
            glPushMatrix(); glTranslatef(p.x, p.y, p.z); glScalef(sc, sc, sc); glRotatef(v * 360, 0, 1, 0);
            glColor3f(0.30f, 0.20f, 0.12f); glCallList(listTrunk);
            if (pine) { glColor3f(0.06f + 0.05f * v, 0.26f + 0.08f * v, 0.12f); glCallList(listPine); }
            else {
                if (v < 0.18f) glColor3f(0.80f, 0.40f, 0.08f); else if (v < 0.30f) glColor3f(0.75f, 0.65f, 0.12f); else glColor3f(0.16f + 0.1f * v, 0.42f + 0.1f * v, 0.10f);
                glCallList(listRound);
            }
            glPopMatrix();
        }
    }
}
