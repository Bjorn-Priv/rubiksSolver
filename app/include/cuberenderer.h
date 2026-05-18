#ifndef HCUBERENH
#define HCUBERENH

#include "rendercubit.h"


//datastructure to hold all data for the animation movement of a side
struct MoveAnimation {
  bool active = false;

  char side;
  Rotation rotation;

  float currentAngle = 0.0f;
  float targetAngle = 90.0f;

  float speed = 270.0f;

  std::vector<int> cubits;
};

//main class of the cube renderer 
//containing both the shader and all cubits and the main logical cube
class CubeRenderer {
  private: 
    //pseudo logic cube to keep track of cubit positions
    RenderCubit cubits[MAX_D][MAX_D][MAX_D];

    //animation stack
    MoveAnimation animation;

    //main logical cube
    RCube *cube = nullptr;
    //cube actions container
    std::vector<CubeAction> *actions = nullptr;
    //main camera
    Camera *camera = nullptr;

    //main shader program to be used
    GLuint shaderProgram;

    //(vertex array object)
    //contains data to know how to reaed the VBO correctly,
    //like the first 3 floats in the VBO are positional
    //then there is 2 floats for texture coordinates etc... 
    //like a recipe to use when reading the VBO for the GPU
    GLuint cubeVAO;
    //(vertex buffer object)
    //contains all raw data like 
    //positions, colours and textures for every vertex
    //and stores it in the GPU
    //it practically holds any per-vertex data
    //but the GPU has no idea how to parse this thing
    //as it needs to know what data is where and cuz its dumb
    //thats why we make VAO
    GLuint cubeVBO;
    //(element buffer object)
    //this is a memory saving buffer that contains information
    //on how to connect vertices to form shapes for the GPU
    //holds an array of integers that act as pointers to the vertices in the VBO
    //and it practically tells the GPU stuff like use vertices 1, 3, and 5 to draw a triangle
    //with an EBO you can now use glDrawElements instead of glDrawArrays
    GLuint cubeEBO;

    float vertices[VERTICESCOUNT*CUBITCOUNT*6];
    unsigned int indices[CUBITCOUNT*12*3];

    //number of elements to be drawn
    int indexCount = 36;

    //global variable locations in shader program
    GLint modelLoc;
    GLint viewLoc;
    GLint projLoc;

  public: 
    //default constructor
    CubeRenderer(RCube *c, std::vector<CubeAction> *a, Camera *cam, GLuint shader);

    //default destructor
    ~CubeRenderer();

    //initialise the mesh 
    void initMesh();

    //update the animation
    void update(float deltaTime){};
    //render the cube
    void render();
    //initiate an animation
    void startMove(char side, Rotation direction){};
};
#endif