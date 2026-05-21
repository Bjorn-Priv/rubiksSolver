#ifndef HKEYBOARDH
#define HKEYBOARDH

#include <SDL3/SDL_keyboard.h>
#include <vector>
#include <queue>
#include "../../cube/include/rcube.h"

#include <SDL3/SDL.h>

//main struct to store camera actions
struct CameraAction {
  int X = 0; //left or right of cube
  int Y = 0; //over or under cube
  int zoom = 0; //zoom in or out to cube

  //checks if cameraaction needs updating
  bool isEmpty() {return X == 0 && Y == 0 && zoom == 0;}
};

//main struct to store cube actions
struct CubeAction {
  MoveID move; 
  Rotation rotate; 
};

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
