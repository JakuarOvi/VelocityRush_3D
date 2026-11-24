// =============================================================================
//  hud_util.h
//  HUD drawing helpers: bitmap text and rectangles.
// =============================================================================
#ifndef HUD_UTIL_H
#define HUD_UTIL_H
#include "common.h"

void text(float x, float y, const char* s, void* font = GLUT_BITMAP_HELVETICA_18);
void textC(float cx, float y, const char* s, void* font = GLUT_BITMAP_HELVETICA_18);
void rect(float x, float y, float w, float h);

#endif
