// =============================================================================
//  camera.cpp
//  Cameras: third person (smoothed chase), first person, free orbit, menu showcase.
// =============================================================================
#include "camera.h"
#include "game_state.h"
#include "road.h"

void applyCamera() {
    float fov = 62.0f + (carV / 60.0f) * 14.0f + (boosting ? 8.0f : 0.0f) + (camMode == 1 ? 8.0f : 0.0f);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    if (state == ST_MENU) glTranslatef(0.32f, 0, 0);   // push the showcase car to the right of the menu panel
    gluPerspective(fov, winW / (float)winH, 0.3, 1600.0);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    V3 eye, at; V3 up = v3(0, 1, 0);
    if (state == ST_MENU) {
        eye = carPos + v3(sinf(menuSpin) * 8.5f, 2.6f, cosf(menuSpin) * 8.5f); at = carPos + v3(0, 0.8f, 0); camInit = false;
    } else if (camMode == 0) {
        V3 want = carPos - carFwd * (8.5f + carV * 0.04f) + v3(0, 3.3f, 0);
        float minY = roadY(carS - 9.0f) + 1.5f; if (want.y < minY) want.y = minY;
        if (!camInit) { camPos = want; camInit = true; }
        float k = 1 - expf(-13.0f * frameDt); camPos = camPos + (want - camPos) * k;
        eye = camPos; at = carPos + carFwd * 7.0f + v3(0, 1.4f, 0);
    } else if (camMode == 1) {
        eye = carPos + carFwd * 0.4f + v3(0, 1.18f, 0); at = eye + carFwd * 12.0f;
    } else {
        eye = carPos + v3(freeDist * cosf(freePitch) * sinf(freeYaw), freeDist * sinf(freePitch), freeDist * cosf(freePitch) * cosf(freeYaw));
        if (eye.y < carPos.y + 0.4f) eye.y = carPos.y + 0.4f; at = carPos + v3(0, 0.9f, 0);
    }
    eyePos = eye;
    gluLookAt(eye.x, eye.y, eye.z, at.x, at.y, at.z, up.x, up.y, up.z);
}
