#include "include/keyboard.h"

void Keyboard::readCamera(const bool *keyboard, int size) {
  if (size < 226) return;

  cameraAction->X = 0;
  cameraAction->Y = 0;
  cameraAction->zoom = 0;

  //X movement
  if (keyboard[SDL_SCANCODE_RIGHT] && keyboard[SDL_SCANCODE_LEFT]) {
    cameraAction->X = 0;
  } else if (keyboard[SDL_SCANCODE_RIGHT]) {
    cameraAction->X = 1;
  } else if (keyboard[SDL_SCANCODE_LEFT]) {
    cameraAction->X = -1;
  }

  //Y movement
  if (keyboard[SDL_SCANCODE_UP] && keyboard[SDL_SCANCODE_DOWN]) {
    cameraAction->Y = 0;
  } else if (keyboard[SDL_SCANCODE_UP]) {
    cameraAction->Y = 1;
  } else if (keyboard[SDL_SCANCODE_DOWN]) {
    cameraAction->Y = -1;
  }

  bool zoomIn = (keyboard[SDL_SCANCODE_LSHIFT] && keyboard[SDL_SCANCODE_EQUALS]) ||
                keyboard[SDL_SCANCODE_KP_PLUS];
  bool zoomOut = (keyboard[SDL_SCANCODE_LSHIFT] && keyboard[SDL_SCANCODE_MINUS]) || 
                  keyboard[SDL_SCANCODE_KP_MINUS];
  
  //zoom
  if (zoomIn && zoomOut) {
    cameraAction->zoom = 0;
  } else if (zoomIn) {
    cameraAction->zoom = 1;
  } else if (zoomOut) {
    cameraAction->zoom = -1;
  }
} //readCamera

void Keyboard::readState(const bool *keyboard, int size) {
  readCamera(keyboard, size);
  //SDL_Log("X: %i --- Y: %i ---- Zoom: %i", cameraAction->X, cameraAction->Y, cameraAction->zoom);
}