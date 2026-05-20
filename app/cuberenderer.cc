#include "include/cuberenderer.h"
#include <vector>

CubeRenderer::CubeRenderer(RCube *c, std::queue<CubeAction> *a, Camera *cam, GLuint shader) : cube(c), actions(a), camera(cam) {
  shaderProgram = shader;

  float X[3] = {-1.0f, 0.0f, 1.0f};
  float Y[3] = {-1.0f, 0.0f, 1.0f};
  float Z[3] = {-1.0f, 0.0f, 1.0f};

  unsigned int *tempIndex;
  int Voffset = 0;
  int Ioffset = 0;
  for (size_t z = 0; z < MAX_D; z++) {
    for (size_t y = 0; y < MAX_D; y++) {
      for (size_t x = 0; x < MAX_D; x++) {
        cubitsP[x][y][z] = &cubits[x][y][z];

        SDL_Log("with x, y, z : %f, %f, %f", X[x], Y[y], Z[z]);
        cubits[x][y][z].setVertices(&vertices[Voffset]);
        cubits[x][y][z].setCentroid(glm::vec3(X[x], Y[y], Z[z]));
        cubits[x][y][z].setCubitData(cube->getCubit(x, y, z));
        tempIndex = cubits[x][y][z].initCubit();

        unsigned int vertexBase = Voffset / 6;
        for (int i = 0; i < 36; i++) {
          indices[i+Ioffset] = tempIndex[i]+vertexBase;
        }
        cubits[x][y][z].indexOffset = Ioffset;
        Ioffset += 36;
        Voffset += (NVERTICES_PER_CUBIT*6);
      }
    }
  }

  // unsigned int indices[] = {
  //   0,1,2, 2,3,1,
  //   4,5,6, 6,7,5,
  //   8,9,10, 10,11,9,
  //   12,13,14, 14,15,13,
  //   16,17,18, 18,19,16,
  //   20,21,22, 22,23,20
  // };

  //grab location of all uniform variables in the shader program
  //uniform variables are practically global variables that you can change
  modelLoc = glGetUniformLocation(shaderProgram, "model");
  viewLoc = glGetUniformLocation(shaderProgram, "view");
  projLoc = glGetUniformLocation(shaderProgram, "projection");

  initMesh();
} //default constructor

void CubeRenderer::initMesh() {
  
  // for (size_t i = 0; i < vertices.size(); i+=6) {
  //   SDL_Log("coords: %f, %f, %f, colour: %f, %f, %f", vertices[i], vertices[i+1], vertices[i+2], vertices[i+3], vertices[i+4], vertices[i+5]);
  // }

  //generate VAO, VBO and EBO
  glGenVertexArrays(1, &cubeVAO);
  glGenBuffers(1, &cubeVBO);
  glGenBuffers(1, &cubeEBO);

  //bind all future vertex array function calls to this VAO
  //bind all future buffer function calls related to ELEMENT_ARRAY_BUFFER to EBO
  //bind all future buffer function calls related to ARRAY_BUFFER to VBO
  glBindVertexArray(cubeVAO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
  glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);

  //put data in ARRAY BUFFER and ELEMENT ARRAY BUFFER 
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

  //what data means what in the VBO is specified here
  //basically in the shader program we created a location = 0 (we can create multiple)
  //these locations contain data for every attribute of the vertices
  //in this case we only have position so we only need one
  //arguments: 
  //index : the location specified in you .vert shader program
  //size : number of data points per vertex attribute 
  //type : datatype of the array elements
  //normalized : basically if you want fixed point values or just take exact values
    //GL_TRUE = fixed
    //GL_FALSE = raw data
  //stride : byte offset between consecutive attributes
  //offset : where to start reading in the array
  glVertexAttribPointer(
    0, 3, GL_FLOAT, GL_FALSE,
    6 * sizeof(float),
    (void*)0
  );
  //allow the use of the VBO during rendering
  //pass the attribute location value as specified in the shader program
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(
    1, 3, GL_FLOAT, GL_FALSE,
    6 * sizeof(float),
    (void*)(3 * sizeof(float))
  );

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

  //flip x axis cuz GL is stupid and has positive x on the left 
  glm::mat4 global(1.0f);
  global = glm::scale(global, glm::vec3(-1.0f, 1.0f, 1.0f));

  for (size_t z = 0; z < MAX_D; z++) {
    for (size_t y = 0; y < MAX_D; y++) {
      for (size_t x = 0; x < MAX_D; x++) {
        RenderCubit& c = cubits[x][y][z]; //grab cubit

        //set cubit rotation
        glm::mat4 model = global * c.orientation;

        //set model in shader program
        glUniformMatrix4fv(
          modelLoc,
          1,
          GL_FALSE,
          &model[0][0]
        );

        //draw cubit
        glDrawElements(
          GL_TRIANGLES,
          indexCount,
          GL_UNSIGNED_INT,
          (void*)(c.indexOffset * sizeof(unsigned int))
        );
      }
    }
  }
  //unbind VAO
  glBindVertexArray(0);
}

void CubeRenderer::update(float deltatime) {
  SDL_Log("IN UPDATE");
  if (animation.empty()) {SDL_Log("ANIMATION EMPTY"); return;}

  if (animation.front().cubits.empty()){
    animation.front().center = retrieveCubits_Center(animation.front().move, &animation.front().cubits);
  }
  float angle = animation.front().speed * deltatime;
  bool done = false;
  
  if (animation.front().targetAngle <= animation.front().currentAngle + angle) {
    angle = animation.front().targetAngle - animation.front().currentAngle;
    done = true;
  }

  for (auto i : animation.front().cubits) {
    i->update(angle, animation.front().center, animation.front().axis);
  }
  animation.front().currentAngle += angle;

  glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
  
  glBufferSubData(
    GL_ARRAY_BUFFER,
    0,
    sizeof(vertices),
    vertices
  );

  if (done) {
    cube->performMove(animation.front().move, animation.front().rotation);
    doPointerMove();
    animation.pop();
  }
}

void CubeRenderer::startMove() {
  int move = actions->front().move;
  Rotation direction = actions->front().rotate;
  SDL_Log("Move: %i, Rotation: %i", move, (int) direction);
  MoveAnimation newMove;
  int direcMultiply;
  actions->pop();

  if (direction == CLOCKWISE) {
    direcMultiply = 1;
  } else if (direction == HALF_CIRCLE) {
    direcMultiply = 2;
  } else {
    direcMultiply = -1;
  }
  newMove.axis = retrieveAxis(move);

  newMove.rotation = direction;
  newMove.speed = 3.0f;
  newMove.move = move;
  newMove.targetAngle = direcMultiply * glm::half_pi<float>();
  newMove.currentAngle = 0.0f;
  
  animation.push(newMove);
} //startMove

glm::vec3 CubeRenderer::retrieveAxis(int move) {
  switch (move) {
  case 0: case 1: case 6:
    return {1.0f, 0.0f, 0.0f};
  case 2: case 3: case 7:
    return {0.0f, 1.0f, 0.0f}; 
  case 4: case 5: case 8:
    return {0.0f, 0.0f, 1.0f}; 
  default: 
    return {0.0f, 1.0f, 0.0f};
  }
} //retrieveAxis

glm::vec3 CubeRenderer::retrieveCubits_Center(int move, std::vector<RenderCubit*> *vec) {
  switch (move) {
  case 0: return retrieveXsurface(vec, 2);
  case 1: return retrieveXsurface(vec, 0);
  case 2: return retrieveYsurface(vec, 2);
  case 3: return retrieveYsurface(vec, 0);
  case 4: return retrieveZsurface(vec, 0);
  case 5: return retrieveZsurface(vec, 2);
  case 6: return retrieveXsurface(vec, 1);
  case 7: return retrieveYsurface(vec, 1);
  case 8: return retrieveZsurface(vec, 1);
  default: return {0.0f, 0.0f, 0.0f};
  }
} //retrieveCubits_Center

glm::vec3 CubeRenderer::retrieveXsurface(std::vector<RenderCubit*> *vec, int x) {
  glm::vec3 center;
  for (size_t i = 0; i < MAX_D; i++) {
    for (size_t j = 0; j < MAX_D; j++) {
      vec->push_back(cubitsP[x][i][j]);
      if (i == 1 && j == 1) center = cubitsP[x][i][j]->getCentroid();
    } //for
  } //for
  return center;
} //retrieveXsurface

glm::vec3 CubeRenderer::retrieveYsurface(std::vector<RenderCubit*> *vec, int y) {
  glm::vec3 center;
  for (size_t i = 0; i < MAX_D; i++) {
    for (size_t j = 0; j < MAX_D; j++) {
      vec->push_back(cubitsP[i][y][j]);
      if (i == 1 && j == 1) center = cubitsP[i][y][j]->getCentroid();
    } //for
  } //for
  return center;
} //retrieveYsurface

glm::vec3 CubeRenderer::retrieveZsurface(std::vector<RenderCubit*> *vec, int z) {
  glm::vec3 center;
  for (size_t i = 0; i < MAX_D; i++) {
    for (size_t j = 0; j < MAX_D; j++) {
      vec->push_back(cubitsP[i][j][z]);
      if (i == 1 && j == 1) center = cubitsP[i][j][z]->getCentroid();
    } //for
  } //for
  return center;
} //retrieveZsurface

void CubeRenderer::doPointerMove() {
  Rotation rotate = animation.front().rotation;
  
  switch (animation.front().move) {
    case 0: R_Rotate(rotate); break;
    case 1: L_Rotate(rotate); break;
    case 2: U_Rotate(rotate); break;
    case 3: D_Rotate(rotate); break;
    case 4: F_Rotate(rotate); break;
    case 5: B_Rotate(rotate); break;
    case 6: M_Rotate(rotate); break;
    case 7: E_Rotate(rotate); break;
    case 8: S_Rotate(rotate); break;
    default: break;
  }
} //doPointerMove

void CubeRenderer::RLM(Rotation rot, int x) {
  for (uint8 i = 0; i < rot; i++) {
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
  for (uint8 i = 0; i < rot; i++) {
    //move corners
    RenderCubit *temp = cubitsP[0][y][0]; //old front left
    cubitsP[0][y][0] = cubitsP[2][y][0]; //new front left is old front right 
    cubitsP[2][y][0] = cubitsP[2][y][2]; //new front right is old back right
    cubitsP[2][y][2] = cubitsP[0][y][2]; //new back right is old back left
    cubitsP[0][y][2] = temp; //new back left is old front left 

    //move edges
    temp = cubitsP[1][y][0]; //old front middle
    cubitsP[1][y][0] = cubitsP[2][y][1]; //new front middle becomes old right middle
    cubitsP[2][y][1] = cubitsP[1][y][2]; //new right middle becomes old back middle
    cubitsP[1][y][2] = cubitsP[0][y][1]; //new back middle becomes old left middle
    cubitsP[0][y][1] = temp; //new left middle becomes old front middle
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
  for (uint8 i = 0; i < rot; i++) {
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
}