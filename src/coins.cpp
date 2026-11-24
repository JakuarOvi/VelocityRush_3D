// =============================================================================
//  coins.cpp
//  Coins drawn on the road (collection logic lives in simulation.cpp).
// =============================================================================
#include "coins.h"
#include "game_state.h"
#include "road.h"
#include "noise.h"
#include "gfx_util.h"

void drawCoins() {
    int k0 = (int)floorf((carS - 40.0f) / 14.0f), k1 = (int)floorf((carS + 420.0f) / 14.0f);
    setSpec(0.9f, 60);
    for (int k = (k0 < 0 ? 0 : k0); k <= k1; k++) {
        int seg = k / 8;
        if (hash01(seg, 77) < 0.5f) continue; if (coinGot.count(k)) continue;
        if (seg == 0) continue;
        float lane = (hash01(seg, 91) * 2 - 1) * 4.2f; float s = k * 14.0f + 20.0f;
        V3 p = roadP(s) + roadR(s) * lane; p.y += 1.1f;
        glPushMatrix(); glTranslatef(p.x, p.y, p.z); glRotatef(simTime * 180.0f + k * 25.0f, 0, 1, 0);
        setEmis(0.55f, 0.42f, 0.0f); glColor3f(1.0f, 0.82f, 0.12f);
        glTranslatef(0, 0, -0.06f); gluCylinder(quad, 0.5, 0.5, 0.12, 20, 1); gluDisk(quad, 0, 0.5, 20, 1);
        glTranslatef(0, 0, 0.12f); gluDisk(quad, 0, 0.5, 20, 1);
        setEmis(0, 0, 0); glColor3f(0.8f, 0.6f, 0.05f); gluDisk(quad, 0.18, 0.34, 16, 1);
        glPopMatrix();
    }
}
