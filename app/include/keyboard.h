#ifndef HKEYBOARDH
#define HKEYBOARDH

#include <SDL3/SDL_keyboard.h>
#include <vector>
#include <queue>
#include "../../cube/include/rcube.h"

#include <SDL3/SDL.h>


//main class that retrieves actions from keyboard state
class Keyboard {
  private:
    //pointer to cameraAction container
    CameraAction *cameraAction = nullptr;

    //pointer to cubeAction queue
    std::queue<CubeAction> *cubeAction = nullptr;

    //read camera instructions from keyboard
    void readCamera(const bool *keyboard, int size);

    //read cube instructions from keyboard
    void readCube(const bool *keyboard, int size);

  public:
    //standard constructor
    Keyboard(CameraAction *a, std::queue<CubeAction> *b) : 
      cameraAction(a), cubeAction(b){};

    //reads all actions into its containers from keyboard state, 
    //size is total keys read  
    void readState(const bool *keyboard, int size);

    //default destructor
    ~Keyboard() = default;
};

#endif
