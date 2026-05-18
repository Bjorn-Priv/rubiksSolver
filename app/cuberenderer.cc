#include "include/cuberenderer.h"
#include <vector>

CubeRenderer::CubeRenderer(RCube *c, std::vector<CubeAction> *a, Camera *cam, GLuint shader) : cube(c), actions(a), camera(cam) {
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
        Voffset += (VERTICESCOUNT*6);
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
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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

  for (size_t z = 0; z < MAX_D; z++) {
    for (size_t y = 0; y < MAX_D; y++) {
      for (size_t x = 0; x < MAX_D; x++) {
        RenderCubit& c = cubits[x][y][z]; //grab cubit

        glm::mat4 model(1.0f); //create identity matrix

        //set cubit rotation
        model *= c.orientation;

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

CubeRenderer::~CubeRenderer() {
  //destroy all gl buffers and arrays
  glDeleteBuffers(1, &cubeVBO);
  glDeleteBuffers(1, &cubeEBO);
  glDeleteVertexArrays(1, &cubeVAO);
}