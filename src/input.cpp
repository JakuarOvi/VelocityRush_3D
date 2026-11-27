// =============================================================================
//  input.cpp
//  Input: keyboard, joystick, mouse drag (free cam) and wheel zoom.
// =============================================================================
#include "input.h"
#include "game_state.h"
#include "simulation.h"
#include "persistence.h"

void quitGame() { saveBest(); exit(0); }
void keyDown(unsigned char k, int, int) {
    if (k >= 'A' && k <= 'Z') k = k - 'A' + 'a';
    keys[k] = true;
    switch (k) {
    case 27: if (state == ST_PLAY) state = ST_PAUSE; else if (state == ST_PAUSE) { state = ST_MENU; saveBest(); } else quitGame(); break;
    case 13: if (state == ST_MENU) { resetRun(); state = ST_PLAY; } break;
    case 'p': if (state == ST_PLAY) state = ST_PAUSE; else if (state == ST_PAUSE) state = ST_PLAY; break;
    case 'm': if (state != ST_MENU) { state = ST_MENU; saveBest(); } break;
    case 'r': if (state != ST_MENU) { saveBest(); resetRun(); state = ST_PLAY; showMsg("New run"); } break;
    case 'c': camMode = (camMode + 1) % 3; break;
    case 'f': weather = (weather + 1) % 3; break;
    case 't': autodrive = !autodrive; break;
    case 'h': headForce = !headForce; break;
    case 'n': dayRun = !dayRun; break;
    case ',': tod = fmodf(tod + 0.98f, 1.0f); break;
    case '.': tod = fmodf(tod + 0.02f, 1.0f); break;
    case '1': carType = 0; break; case '2': carType = 1; break; case '3': carType = 2; break;
    }
}
void keyUp(unsigned char k, int, int) { if (k >= 'A' && k <= 'Z') k = k - 'A' + 'a'; keys[k] = false; }
void specDown(int k, int, int) { if (k < 256) sp[k] = true; if (state == ST_MENU) { if (k == GLUT_KEY_LEFT) carType = (carType + 2) % 3; if (k == GLUT_KEY_RIGHT) carType = (carType + 1) % 3; } }
void specUp(int k, int, int) { if (k < 256) sp[k] = false; }
void joystickFn(unsigned int buttons, int x, int, int) {
    float axis = clampf(x / 1000.0f, -1.0f, 1.0f);
    controllerSteer = fabsf(axis) < 0.12f ? 0.0f : (axis > 0 ? (axis - 0.12f) / 0.88f : (axis + 0.12f) / 0.88f);
    keys[' '] = (buttons & 1u) != 0;
}
void mouse(int b, int s, int x, int y) { if (b == GLUT_LEFT_BUTTON) { dragging = (s == GLUT_DOWN); lastMX = x; lastMY = y; } }
void motion(int x, int y) { if (!dragging) return; freeYaw -= (x - lastMX) * 0.008f; freePitch = clampf(freePitch + (y - lastMY) * 0.008f, 0.05f, 1.4f); lastMX = x; lastMY = y; if (state == ST_PLAY) camMode = 2; }
void wheelFn(int, int dir, int, int) { freeDist = clampf(freeDist - dir * 1.0f, 5.0f, 40.0f); }
