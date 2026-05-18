#include "include/cuberenderer.h"

CubeRenderer::CubeRenderer(RCube *c, std::vector<CubeAction> *a, Camera *cam, GLuint shader) : cube(c), actions(a), camera(cam) {
  shaderProgram = shader;

  //grab location of all uniform variables in the shader program
  //uniform variables are practically global variables that you can change
  modelLoc = glGetUniformLocation(shaderProgram, "model");
  viewLoc = glGetUniformLocation(shaderProgram, "view");
  projLoc = glGetUniformLocation(shaderProgram, "projection");

  initMesh();
}

void CubeRenderer::initMesh() {
  //all vertex positions 
  float vertices[] = {
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, //red 0
     0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, //red 1
     0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, //red 2
    -0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, //red 3

     0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, //blue 4
     0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, //blue 5
     0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, //blue 6
     0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, //blue 7

    -0.5f,  0.5f, -0.5f, 1.0f, 0.647f, 0.0f, //orange 8 
    -0.5f, -0.5f, -0.5f, 1.0f, 0.647f, 0.0f, //orange 9
    -0.5f,  0.5f,  0.5f, 1.0f, 0.647f, 0.0f, //orange 10
    -0.5f, -0.5f,  0.5f, 1.0f, 0.647f, 0.0f, //orange 11

    -0.5f,  0.5f,  0.5f, 0.0f, 0.502f, 0.0f, //green 12
    -0.5f, -0.5f,  0.5f, 0.0f, 0.502f, 0.0f, //green 13
     0.5f,  0.5f,  0.5f, 0.0f, 0.502f, 0.0f, //green 14
     0.5f, -0.5f,  0.5f, 0.0f, 0.502f, 0.0f, //green 15

    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f, //yellow 16 
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f, //yellow 17
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f, //yellow 18 
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f, //yellow 19

    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, //white 20
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, //white 21
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, //white 22
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, //white 23
  };

  //all vertex indices used to form triangles 
  unsigned int indices[] = {
    0,1,2, 2,3,0,
    4,5,6, 6,7,5,
    8,9,10, 10,11,9,
    12,13,14, 14,15,13,
    16,17,18, 18,19,16,
    20,21,22, 22,23,20
  };

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

  //grab single cubit
  RenderCubit& c = cubits[1][1][1];

  //create identity matrix
  glm::mat4 model(1.0f);

  //multiply model by a vector
  model = glm::translate(
    model,
    glm::vec3(0.0f, 0.0f, 0.0f)
  );

  //multiply model by orientation
  model *= c.orientation;

  //set all global variables in the shader
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &(camera->getView()[0][0]));
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, &(camera->getProjection()[0][0]));

  //set used vertex array to VAO
  glBindVertexArray(cubeVAO);
  //draw all elements
  //mode: TRIANGLES
  //count: amount of elements
  //type: the data type of the values in indices
  //indices: a byte offset into the buffer bound to draw from
  glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
  //unbind VAO
  glBindVertexArray(0);
}

CubeRenderer::~CubeRenderer() {
  //destroy all gl buffers and arrays
  glDeleteBuffers(1, &cubeVBO);
  glDeleteBuffers(1, &cubeEBO);
  glDeleteVertexArrays(1, &cubeVAO);
}