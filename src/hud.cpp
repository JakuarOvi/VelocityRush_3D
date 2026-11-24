// =============================================================================
//  hud.cpp
//  HUD: 2D overlay setup + in-game info (distance, best, coins, clock, boost bar, key hints).
// =============================================================================
#include "hud.h"
#include "menu.h"
#include "speedometer.h"
#include "hud_util.h"
#include "game_state.h"
#include "minimap.h"

void drawPlayHUD() {
    char b[160];
    drawSpeedo(winW - 130.0f, 470.0f, 100.0f);
    drawTacho(winW - 290.0f, 470.0f, 70.0f);
    drawMinimap();
    glColor4f(0, 0, 0, 0.45f); rect(14, winH - 118.0f, 300, 104);
    glColor3f(1, 1, 1);
    sprintf(b, "Distance  %.2f km", dist / 1000.0f); text(26, winH - 38.0f, b);
    sprintf(b, "Best      %.2f km", bestDist / 1000.0f); text(26, winH - 62.0f, b);
    sprintf(b, "Coins     %d", coins); glColor3f(1, 0.85f, 0.25f); text(26, winH - 86.0f, b);
    float hrs = tod * 24.0f; sprintf(b, "%02d:%02d   %s   %s", (int)hrs, (int)((hrs - (int)hrs) * 60), wName[weather], camMode == 0 ? "3rd person" : (camMode == 1 ? "1st person" : "Free cam"));
    glColor3f(0.85f, 0.9f, 1.0f); text(26, winH - 108.0f, b, GLUT_BITMAP_HELVETICA_12);
    // boost bar
    glColor4f(0, 0, 0, 0.5f); rect(winW - 230.0f, 300, 200, 16);
    glColor3f(boosting ? 1.0f : 0.2f, 0.6f, 1.0f); rect(winW - 228.0f, 302, 196 * clampf(boostCh, 0, 1), 12);
    glColor3f(1, 1, 1); text(winW - 230.0f, 321, "BOOST (SPACE)", GLUT_BITMAP_HELVETICA_12);
    if (autodrive) { glColor3f(0.3f, 1, 0.4f); textC(winW / 2.0f, winH - 40.0f, "AUTODRIVE", GLUT_BITMAP_TIMES_ROMAN_24); }
    glColor4f(1, 1, 1, 0.75f);
    text(14, 12, "W/S throttle/brake  A/D steer  SPACE boost  T autodrive  C camera  F weather  H lights  N/,/. time  P pause  R reset  M menu", GLUT_BITMAP_HELVETICA_12);
    if (msgTimer > 0) { glColor4f(1, 0.35f, 0.25f, clampf(msgTimer, 0, 1)); textC(winW / 2.0f, winH * 0.62f, msg, GLUT_BITMAP_TIMES_ROMAN_24); }
}

void drawHUD() {
    glDisable(GL_LIGHTING); glDisable(GL_FOG); glDisable(GL_DEPTH_TEST); glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity(); gluOrtho2D(0, winW, 0, winH);
    glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
    if (state != ST_MENU) drawPlayHUD();
    if (state == ST_PAUSE) drawPauseScreen();
    if (state == ST_MENU) drawMenuScreen();
    glMatrixMode(GL_PROJECTION); glPopMatrix(); glMatrixMode(GL_MODELVIEW); glPopMatrix();
    glDisable(GL_BLEND); glEnable(GL_DEPTH_TEST); glEnable(GL_LIGHTING);
}
