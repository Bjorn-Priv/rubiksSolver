#ifndef HCUBERENH
#define HCUBERENH

#include "rendercubit.h"
#include <queue>

//datastructure to hold all data for the animation movement of a side
struct MoveAnimation {
  //rotation direction
  Rotation rotation; 

  float currentAngle = 0.0f; 

  float targetAngle = 0.0f;

  //center around which to rotate
  glm::vec3 center = {0.0f, 0.0f, 0.0f}; 

   //axis to rotate 
  glm::vec3 axis = {0.0f, 0.1f, 0.0f};

  //which move to do
  MoveID move; 

  //speed of animation
  float speed = 5.0f; 

  //if its part of a random shuffle
  bool random = false;

  //vector of cubits that need to rotate
  std::vector<RenderCubit*> cubits; 
};

//main class of the cube renderer 
//containing both the shader and all cubits and the main logical cube
class CubeRenderer {
  private: 
    //pseudo logic cube to keep track of cubit positions
    RenderCubit cubits[MAX_D][MAX_D][MAX_D];

    //pseudo logic cube with pointers to keep track of cubit positions
    RenderCubit *cubitsP[MAX_D][MAX_D][MAX_D];

    //animation queue
    std::queue<MoveAnimation> animation;

    //main logical cube that will solve the cube
    RCube *cube = nullptr;

    //cube actions container
    std::queue<CubeAction> *actions = nullptr;

    //main camera
    Camera *camera = nullptr;

    //main shader program to be used
    GLuint shaderProgram;

    //(vertex array object)
    //contains data to know how to read the VBO correctly,
    GLuint cubeVAO;

    //(vertex buffer object)
    //contains all raw data
    GLuint cubeVBO;
    
    //(element buffer object)
    //this is a memory saving buffer that contains information
    //on how to connect vertices to form shapes for the GPU
    GLuint cubeEBO;

    //vertices buffer
    float vertices[NVERTICES_PER_CUBIT*NCUBITS*NFLOATS_PER_VERTEX];

    //vertices index buffer for triangles
    unsigned int indices[NCUBITS*NTRIANGLES_PER_CUBIT*NINTS_PER_TRIANGLE];

    //number of elements to be drawn
    int indexCount = NTRIANGLES_PER_CUBIT*NINTS_PER_TRIANGLE;

    //global variable locations in shader program
    GLint viewLoc, projLoc;

    //amount of moves to shuffle cube with if random
    int N_Random = 25;

    //wrapper function to rotate cubits pointers using the front most move in the action queue
    //should be called at the end of an animation so that logically everything stays aligned
    void doPointerMove();

    //helper function to perform the rotations R, L and M
    //pass the function the direction of the rotation and on which X axis the rotation should take place
    //this function does the actual rotating for doPointerMove
    //R -> x = 2
    //L -> x = 0
    //M -> x = 1
    void RLM(Rotation, int x);

    //helper function to perform the rotations U, D and E
    //pass the function the direction of the rotation and on which Y axis the rotation should take place
    //this function does the actual rotating for doPointerMove
    //U -> y = 2
    //D -> y = 0
    //E -> y = 1
    void UDE(Rotation, int y);

    //helper function to perform the rotations F, B and S
    //pass the function the direction of the rotation and on which Z axis the rotation should take place
    //this function does the actual rotating for doPointerMove
    //F -> z = 0
    //B -> z = 2
    //S -> z = 1
    void FBS(Rotation, int z);

    //wrapper functions to call RLM/UDE/FBS with the correct parameters for every move
    //pass the direction of the rotation to rotate the cubit pointers in the correct direction
    void R_Rotate(Rotation), L_Rotate(Rotation), M_Rotate(Rotation),
         U_Rotate(Rotation), D_Rotate(Rotation), E_Rotate(Rotation), 
         F_Rotate(Rotation), B_Rotate(Rotation), S_Rotate(Rotation);

    //helper function to retrieve the correct axis around which to rotate every move
    //pass the move identity and then returns a vector with the correct axis
    //RLM -> x-axis -> {1.0f, 0.0f, 0.0f}
    //UDE -> y-axis -> {0.0f, 1.0f, 0.0f}
    //FBS -> z-axis -> {0.0f, 0.0f, 1.0f}
    glm::vec3 retrieveAxis(MoveID move);

    //wrapper function to extract the vector of correct RenderCubits to rotate
    //and the centroid of the center most cubit to rotate around
    //pass the move identity and a pointer to a vector to fill 
    glm::vec3 retrieveCubits_Center(MoveID move, std::vector<RenderCubit*> *vec);

    //helper functions to retrieve the correct pointers for every move at the correct axis index 
    //pass a pointer to a vector to fill and the correct axis level
    glm::vec3 retrieveXsurface(std::vector<RenderCubit*> *vec, int x), 
              retrieveYsurface(std::vector<RenderCubit*> *vec, int y),
              retrieveZsurface(std::vector<RenderCubit*> *vec, int z);

    //updates all centroids for all cubits at each pointer
    void updateCentroids();

    //generates random moves
    void handleRandom();

    //initialise the mesh by generating buffers and attributes
    void initMesh();

  public: 
    //default constructor
    CubeRenderer(RCube *c, std::queue<CubeAction> *a, Camera *cam, GLuint shader);

    //default destructor
    ~CubeRenderer();

    //update the animation with deltatime 
    void update(float deltaTime);

    //render all the cubits
    void render();

    //sets amount of moves to use when shuffling cube
    void setRandomNum(int N){N_Random = N;};

    //initiate an animation 
    //by grabbing an action from the queue
    void startMove();
};
#endif