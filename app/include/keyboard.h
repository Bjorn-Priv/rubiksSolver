#ifndef HKEYBOARDH
#define HKEYBOARDH

#include <SDL3/SDL_keyboard.h>
#include <vector>
#include <queue>
#include "../../cube/include/rcube.h"

#include <SDL3/SDL.h>

struct CameraAction {
  int X = 0;
  int Y = 0;
  int zoom = 0;
  int rotate = 0;

  bool isEmpty() {return X == 0 && Y == 0 && zoom == 0 && rotate == 0;}
};

struct CubeAction {
  int move;
  Rotation rotate;
};

//main class that retrieves actions from keyboard state
class Keyboard {
  private:
    CameraAction *cameraAction = nullptr;
    std::queue<CubeAction> *cubeAction = nullptr;

    void readCamera(const bool *keyboard, int size);
    void readCube(const bool *keyboard, int size);

  public:
    //standard constructor
    Keyboard(CameraAction *a, std::queue<CubeAction> *b) : 
      cameraAction(a), cubeAction(b){};

    //reads all actions into its containers from keyboard state
    void readState(const bool *keyboard, int size);

    //default destructor
    ~Keyboard() = default;
};

#endif
