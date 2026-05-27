#include "include/keyboard.h"

void Keyboard::readCamera(const bool *k, int size) {
  if (size < 226) return;

  bool right = k[SDL_SCANCODE_RIGHT];
  bool left = k[SDL_SCANCODE_LEFT];
  bool up = k[SDL_SCANCODE_UP];
  bool down = k[SDL_SCANCODE_DOWN];

  bool zoomI = (k[SDL_SCANCODE_LSHIFT] && k[SDL_SCANCODE_EQUALS]) ||
                k[SDL_SCANCODE_KP_PLUS];
  bool zoomO = (k[SDL_SCANCODE_LSHIFT] && k[SDL_SCANCODE_MINUS]) || 
                k[SDL_SCANCODE_KP_MINUS];

  cameraAction->X = 0;
  cameraAction->Y = 0;
  cameraAction->zoom = 0;

  //X movement
  if (right && left) {
    cameraAction->X = 0;
  } else if (right) {
    cameraAction->X = 1;
  } else if (left) {
    cameraAction->X = -1;
  }

  //Y movement
  if (up && down) {
    cameraAction->Y = 0;
  } else if (up) {
    cameraAction->Y = 1;
  } else if (down) {
    cameraAction->Y = -1;
  }
  
  //zoom
  if (zoomI && zoomO) {
    cameraAction->zoom = 0;
  } else if (zoomI) {
    cameraAction->zoom = 1;
  } else if (zoomO) {
    cameraAction->zoom = -1;
  }
} //readCamera

void Keyboard::readCube(const bool *k, int size) {
  bool front = k[SDL_SCANCODE_F];
  bool back = k[SDL_SCANCODE_B];
  bool middle = k[SDL_SCANCODE_M];
  bool slice = k[SDL_SCANCODE_S];
  bool equator = k[SDL_SCANCODE_E];
  bool upper = k[SDL_SCANCODE_U];
  bool right = k[SDL_SCANCODE_R];
  bool left = k[SDL_SCANCODE_L];
  bool down = k[SDL_SCANCODE_D];
  bool random = k[SDL_SCANCODE_LCTRL];
  bool solve = k[SDL_SCANCODE_1];

  bool shift = k[SDL_SCANCODE_LSHIFT];

  Rotation rot = CLOCKWISE;

  if (shift) rot = COUNT_CLOCKWISE;


  if (random) {
    SDL_Log("READ CTRL!!!!");
    CubeAction action;
    action.move = RANDOM;
    cubeAction->push(action);
  }

  if (front) {
    SDL_Log("READ F!!!!");
    CubeAction action;
    action.move = F;
    action.rotate = rot;
    cubeAction->push(action);
  }

  if (down) {
    SDL_Log("READ D!!!!");
    CubeAction action;
    action.move = D;
    action.rotate = rot;
    cubeAction->push(action);
  }

  if (right) {
    SDL_Log("READ R!!!!");
    CubeAction action;
    action.move = R;
    action.rotate = rot;
    cubeAction->push(action);
  }

  if (left) {
    SDL_Log("READ L!!!!");
    CubeAction action;
    action.move = L;
    action.rotate = rot;
    cubeAction->push(action);
  }

  if (equator) {
    SDL_Log("READ E!!!!");
    CubeAction action;
    action.move = E;
    action.rotate = rot;
    cubeAction->push(action);
  }

  if (upper) {
    SDL_Log("READ U!!!!");
    CubeAction action;
    action.move = U;
    action.rotate = rot;
    cubeAction->push(action);
  }

  if (back) {
    SDL_Log("READ B!!!!");
    CubeAction action;
    action.move = B;
    action.rotate = rot;
    cubeAction->push(action);
  }

  if (slice) {
    SDL_Log("READ S!!!!");
    CubeAction action;
    action.move = S;
    action.rotate = rot;
    cubeAction->push(action);
  }
  
  if (middle) {
    SDL_Log("READ M!!!!");
    CubeAction action;
    action.move = M;
    action.rotate = rot;
    cubeAction->push(action);
  }
  if (solve) {
    SDL_Log("READ right cntrl!!!!");
    CubeAction action;
    action.move = SOLVE;
    action.rotate = rot;
    cubeAction->push(action);
  }
} //readCube

void Keyboard::readState(const bool *keyboard, int size) {
  readCamera(keyboard, size);
  readCube(keyboard, size);
} //readState