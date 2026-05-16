#include "include/cuberenderer.h"

CubeRenderer::CubeRenderer(RCube *c, GLuint shader) : cube(c) {
  view = glm::lookAt(
    glm::vec3(6, 6, 6),
    glm::vec3(0, 0, 0),
    glm::vec3(0, 1, 0)
  );

  projection = glm::perspective(
    glm::radians(45.0f),
    1.0f,
    0.1f,
    100.0f
  );
  shaderProgram = shader;
  modelLoc = glGetUniformLocation(shaderProgram, "model");
  viewLoc = glGetUniformLocation(shaderProgram, "view");
  projLoc = glGetUniformLocation(shaderProgram, "projection");

  initMesh();
}

void CubeRenderer::initMesh() {
  float vertices[] = {
    // positions
    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f
  };

  unsigned int indices[] = {
    0,1,2, 2,3,0,
    4,5,6, 6,7,4,
    0,4,7, 7,3,0,
    1,5,6, 6,2,1,
    3,2,6, 6,7,3,
    0,1,5, 5,4,0
  };

  glGenVertexArrays(1, &cubeVAO);
  glGenBuffers(1, &cubeVBO);
  glGenBuffers(1, &cubeEBO);

  glBindVertexArray(cubeVAO);

  glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // position attribute (location = 0 in shader)
  glVertexAttribPointer(
      0, 3, GL_FLOAT, GL_FALSE,
      3 * sizeof(float),
      (void*)0
  );

  glEnableVertexAttribArray(0);

  glBindVertexArray(0);
}

void CubeRenderer::render() {
  glUseProgram(shaderProgram);

  RenderCubit& c = cubits[1][1][1];

  glm::mat4 model(1.0f);

  model = glm::translate(
    model,
    glm::vec3(0.0f, 0.0f, 0.0f)
  );

  model *= c.orientation;

  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);

  glBindVertexArray(cubeVAO);
  glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}