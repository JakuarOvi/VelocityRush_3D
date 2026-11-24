// =============================================================================
//  car.cpp
//  Car frame + car models (sedan, sports, SUV), wheels and blob shadow.
// =============================================================================
#include "car.h"
#include "game_state.h"
#include "road.h"
#include "gfx_util.h"

void computeCarFrame() {
    float dx = roadDX(carS), dy = roadDY(carS);
    V3 p = roadP(carS) + roadR(carS) * carD;
    float slip = atan2f(carVd, fmaxf(carV, 6.0f));
    float a = -atanf(dx) - slip * 0.8f;
    float pitch = atan2f(dy, sqrtf(1 + dx * dx));
    carPos = p; carYawDeg = a * 57.29578f; carPitchDeg = pitch * 57.29578f;
    carFwd = norm(v3(-sinf(a) * cosf(pitch), sinf(pitch), -cosf(a) * cosf(pitch)));
}
void carTransform() {
    glTranslatef(carPos.x, carPos.y, carPos.z);
    glRotatef(carYawDeg, 0, 1, 0); glRotatef(carPitchDeg, 1, 0, 0);
}

static void frustumBox(float yb, float zb, float wb, float lb, float zt, float wt, float lt, float h, const float* sideC, const float* topC) {
    V3 b[4] = { v3(-wb / 2, yb, zb - lb / 2), v3(wb / 2, yb, zb - lb / 2), v3(wb / 2, yb, zb + lb / 2), v3(-wb / 2, yb, zb + lb / 2) };
    V3 t[4] = { v3(-wt / 2, yb + h, zt - lt / 2), v3(wt / 2, yb + h, zt - lt / 2), v3(wt / 2, yb + h, zt + lt / 2), v3(-wt / 2, yb + h, zt + lt / 2) };
    V3 c = v3(0, yb + h / 2, (zb + zt) / 2);
    glColor3fv(sideC); glBegin(GL_QUADS);
    for (int i = 0; i < 4; i++) {
        int j = (i + 1) % 4; V3 n = norm(cross(b[j] - b[i], t[i] - b[i]));
        V3 mid = (b[i] + b[j] + t[i] + t[j]) * 0.25f; if (dot(n, mid - c) < 0) n = n * -1.0f;
        glNormal3f(n.x, n.y, n.z); glVertex3f(b[i].x, b[i].y, b[i].z); glVertex3f(b[j].x, b[j].y, b[j].z);
        glVertex3f(t[j].x, t[j].y, t[j].z); glVertex3f(t[i].x, t[i].y, t[i].z);
    }
    glEnd();
    glColor3fv(topC); glBegin(GL_QUADS); glNormal3f(0, 1, 0);
    for (int i = 0; i < 4; i++) glVertex3f(t[i].x, t[i].y, t[i].z);
    glEnd();
}

static void wheel(float x, float y, float z, float R, float w, float steerDeg) {
    glPushMatrix(); glTranslatef(x, y, z);
    glRotatef(steerDeg, 0, 1, 0); glRotatef(wheelRot, 1, 0, 0); glRotatef(90, 0, 1, 0); glTranslatef(0, 0, -w / 2);
    setSpec(0.08f, 8); glColor3f(0.04f, 0.04f, 0.045f);
    gluCylinder(quad, R, R, w, 18, 1); gluDisk(quad, 0, R, 18, 1);
    glPushMatrix(); glTranslatef(0, 0, w); gluDisk(quad, 0, R, 18, 1); glPopMatrix();
    setSpec(0.9f, 80); glColor3f(0.75f, 0.77f, 0.8f);
    for (int e = 0; e < 2; e++) {
        float zz = e ? w + 0.012f : -0.012f;
        glPushMatrix(); glTranslatef(0, 0, zz); gluDisk(quad, 0, R * 0.62f, 18, 1);
        glColor3f(0.12f, 0.12f, 0.14f);
        for (int k = 0; k < 5; k++) { glPushMatrix(); glRotatef(k * 72.0f, 0, 0, 1); glTranslatef(R * 0.33f, 0, 0.006f); glScalef(R * 0.55f, 0.075f, 0.01f); glutSolidCube(1.0); glPopMatrix(); }
        glColor3f(0.75f, 0.77f, 0.8f); glPopMatrix();
    }
    glPopMatrix();
}

static void drawDetails(int type, float len, float wid, float bh, float by, const float* body, const float* dark) {
    float chrome[3] = { 0.78f, 0.80f, 0.84f };
    // side mirrors
    glColor3fv(body); setSpec(0.9f, 90);
    for (int s = -1; s <= 1; s += 2) box(s * (wid / 2 + 0.11f), by + bh * 0.62f, -0.55f, 0.16f, 0.12f, 0.22f);
    // door seams and handles
    for (int s = -1; s <= 1; s += 2) {
        glColor3fv(dark); setSpec(0.2f, 20);
        box(s * (wid / 2 + 0.002f), by + bh * 0.42f, 0.0f, 0.01f, 0.035f, len * 0.55f);
        glColor3fv(chrome); setSpec(0.95f, 110);
        box(s * (wid / 2 + 0.01f), by + bh * 0.58f, 0.25f, 0.02f, 0.05f, 0.18f);
    }
    // front grille with chrome strip
    glColor3fv(dark); setSpec(0.3f, 30);
    box(0, by + 0.10f, -len / 2 - 0.005f, wid * 0.5f, 0.14f, 0.03f);
    glColor3fv(chrome); setSpec(0.95f, 110);
    box(0, by + 0.17f, -len / 2 - 0.012f, wid * 0.52f, 0.02f, 0.03f);
    // chrome exhaust tips
    setSpec(0.95f, 110); glColor3fv(chrome);
    for (int s = -1; s <= 1; s += 2) {
        glPushMatrix(); glTranslatef(s * 0.28f, 0.2f, len / 2 - 0.02f);
        gluCylinder(quad, 0.045f, 0.045f, 0.06f, 10, 1); glPopMatrix();
    }
    // rear diffuser (dark) on all cars
    glColor3fv(dark); setSpec(0.2f, 20);
    box(0, 0.22f, len / 2 - 0.02f, wid * 0.7f, 0.12f, 0.1f);
    if (type == 0) {
        // sedan: small trunk lip
        glColor3fv(body); setSpec(0.9f, 90);
        box(0, by + bh * 0.95f, len / 2 - 0.25f, wid * 0.9f, 0.03f, 0.12f);
    } else if (type == 1) {
        // sports: rear wing on two uprights
        glColor3fv(body); setSpec(0.9f, 90);
        box(0, by + bh + 0.10f, len / 2 - 0.2f, wid * 0.82f, 0.05f, 0.32f);
        glColor3fv(dark); setSpec(0.2f, 20);
        box(-0.42f, by + bh + 0.02f, len / 2 - 0.2f, 0.06f, 0.2f, 0.06f);
        box( 0.42f, by + bh + 0.02f, len / 2 - 0.2f, 0.06f, 0.2f, 0.06f);
    } else {
        // SUV: running boards
        glColor3fv(dark); setSpec(0.2f, 20);
        for (int s = -1; s <= 1; s += 2) box(s * (wid / 2 + 0.05f), 0.35f, 0.0f, 0.1f, 0.06f, len * 0.7f);
    }
}

static void drawCarModel(int type, bool lamps, bool brake) {
    const CarSpec& cs = cars[type];
    float body[3] = { cs.r, cs.g, cs.b }, glass[3] = { 0.07f, 0.10f, 0.15f }, dark[3] = { 0.05f, 0.05f, 0.06f };
    float len, wid, bh, by, R, wx, wz, ww;
    if (type == 0) { len = 4.4f; wid = 1.82f; bh = 0.55f; by = 0.62f; R = 0.38f; wx = 0.93f; wz = 1.4f; ww = 0.28f; }
    else if (type == 1) { len = 4.5f; wid = 1.90f; bh = 0.42f; by = 0.52f; R = 0.36f; wx = 0.97f; wz = 1.45f; ww = 0.32f; }
    else { len = 4.6f; wid = 2.00f; bh = 0.85f; by = 0.80f; R = 0.47f; wx = 1.00f; wz = 1.45f; ww = 0.34f; }
    float roll = steerVis * (carV / cs.vmax) * 2.5f;
    glPushMatrix();
    glTranslatef(0, R, 0); glRotatef(bodyPitch, 1, 0, 0); glTranslatef(0, -R, 0);   // nose dip / squat
    glRotatef(roll, 0, 0, 1);
    setSpec(0.9f, 90);
    glColor3fv(body); box(0, by, 0, wid, bh, len);
    glColor3fv(dark); setSpec(0.2f, 20);
    box(0, 0.36f, -len / 2 + 0.06f, wid * 0.97f, 0.22f, 0.22f); box(0, 0.36f, len / 2 - 0.06f, wid * 0.97f, 0.22f, 0.22f);
    box(0, 0.30f, 0, wid * 0.9f, 0.1f, len * 0.8f);
    setSpec(0.95f, 110);
    if (type == 0) frustumBox(by + bh / 2, 0.15f, 1.62f, 2.5f, 0.25f, 1.40f, 1.5f, 0.55f, glass, body);
    else if (type == 1) {
        frustumBox(by + bh / 2, 0.3f, 1.55f, 2.2f, 0.45f, 1.30f, 1.2f, 0.40f, glass, body);
        glColor3fv(dark); box(0, 1.02f, len / 2 - 0.2f, 1.7f, 0.06f, 0.4f);
        box(-0.6f, 0.88f, len / 2 - 0.2f, 0.07f, 0.28f, 0.2f); box(0.6f, 0.88f, len / 2 - 0.2f, 0.07f, 0.28f, 0.2f);
    } else {
        frustumBox(by + bh / 2, 0.35f, 1.90f, 3.3f, 0.4f, 1.75f, 3.0f, 0.65f, glass, body);
        glColor3fv(dark); box(0, by + bh / 2 + 0.7f, 0.4f, 1.4f, 0.06f, 2.2f);
    }
    // lamps
    float lx = wid / 2 - 0.36f;
    for (int s = -1; s <= 1; s += 2) {
        if (lamps) setEmis(1.0f, 0.95f, 0.75f); else setEmis(0.25f, 0.25f, 0.22f);
        glColor3f(1, 1, 0.9f); glPushMatrix(); glTranslatef(s * lx, by + 0.06f, -len / 2 - 0.02f); glScalef(1, 0.6f, 0.5f); glutSolidSphere(0.17, 10, 8); glPopMatrix();
        float tb = brake ? 1.0f : (lamps ? 0.45f : 0.12f); setEmis(tb, 0.02f, 0.02f);
        glColor3f(0.9f, 0.05f, 0.05f); box(s * lx, by + 0.08f, len / 2 + 0.01f, 0.4f, 0.12f, 0.05f);
        setEmis(0, 0, 0);
    }
    setEmis(0, 0, 0);
    drawDetails(type, len, wid, bh, by, body, dark);
    float steerDeg = -steerVis * 25.0f;
    wheel(-wx, R, -wz, R, ww, steerDeg); wheel(wx, R, -wz, R, ww, steerDeg);
    wheel(-wx, R, wz, R, ww, 0); wheel(wx, R, wz, R, ww, 0);
    glPopMatrix();
}

void drawCarFull() {
    glPushMatrix(); carTransform();
    // blob shadow
    glDisable(GL_LIGHTING); glDisable(GL_TEXTURE_2D); glEnable(GL_BLEND); glDepthMask(GL_FALSE);
    glEnable(GL_POLYGON_OFFSET_FILL); glPolygonOffset(-4, -4);
    glColor4f(0, 0, 0, 0.38f * (1 - cloudCur * 0.6f) * (0.2f + env.dayF * 0.8f));
    glBegin(GL_TRIANGLE_FAN); glVertex3f(0, 0.03f, 0);
    for (int i = 0; i <= 24; i++) { float a = i * 2 * PI / 24; glVertex3f(cosf(a) * 1.25f, 0.03f, sinf(a) * 2.6f); }
    glEnd(); glDisable(GL_POLYGON_OFFSET_FILL); glDepthMask(GL_TRUE); glDisable(GL_BLEND); glEnable(GL_LIGHTING);
    drawCarModel(carType, (headForce || env.elev < 0.12f || weather >= 1), braking);
    glPopMatrix();
}
