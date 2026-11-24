// =============================================================================
//  resources.cpp
//  Procedural detail texture + display lists (tree trunk / pine / round tree).
// =============================================================================
#include "resources.h"
#include "game_state.h"
#include "noise.h"

void makeTextures() {
    const int N = 256; static unsigned char img[N * N * 3];
    for (int y = 0; y < N; y++) for (int x = 0; x < N; x++) {
        float u = x / (float)N, v = y / (float)N;
        float n = 0.5f * vnoiseP(u * 8, v * 8, 8) + 0.3f * vnoiseP(u * 16, v * 16, 16) + 0.2f * vnoiseP(u * 64, v * 64, 64);
        float g = (hash01(x, y + 999) - 0.5f) * 0.10f;
        float c = clampf(0.52f + 0.48f * n + g, 0, 1);
        unsigned char b = (unsigned char)(c * 255);
        img[(y * N + x) * 3 + 0] = b; img[(y * N + x) * 3 + 1] = b; img[(y * N + x) * 3 + 2] = b;
    }
    glGenTextures(1, &texDetail); glBindTexture(GL_TEXTURE_2D, texDetail);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, N, N, GL_RGB, GL_UNSIGNED_BYTE, img);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8.0f);
    glGetError();   // swallow error if anisotropy is unsupported
}
void buildLists() {
    listTrunk = glGenLists(3); listPine = listTrunk + 1; listRound = listTrunk + 2;
    glNewList(listTrunk, GL_COMPILE);
    glPushMatrix(); glRotatef(-90, 1, 0, 0); gluCylinder(quad, 0.28, 0.18, 2.6, 8, 1); glPopMatrix();
    glEndList();
    glNewList(listPine, GL_COMPILE);
    glPushMatrix(); glTranslatef(0, 1.3f, 0); glRotatef(-90, 1, 0, 0); glutSolidCone(2.3, 3.6, 10, 2); glPopMatrix();
    glPushMatrix(); glTranslatef(0, 3.3f, 0); glRotatef(-90, 1, 0, 0); glutSolidCone(1.8, 3.0, 10, 2); glPopMatrix();
    glPushMatrix(); glTranslatef(0, 5.0f, 0); glRotatef(-90, 1, 0, 0); glutSolidCone(1.2, 2.6, 10, 2); glPopMatrix();
    glEndList();
    glNewList(listRound, GL_COMPILE);
    glPushMatrix(); glTranslatef(0, 3.8f, 0); glScalef(1.0f, 0.85f, 1.0f); glutSolidSphere(2.3, 14, 10); glPopMatrix();
    glPushMatrix(); glTranslatef(0.9f, 3.0f, 0.5f); glutSolidSphere(1.5, 10, 8); glPopMatrix();
    glEndList();
}
