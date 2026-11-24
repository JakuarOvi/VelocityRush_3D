// =============================================================================
//  sky.cpp
//  Sky dome, sun/moon glow, stars and clouds.
// =============================================================================
#include "sky.h"
#include "game_state.h"
#include "noise.h"

void glowDisc(V3 dir, float radius, float r, float g, float b, float a) {
    V3 up = fabsf(dir.y) > 0.99f ? v3(1, 0, 0) : v3(0, 1, 0);
    V3 rt = norm(cross(dir, up)); V3 u2 = cross(rt, dir);
    V3 c = dir * 790.0f;
    glBegin(GL_TRIANGLE_FAN);
    glColor4f(r, g, b, a); glVertex3f(c.x, c.y, c.z);
    glColor4f(r, g, b, 0);
    for (int i = 0; i <= 32; i++) { float p = i * 2 * PI / 32; V3 q = c + (rt * cosf(p) + u2 * sinf(p)) * radius; glVertex3f(q.x, q.y, q.z); }
    glEnd();
}

void drawSky(V3 eye) {
    glDisable(GL_LIGHTING); glDisable(GL_FOG); glDisable(GL_TEXTURE_2D); glDisable(GL_DEPTH_TEST); glDepthMask(GL_FALSE);
    glPushMatrix(); glTranslatef(eye.x, eye.y, eye.z);
    const int ST = 16, SL = 48; const float R = 800.0f;
    for (int i = 0; i < ST; i++) {
        float e0 = -0.25f + (PI / 2 + 0.25f) * i / ST, e1 = -0.25f + (PI / 2 + 0.25f) * (i + 1) / ST;
        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j <= SL; j++) {
            float az = j * 2 * PI / SL;
            for (int k = 0; k < 2; k++) {
                float el = k ? e1 : e0;
                V3 d = v3(cosf(el) * sinf(az), sinf(el), -cosf(el) * cosf(az));
                float h = el > 0 ? el / (PI / 2) : 0; float t = powf(h, 0.55f);
                float c[3]; mix3(env.hor, env.zen, t, c);
                if (el > 0) {
                    float sd = fmaxf(dot(d, env.sunDir), 0);
                    float glow = powf(sd, 6.0f) * env.duskF * 0.8f * (1 - cloudCur * 0.7f) * (1 - t * 0.6f);
                    float halo = powf(sd, 48.0f) * 0.5f * env.dayF * (1 - cloudCur);
                    c[0] += 1.0f * glow + halo; c[1] += 0.45f * glow + halo; c[2] += 0.15f * glow + halo * 0.9f;
                }
                glColor3f(clampf(c[0], 0, 1), clampf(c[1], 0, 1), clampf(c[2], 0, 1));
                glVertex3f(R * d.x, R * d.y, R * d.z);
            }
        }
        glEnd();
    }
    // stars, sun, moon
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    float sa = env.nightF * (1 - cloudCur);
    if (sa > 0.02f) {
        glEnable(GL_POINT_SMOOTH); glPointSize(1.7f); glBegin(GL_POINTS);
        for (int i = 0; i < NSTAR; i++) { glColor4f(1, 1, 1, sa * starB[i]); glVertex3f(starP[i].x * 780, starP[i].y * 780, starP[i].z * 780); }
        glEnd();
    }
    if (env.sunDir.y > -0.12f) {
        float a = (1 - cloudCur * 0.75f);
        float wr = 1.0f, wg = mixf(0.55f, 0.92f, env.dayF), wb = mixf(0.25f, 0.75f, env.dayF);
        glowDisc(env.sunDir, 190, wr, wg * 0.8f, wb * 0.6f, 0.35f * a);
        glowDisc(env.sunDir, 48, wr, wg, wb, 0.9f * a);
        glowDisc(env.sunDir, 26, 1, 1, 0.95f, 1.0f * a);
    }
    V3 md = env.sunDir * -1.0f;
    if (md.y > -0.1f) {
        float a = env.nightF * (1 - cloudCur * 0.8f);
        glowDisc(md, 90, 0.5f, 0.6f, 0.9f, 0.25f * a);
        glowDisc(md, 24, 0.9f, 0.93f, 1.0f, 0.95f * a);
    }
    // clouds
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    float lum = 0.25f + 0.75f * env.dayF;
    int n = 10 + (int)(cloudCur * 18);
    for (int i = 0; i < n; i++) {
        float hx = hash01(i, 1), hy = hash01(i, 2), hz = hash01(i, 3), hs = hash01(i, 4);
        float rx = wrapf(hx * 3200 + simTime * 5.0f - eye.x, 3200);
        float rz = wrapf(hz * 3200 - eye.z, 3200);
        float dd = sqrtf(rx * rx + rz * rz); if (dd < 200) continue;
        float y = 150 + hy * 100 - eye.y * 0.0f;
        float alpha = (0.55f + 0.35f * cloudCur) * (1.0f - dd / 1600.0f);
        float cr = mixf(1.0f, 0.62f, cloudCur) * lum + 0.35f * env.duskF, cg = mixf(1.0f, 0.64f, cloudCur) * lum + 0.12f * env.duskF, cb = mixf(1.0f, 0.68f, cloudCur) * lum;
        glColor4f(clampf(cr, 0, 1), clampf(cg, 0, 1), clampf(cb, 0, 1), alpha);
        glPushMatrix(); glTranslatef(rx, y, rz);
        for (int p = 0; p < 5; p++) {
            float rr = 38 + 30 * hash01(i, 10 + p) + 18 * hs;
            glPushMatrix(); glTranslatef((p - 2) * 42.0f * (0.7f + hs * 0.5f), hash01(i, 20 + p) * 8.0f, (hash01(i, 30 + p) - 0.5f) * 40.0f);
            glScalef(rr * 1.5f, rr * 0.30f, rr); glutSolidSphere(1.0, 12, 7); glPopMatrix();
        }
        glPopMatrix();
    }
    glDisable(GL_BLEND);
    glPopMatrix();
    glDepthMask(GL_TRUE); glEnable(GL_DEPTH_TEST);
}
