#include "include/keyboard.h"

void Keyboard::readCamera(const bool *keyboard, int size) {
  if (size < 226) return;

  bool right = keyboard[SDL_SCANCODE_RIGHT];
  bool left = keyboard[SDL_SCANCODE_LEFT];
  bool up = keyboard[SDL_SCANCODE_UP];
  bool down = keyboard[SDL_SCANCODE_DOWN];

  bool clockwise = keyboard[SDL_SCANCODE_E];
  bool counterClock = keyboard[SDL_SCANCODE_Q];

  bool zoomI = (keyboard[SDL_SCANCODE_LSHIFT] && keyboard[SDL_SCANCODE_EQUALS]) ||
                keyboard[SDL_SCANCODE_KP_PLUS];
  bool zoomO = (keyboard[SDL_SCANCODE_LSHIFT] && keyboard[SDL_SCANCODE_MINUS]) || 
                keyboard[SDL_SCANCODE_KP_MINUS];

  cameraAction->X = 0;
  cameraAction->Y = 0;
  cameraAction->zoom = 0;
  cameraAction->rotate = 0;

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

  //rotation
  if (clockwise && counterClock) {
    cameraAction->rotate = 0;
  } else if (clockwise) {
    cameraAction->rotate = 1;
  } else if (counterClock) {
    cameraAction->rotate = -1;
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

void Keyboard::readState(const bool *keyboard, int size) {
  readCamera(keyboard, size);
  //SDL_Log("X: %i --- Y: %i ---- Zoom: %i ----- rotate: %i", cameraAction->X, cameraAction->Y, cameraAction->zoom, cameraAction->rotate);
}