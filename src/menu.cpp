// =============================================================================
//  menu.cpp
//  Main menu (car picker, controls list) and pause screen.
// =============================================================================
#include "menu.h"
#include "game_state.h"
#include "hud_util.h"

void drawPauseScreen() {
    glColor4f(0, 0, 0, 0.5f); rect(0, 0, (float)winW, (float)winH);
    glColor3f(1, 1, 1); textC(winW / 2.0f, winH / 2.0f + 10, "PAUSED", GLUT_BITMAP_TIMES_ROMAN_24);
    textC(winW / 2.0f, winH / 2.0f - 20, "P - resume     ESC - main menu", GLUT_BITMAP_HELVETICA_18);
}

void drawMenuScreen() {
    char b[160];
    float px = 24, py = winH / 2.0f - 215; float mx = px + 340;
    glColor4f(0.02f, 0.03f, 0.06f, 0.72f); rect(px, py, 680, 430);
    glColor3f(1.0f, 0.75f, 0.2f); textC(mx, py + 385, "VELOCITY RUSH 3D", GLUT_BITMAP_TIMES_ROMAN_24);
    glColor3f(0.7f, 0.8f, 0.95f); textC(mx, py + 360, "Endless procedural scenic driving simulator", GLUT_BITMAP_HELVETICA_12);
    const CarSpec& cs = cars[carType];
    glColor3f(cs.r * 0.5f + 0.5f, cs.g * 0.5f + 0.5f, cs.b * 0.5f + 0.5f);
    sprintf(b, "<   %s   >", cs.name); textC(mx, py + 320, b, GLUT_BITMAP_TIMES_ROMAN_24);
    glColor3f(1, 1, 1); sprintf(b, "Top speed %d km/h   |   %s", (int)(cs.vmax * 3.6f), cs.desc); textC(mx, py + 295, b);
    glColor3f(0.95f, 0.95f, 0.95f);
    float y = py + 250; const char* lines[] = {
        "1 / 2 / 3  or  LEFT / RIGHT  -  choose car",
        "F - weather (sunny / foggy / rainy)      N , .  - time of day",
        "C - camera (3rd / 1st / free)      T - autodrive      H - headlights",
        "W S / UP DOWN - throttle, brake      A D / LEFT RIGHT - steer",
        "SPACE - boost      P - pause      R - restart run      ESC - quit" };
    for (int i = 0; i < 5; i++) { textC(mx, y, lines[i]); y -= 26; }
    sprintf(b, "Best distance: %.2f km", bestDist / 1000.0f); glColor3f(1, 0.85f, 0.3f); textC(mx, py + 100, b);
    float pulse = 0.6f + 0.4f * sinf(simTime * 4); glColor3f(pulse, 1.0f * pulse, pulse);
    textC(mx, py + 50, "Press ENTER to start", GLUT_BITMAP_TIMES_ROMAN_24);
}
