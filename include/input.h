// =============================================================================
//  input.h
//  Keyboard, special keys, mouse and wheel callbacks.
// =============================================================================
#ifndef INPUT_H
#define INPUT_H

void keyDown(unsigned char k, int, int);
void keyUp(unsigned char k, int, int);
void specDown(int k, int, int);
void specUp(int k, int, int);
void mouse(int b, int s, int x, int y);
void motion(int x, int y);
void wheelFn(int, int dir, int, int);
void quitGame();

#endif
