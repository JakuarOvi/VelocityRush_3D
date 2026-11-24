// =============================================================================
//  gfx_util.cpp
//  Small drawing helpers used by car, road and trees.
// =============================================================================
#include "gfx_util.h"
#include "common.h"

void setSpec(float s, float sh) { GLfloat v[4] = { s, s, s, 1 }; glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, v); glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, sh); }
void setEmis(float r, float g, float b) { GLfloat e[4] = { r, g, b, 1 }; glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, e); }
void box(float cx, float cy, float cz, float w, float h, float l) { glPushMatrix(); glTranslatef(cx, cy, cz); glScalef(w, h, l); glutSolidCube(1.0); glPopMatrix(); }
