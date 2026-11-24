// =============================================================================
//  renderer.cpp
//  Frame rendering order and GLUT display / timer / reshape callbacks.
// =============================================================================
#include "renderer.h"
#include "game_state.h"
#include "camera.h"
#include "sky.h"
#include "lighting.h"
#include "headlights.h"
#include "terrain.h"
#include "road_render.h"
#include "trees.h"
#include "coins.h"
#include "car.h"
#include "rain.h"
#include "hud.h"
#include "simulation.h"
#include "gfx_util.h"

void render() {
    computeCarFrame();
    glClearColor(env.hor[0], env.hor[1], env.hor[2], 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, winW, winH);
    applyCamera();

    drawSky(eyePos);

    glEnable(GL_DEPTH_TEST); glEnable(GL_LIGHTING); glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL); glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
    setupLighting(); setupHeadlights();
    setEmis(0, 0, 0);

    drawTerrain(); drawRoad(); drawTrees(); drawCoins();
    if (camMode != 1 || state == ST_MENU) drawCarFull();
    drawBeams(); drawRain();
    drawHUD();
}

void display() {
    static int last = 0; int now = glutGet(GLUT_ELAPSED_TIME);
    float dt = (now - last) / 1000.0f; last = now; if (dt > 0.05f) dt = 0.05f; if (dt < 0) dt = 0;
    update(dt); render(); glutSwapBuffers();
}
void tick(int) { glutPostRedisplay(); glutTimerFunc(8, tick, 0); }
void reshape(int w, int h) { winW = w; winH = h > 0 ? h : 1; }
