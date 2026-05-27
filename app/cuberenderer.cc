#include "include/cuberenderer.h"
#include <vector>

void CubeRenderer::updateCentroids() {
  float X[3] = {1.0f, 0.0f, -1.0f};
  float Y[3] = {-1.0f, 0.0f, 1.0f};
  float Z[3] = {-1.0f, 0.0f, 1.0f};

  for (size_t z = 0; z < MAX_D; z++) { //for all z
    for (size_t y = 0; y < MAX_D; y++) { //for all y
      for (size_t x = 0; x < MAX_D; x++) { //for all x
        cubitsP[x][y][z]->setCentroid(glm::vec3(X[x], Y[y], Z[z]));
      } //for
    } //for
  } //for
} //updateCentroids

CubeRenderer::CubeRenderer(RCube *c, std::queue<CubeAction> *a, Camera *cam, GLuint shader) : cube(c), actions(a), camera(cam) {
  shaderProgram = shader;

  float X[3] = {1.0f, 0.0f, -1.0f};
  float Y[3] = {-1.0f, 0.0f, 1.0f};
  float Z[3] = {-1.0f, 0.0f, 1.0f};

  unsigned int *tempIndex;
  int Voffset = 0;
  int Ioffset = 0;

  for (size_t z = 0; z < MAX_D; z++) { //for all z
    for (size_t y = 0; y < MAX_D; y++) { //for all y
      for (size_t x = 0; x < MAX_D; x++) { //for all x
        cubitsP[x][y][z] = &cubits[x][y][z]; //grab pointer

        //set data
        cubits[x][y][z].setVertices(&vertices[Voffset]);
        cubits[x][y][z].setCentroid(glm::vec3(X[x], Y[y], Z[z]));
        cubits[x][y][z].setCubitData(cube->getCubit(x, y, z));

        //init cubit render object
        tempIndex = cubits[x][y][z].initCubit();

        unsigned int vertexBase = Voffset / NFLOATS_PER_VERTEX;

        int max = NTRIANGLES_PER_CUBIT * NINTS_PER_TRIANGLE;
        for (int i = 0; i < max; i++) {
          indices[i+Ioffset] = tempIndex[i]+vertexBase;
        } //for

        cubits[x][y][z].setIndexOffset(Ioffset);
        Ioffset += max;
        Voffset += (NVERTICES_PER_CUBIT*6);
      } //for
    } //for
  } //for

  //grab location of all uniform variables in the shader program
  //uniform variables are practically global variables that you can change
  viewLoc = glGetUniformLocation(shaderProgram, "view");
  projLoc = glGetUniformLocation(shaderProgram, "projection");

  initMesh();
} //default constructor

void CubeRenderer::initMesh() {
  //generate VAO, VBO and EBO
  glGenVertexArrays(1, &cubeVAO);
  glGenBuffers(1, &cubeVBO);
  glGenBuffers(1, &cubeEBO);

  //bind all future vertex array function calls to this VAO
  glBindVertexArray(cubeVAO);
  //bind all future buffer function calls related to ELEMENT_ARRAY_BUFFER to EBO
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
  //bind all future buffer function calls related to ARRAY_BUFFER to VBO
  glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);

  //put data in ARRAY BUFFER and ELEMENT ARRAY BUFFER 
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

  //tell gpu that first thruple is an attribute linked to location 0
  glVertexAttribPointer(
    0, 3, GL_FLOAT, GL_FALSE,
    6 * sizeof(float),
    (void*)0
  );

  //allow the use of the VBO during rendering
  //pass the attribute location value as specified in the shader program
  glEnableVertexAttribArray(0);

  //tell gpu that second thruple is an attribute linked to location 1
  glVertexAttribPointer(
    1, 3, GL_FLOAT, GL_FALSE,
    6 * sizeof(float),
    (void*)(3 * sizeof(float))
  );

  //allow use of VBO and pass location
  glEnableVertexAttribArray(1);

  //unbind the VAO so we can't accidentally change it later
  glBindVertexArray(0);
} //initMesh

void CubeRenderer::render() {
  //define which shader program to use
  glUseProgram(shaderProgram);

  glDisable(GL_CULL_FACE);

  //set 2 global varaiables
  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &(camera->getView()[0][0]));
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, &(camera->getProjection()[0][0]));

  //set used vertex array to VAO
  glBindVertexArray(cubeVAO);
  
  for (size_t z = 0; z < MAX_D; z++) { //for all z
    for (size_t y = 0; y < MAX_D; y++) { //for all y
      for (size_t x = 0; x < MAX_D; x++) { //for all x
        RenderCubit& c = cubits[x][y][z]; //grab cubit
        //draw cubit
        glDrawElements(
          GL_TRIANGLES,
          indexCount,
          GL_UNSIGNED_INT,
          (void*)(c.getIndexOffset() * sizeof(unsigned int))
        );
      } //for
    } //for
  } //for
  //unbind VAO
  glBindVertexArray(0);
} //render

void CubeRenderer::update(float deltatime) {
  if (animation.empty()) return;

  if (animation.front().cubits.empty()) { //if no cubits yet
    animation.front().center = retrieveCubits_Center(animation.front().move, &animation.front().cubits);
  }

  //if its not random and there are lots of animations increase speed
  float sizeHandler = 1.0f;
  if (!animation.front().random) {
    sizeHandler = 1.0f + animation.size()/3;
  }

  //calculate angle that needs to be traveled
  float angle = animation.front().speed * sizeHandler * deltatime;

  //animation finished
  bool done = false;
  
  //if target angle will be reached or overshot
  if (std::abs(animation.front().targetAngle) <= std::abs(animation.front().currentAngle + angle)) {
    angle = animation.front().targetAngle - animation.front().currentAngle;
    done = true;
  }

  //all data from animation to keep it efficient
  std::vector<RenderCubit*> *cubits = &animation.front().cubits;
  glm::vec3 center = animation.front().center;
  glm::vec3 axis = animation.front().axis;

  //update loop
  for (size_t i = 0; i < cubits->size(); i++) { //for all cubits
    (*cubits)[i]->update(angle, center, axis);
  } //for

  animation.front().currentAngle += angle; //update angle

  //bin VBO
  glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);

  //reload vertices data that has been updated
  glBufferSubData(
    GL_ARRAY_BUFFER,
    0,
    sizeof(vertices),
    vertices
  );

  if (done) { //do all logical moves if animation is done
    cube->performMove((MoveID)animation.front().move, animation.front().rotation);
    doPointerMove();
    updateCentroids();
    animation.pop();
  }
} //update

void CubeRenderer::handleRandom() {
  //generate N_Random random numbers
  std::vector<int> moves;
  std::vector<int> directions;
  GenRandomVector(N_Random, &moves, 8, 0);
  GenRandomVector(N_Random, &directions, 3, 1);

  //random move
  MoveID move;
  //random direction
  Rotation direction;

  //new animation
  MoveAnimation newMove; 
  //faster animation for shuffle
  newMove.speed = 30.0f;
  //direction with which to multiply angle
  float direcMultiply;
  for (int i = 0; i < N_Random; i++) { //for all random moves
    move = (MoveID)moves[i];
    direction = (Rotation)directions[i];

    if (direction == CLOCKWISE) {
      direcMultiply = 1.0f;
    } else if (direction == HALF_CIRCLE) {
      direcMultiply = 2.0f;
    } else { //inverse speed for opposite direction
      newMove.speed = -newMove.speed;
      direcMultiply = -1.0f;
    }
    //set all animation values
    newMove.axis = retrieveAxis(move);
    newMove.rotation = direction; 
    newMove.move = move;
    newMove.targetAngle = direcMultiply * glm::half_pi<float>();
    newMove.currentAngle = 0.0f;
    newMove.random = true;
    
    animation.push(newMove);
  } //for
} //handleRandom

void CubeRenderer::startMove() {
  if (actions->empty()) return;

  MoveID move = actions->front().move;

  if (move == RANDOM) {
    handleRandom();
    actions->pop();
    return;
  }

  Rotation direction = actions->front().rotate;
  
  //new animation
  MoveAnimation newMove; 

  //direction to multiply for correct angle
  float direcMultiply;
  actions->pop();

  if (direction == CLOCKWISE) {
    direcMultiply = 1.0f;
  } else if (direction == HALF_CIRCLE) {
    direcMultiply = 2.0f;
  } else { //inverse speed for opposite direction
    newMove.speed = -newMove.speed;
    direcMultiply = -1.0f;
  }
  //set all animation values
  newMove.axis = retrieveAxis(move);
  newMove.rotation = direction; 
  newMove.move = move;
  newMove.targetAngle = direcMultiply * glm::half_pi<float>();
  newMove.currentAngle = 0.0f;
  
  animation.push(newMove);
} //startMove

glm::vec3 CubeRenderer::retrieveAxis(MoveID move) {
  switch (move) {
  case R: case L: case M:
    return {1.0f, 0.0f, 0.0f};
  case U: case D: case E:
    return {0.0f, 1.0f, 0.0f}; 
  case F: case B: case S:
    return {0.0f, 0.0f, 1.0f};
  default : return {};
  } //switch
} //retrieveAxis

glm::vec3 CubeRenderer::retrieveCubits_Center(MoveID move, std::vector<RenderCubit*> *vec) {
  switch (move) {
  case R: return retrieveXsurface(vec, 2);
  case L: return retrieveXsurface(vec, 0);
  case U: return retrieveYsurface(vec, 2);
  case D: return retrieveYsurface(vec, 0);
  case F: return retrieveZsurface(vec, 0);
  case B: return retrieveZsurface(vec, 2);
  case M: return retrieveXsurface(vec, 1);
  case E: return retrieveYsurface(vec, 1);
  case S: return retrieveZsurface(vec, 1);
  default : return {};
  } //switch
} //retrieveCubits_Center

glm::vec3 CubeRenderer::retrieveXsurface(std::vector<RenderCubit*> *vec, int x) {
  glm::vec3 center;
  for (size_t i = 0; i < MAX_D; i++) { //for all y
    for (size_t j = 0; j < MAX_D; j++) { //for all z
      vec->push_back(cubitsP[x][i][j]);
      if (i == 1 && j == 1) center = cubitsP[x][i][j]->getCentroid();
    } //for
  } //for
  return center;
} //retrieveXsurface

glm::vec3 CubeRenderer::retrieveYsurface(std::vector<RenderCubit*> *vec, int y) {
  glm::vec3 center;
  for (size_t i = 0; i < MAX_D; i++) { //for all x
    for (size_t j = 0; j < MAX_D; j++) { // for all z
      vec->push_back(cubitsP[i][y][j]);
      if (i == 1 && j == 1) center = cubitsP[i][y][j]->getCentroid();
    } //for
  } //for
  return center;
} //retrieveYsurface

glm::vec3 CubeRenderer::retrieveZsurface(std::vector<RenderCubit*> *vec, int z) {
  glm::vec3 center;
  for (size_t i = 0; i < MAX_D; i++) { //for all x
    for (size_t j = 0; j < MAX_D; j++) { //for all y
      vec->push_back(cubitsP[i][j][z]);
      if (i == 1 && j == 1) center = cubitsP[i][j][z]->getCentroid();
    } //for
  } //for
  return center;
} //retrieveZsurface

void CubeRenderer::doPointerMove() {
  switch (animation.front().move) {
    case R: R_Rotate(animation.front().rotation); break;
    case L: L_Rotate(animation.front().rotation); break;
    case U: U_Rotate(animation.front().rotation); break;
    case D: D_Rotate(animation.front().rotation); break;
    case F: F_Rotate(animation.front().rotation); break;
    case B: B_Rotate(animation.front().rotation); break;
    case M: M_Rotate(animation.front().rotation); break;
    case E: E_Rotate(animation.front().rotation); break;
    case S: S_Rotate(animation.front().rotation); break;
    default: break;
  } //switch
} //doPointerMove

void CubeRenderer::RLM(Rotation rot, int x) {
  for (uint8 i = 0; i < rot; i++) { //for correct direction
    //move corners
    RenderCubit *temp = cubitsP[x][2][2]; //old back top
    cubitsP[x][2][2] = cubitsP[x][2][0]; //new back top is old front top
    cubitsP[x][2][0] = cubitsP[x][0][0]; //new front top is old front bottom
    cubitsP[x][0][0] = cubitsP[x][0][2]; //new front bottom is old back bottom
    cubitsP[x][0][2] = temp; //new back bottom is old back top

    //move edges
    temp = cubitsP[x][2][1]; //old top middle
    cubitsP[x][2][1] = cubitsP[x][1][0]; //new top middle becomes old front middle
    cubitsP[x][1][0] = cubitsP[x][0][1]; //new front middle becomes old bottom middle
    cubitsP[x][0][1] = cubitsP[x][1][2]; //new bottom middle becomes old back middle
    cubitsP[x][1][2] = temp; //new back middle becomes old top middle
  } //for
} //RLM

void CubeRenderer::R_Rotate(Rotation rot) {
  RLM(rot, 2);
} //R_Rotate

void CubeRenderer::L_Rotate(Rotation rot) {
  RLM(rot, 0);
} //L_Rotate

void CubeRenderer::M_Rotate(Rotation rot) {
  RLM(rot, 1);
} //M_Rotate

void CubeRenderer::UDE(Rotation rot, int y) {
  for (uint8 i = 0; i < rot; i++) { //for correct direction
    //move corners
    RenderCubit *temp = cubitsP[0][y][0]; //temp is old front left
    cubitsP[0][y][0] = cubitsP[0][y][2]; //new front left is old back left
    cubitsP[0][y][2] = cubitsP[2][y][2]; //new back left is old back right
    cubitsP[2][y][2] = cubitsP[2][y][0]; //new back right is old front right
    cubitsP[2][y][0] = temp; //new front 

    //move edges
    temp = cubitsP[1][y][0];
    cubitsP[1][y][0] = cubitsP[0][y][1];
    cubitsP[0][y][1] = cubitsP[1][y][2];
    cubitsP[1][y][2] = cubitsP[2][y][1];
    cubitsP[2][y][1] = temp;
  } //for
} //UDE

void CubeRenderer::U_Rotate(Rotation rot) {
  UDE(rot, 2);
} //U_Rotate

void CubeRenderer::D_Rotate(Rotation rot) {
  UDE(rot, 0);
} //D_Rotate

void CubeRenderer::E_Rotate(Rotation rot) {
  UDE(rot, 1);
} //E_Rotate

void CubeRenderer::FBS(Rotation rot, int z) {
  for (uint8 i = 0; i < rot; i++) { //for correct direction
    //move corners
    RenderCubit *temp = cubitsP[0][2][z]; //old top left
    cubitsP[0][2][z] = cubitsP[0][0][z]; //new top left becomes old bottom left
    cubitsP[0][0][z] = cubitsP[2][0][z]; //new bottom left becomes old bottom right
    cubitsP[2][0][z] = cubitsP[2][2][z]; //new bottom right becomes old top right
    cubitsP[2][2][z] = temp; //new top right becomes old top left

    //move edges
    temp = cubitsP[1][2][z]; //old top middle
    cubitsP[1][2][z] = cubitsP[0][1][z]; //new top middle becomes old left middle
    cubitsP[0][1][z] = cubitsP[1][0][z]; //new left middle becomes old bottom middle
    cubitsP[1][0][z] = cubitsP[2][1][z]; //new bottom middle becomes old right middle
    cubitsP[2][1][z] = temp; //new right middle becomes old top middle
  } //for
} //FBS

void CubeRenderer::B_Rotate(Rotation rot) {
  FBS(rot, 2);
} //B_Rotate

void CubeRenderer::F_Rotate(Rotation rot) {
  FBS(rot, 0);
} //F_Rotate

void CubeRenderer::S_Rotate(Rotation rot) {
  FBS(rot, 1);
} //S_Rotate

CubeRenderer::~CubeRenderer() {
  //destroy all gl buffers and arrays
  glDeleteBuffers(1, &cubeVBO);
  glDeleteBuffers(1, &cubeEBO);
  glDeleteVertexArrays(1, &cubeVAO);
} //default destructor