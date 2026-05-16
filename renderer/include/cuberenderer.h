#ifndef HCUBERENH
#define HCUBERENH

#include "../../cube/include/rcube.h"

#include <vector>

#include <GL/glew.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_opengl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct RenderCubit {
  //identity matrix
  glm::mat4 orientation = glm::mat4(1.0f);
};

struct MoveAnimation {
  bool active = false;

  char side;
  Rotation rotation;

  float currentAngle = 0.0f;
  float targetAngle = 90.0f;

  float speed = 270.0f;

  std::vector<int> cubits;
};

class CubeRenderer {
  private: 
    RenderCubit cubits[MAX_D][MAX_D][MAX_D];
    
    MoveAnimation animation;

    RCube *cube = nullptr;

    GLuint shaderProgram;

    GLuint cubeVAO;
    GLuint cubeVBO;
    GLuint cubeEBO;

    int indexCount = 36;

    GLint modelLoc;
    GLint viewLoc;
    GLint projLoc;

    glm::mat4 view;
    glm::mat4 projection;

  public: 
    CubeRenderer(RCube *c, GLuint shader);

    void initMesh();

    void update(float deltaTime){};
    void render();
    void startMove(char side, Rotation direction){};
};
#endif