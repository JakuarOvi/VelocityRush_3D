// =============================================================================
//  main.cpp
//  Program entry: window/GL setup, resource init, GLUT callback registration.
// =============================================================================
#include "game_state.h"
#include "persistence.h"
#include "resources.h"
#include "environment.h"
#include "simulation.h"
#include "renderer.h"
#include "input.h"
#include "noise.h"

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutSetOption(GLUT_MULTISAMPLE, 4);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowSize(winW, winH);
    glutCreateWindow("Velocity Rush 3D");
    glutIgnoreKeyRepeat(1);
    quad = gluNewQuadric(); gluQuadricNormals(quad, GLU_SMOOTH);
    glEnable(GL_MULTISAMPLE); glEnable(GL_DEPTH_TEST); glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    makeTextures(); buildLists(); loadBest(); loadPhysicsConfig();
    srand(1234);
    for (int i = 0; i < NRAIN; i++) rainP[i] = v3((float)(rand() % 6000) / 100.0f, (float)(rand() % 4000) / 100.0f, (float)(rand() % 6000) / 100.0f);
    for (int i = 0; i < NSTAR; i++) {
        float y = 0.03f + (rand() % 1000) / 1000.0f * 0.97f, a = (rand() % 6283) / 1000.0f, r = sqrtf(1 - y * y);
        starP[i] = v3(r * cosf(a), y, r * sinf(a)); starB[i] = 0.35f + 0.65f * (rand() % 100) / 100.0f;
    }
    memset(keys, 0, sizeof(keys)); memset(sp, 0, sizeof(sp));
    updateEnv(0);
    glutDisplayFunc(display); glutReshapeFunc(reshape);
    glutKeyboardFunc(keyDown); glutKeyboardUpFunc(keyUp);
    glutSpecialFunc(specDown); glutSpecialUpFunc(specUp);
    glutJoystickFunc(joystickFn, 16);
    glutMouseFunc(mouse); glutMotionFunc(motion); glutMouseWheelFunc(wheelFn);
    glutTimerFunc(8, tick, 0);
    atexit(saveBest);
    if (getenv("VR_DEBUG")) { resetRun(); state = ST_PLAY; autodrive = true; weather = atoi(getenv("VR_DEBUG")) % 3; tod = getenv("VR_TOD") ? (float)atof(getenv("VR_TOD")) : tod; dayRun = false; }
    glutMainLoop();
    return 0;
}
