#include "rcube.h"
#include "SDL3/SDL.h"

int main() {
  RCube* cube = new RCube();
  cube->printCube(std::cout);
  delete cube;
}