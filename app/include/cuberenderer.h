#ifndef HCUBERENH
#define HCUBERENH

#include "rendercubit.h"
#include <queue>

//datastructure to hold all data for the animation movement of a side
struct MoveAnimation {
  Rotation rotation;

  float currentAngle = 0.0f;
  float targetAngle = 0.0f;

  glm::vec3 center = {0.0f, 0.0f, 0.0f};
  glm::vec3 axis = {0.0f, 0.1f, 0.0f};

  int move = 0;

  float speed = 0.0f;

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

    //animation stack
    std::queue<MoveAnimation> animation;

    //main logical cube
    RCube *cube = nullptr;
    //cube actions container
    std::queue<CubeAction> *actions = nullptr;
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

    float vertices[NVERTICES_PER_CUBIT*NCUBITS*NFLOATS_PER_VERTEX];
    unsigned int indices[NCUBITS*NTRIANGLES_PER_CUBIT*NINTS_PER_TRIANGLE];

    //number of elements to be drawn
    int indexCount = NTRIANGLES_PER_CUBIT*NINTS_PER_TRIANGLE;

    //global variable locations in shader program
    GLint modelLoc;
    GLint viewLoc;
    GLint projLoc;

    void doPointerMove();

    void RLM(Rotation, int x);
    void UDE(Rotation, int y);
    void FBS(Rotation, int z);

    void R_Rotate(Rotation);
    void L_Rotate(Rotation);
    void U_Rotate(Rotation);
    void D_Rotate(Rotation);
    void F_Rotate(Rotation);
    void B_Rotate(Rotation);

    void M_Rotate(Rotation);
    void E_Rotate(Rotation);
    void S_Rotate(Rotation);

    glm::vec3 retrieveAxis(int move);
    glm::vec3 retrieveCubits_Center(int move, std::vector<RenderCubit*> *vec);

    glm::vec3 retrieveXsurface(std::vector<RenderCubit*> *vec, int x);
    glm::vec3 retrieveYsurface(std::vector<RenderCubit*> *vec, int y);
    glm::vec3 retrieveZsurface(std::vector<RenderCubit*> *vec, int z);

  public: 
    //default constructor
    CubeRenderer(RCube *c, std::queue<CubeAction> *a, Camera *cam, GLuint shader);

    //default destructor
    ~CubeRenderer();

    //initialise the mesh 
    void initMesh();

    //update the animation
    void update(float deltaTime);
    //render the cube
    void render();
    //initiate an animation
    void startMove();
};
#endif