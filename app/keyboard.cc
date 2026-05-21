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
  bool middle = k[SDL_SCANCODE_M];

  if (front) {
    SDL_Log("READ F!!!!");
    CubeAction action;
    action.move = F;
    action.rotate = CLOCKWISE;
    cubeAction->push(action);
  }
  
  if (middle) {
    SDL_Log("READ F!!!!");
    CubeAction action;
    action.move = M;
    action.rotate = CLOCKWISE;
    cubeAction->push(action);
  }
} //readCube

void Keyboard::readState(const bool *keyboard, int size) {
  readCamera(keyboard, size);
  readCube(keyboard, size);
} //readState