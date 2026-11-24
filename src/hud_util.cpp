// =============================================================================
//  hud_util.cpp
//  HUD helpers: bitmap text, centred text, rectangles.
// =============================================================================
#include "hud_util.h"

void text(float x, float y, const char* s, void* font) {
    glRasterPos2f(x, y); for (; *s; s++) glutBitmapCharacter(font, *s);
}
void textC(float cx, float y, const char* s, void* font) {
    int w = glutBitmapLength(font, (const unsigned char*)s); text(cx - w / 2.0f, y, s, font);
}
void rect(float x, float y, float w, float h) { glBegin(GL_QUADS); glVertex2f(x, y); glVertex2f(x + w, y); glVertex2f(x + w, y + h); glVertex2f(x, y + h); glEnd(); }
