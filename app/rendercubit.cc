#include "include/rendercubit.h"

static float COLOURS[7][3] = {
  {1.0f, 1.0f, 1.0f},
  {0.0f, 0.502f, 0.0f},
  {1.0f, 0.0f, 0.0f},
  {1.0f, 0.647f, 0.0f},
  {0.0f, 0.0f, 1.0f},
  {1.0f, 1.0f, 0.0f},
  {0.0f, 0.0f, 0.0f}
};

unsigned int *RenderCubit::initCubit() {
  SDL_Log("before");
  SDL_Log("Centroid: %f, %f, %f", centroid[0], centroid[1], centroid[2]);
  Colour NegXcol = EMPTY;
  Colour PosXcol = EMPTY;
  Colour NegYcol = EMPTY;
  Colour PosYcol = EMPTY;
  Colour NegZcol = EMPTY;
  Colour PosZcol = EMPTY;

  int Vindex = 0;
  int Iindex = 0;

  //set x Colour
  if (centroid[0] > 0.0f) PosXcol = cubit.X;
  else if (centroid[0] < 0.0f) NegXcol = cubit.X;

  SDL_Log("X");
  createXWall(NegXcol, -0.5f, Vindex, Iindex);
  createXWall(PosXcol, 0.5f, Vindex, Iindex);

  //set Y Colour
  if (centroid[1] > 0.0f) PosYcol = cubit.Y;
  else if (centroid[1] < 0.0f) NegYcol = cubit.Y;

  SDL_Log("Y");
  createYWall(NegYcol, -0.5f, Vindex, Iindex);
  createYWall(PosYcol, 0.5f, Vindex, Iindex);

  //set Z Colour
  if (centroid[2] > 0.0f) PosZcol = cubit.Z;
  else if (centroid[2] < 0.0f) NegZcol = cubit.Z;

  SDL_Log("Z");
  createZWall(NegZcol, -0.5f, Vindex, Iindex);
  createZWall(PosZcol, 0.5f, Vindex, Iindex);

  return indices;
} //render Cubit

void RenderCubit::createXWall(Colour col, float offset, int &Vindex, int &Iindex) {
  float vertexX = centroid[0] + offset;
  float vertexY[2] = {(centroid[1] - 0.5f), (centroid[1] + 0.5f)};
  float vertexZ[2] = {(centroid[2] - 0.5f), (centroid[2] + 0.5f)};

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      data[Vindex] = vertexX;
      data[Vindex+1] = vertexY[i];
      data[Vindex+2] = vertexZ[j];
      data[Vindex+3] = COLOURS[col][0];
      data[Vindex+4] = COLOURS[col][1];
      data[Vindex+5] = COLOURS[col][2];
      Vindex += 6;
    }
  }
  unsigned int base = Iindex / 6 * 4;

  indices[Iindex] = base;
  indices[Iindex+1] = base+1;
  indices[Iindex+2] = base+2;
  indices[Iindex+3] = base+2;
  indices[Iindex+4] = base+1;
  indices[Iindex+5] = base+3;
  Iindex+=6;
}

void RenderCubit::createYWall(Colour col, float offset, int &Vindex, int &Iindex) {
  float vertexX[2] = {(centroid[0] - 0.5f), (centroid[0] + 0.5f)};
  float vertexY = centroid[1] + offset;
  float vertexZ[2] = {(centroid[2] - 0.5f), (centroid[2] + 0.5f)};
  
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      data[Vindex] = vertexX[i];
      data[Vindex+1] = vertexY;
      data[Vindex+2] = vertexZ[j];
      data[Vindex+3] = COLOURS[col][0];
      data[Vindex+4] = COLOURS[col][1];
      data[Vindex+5] = COLOURS[col][2];
      Vindex += 6;
    }
  }
  unsigned int base = Iindex / 6 * 4;

  indices[Iindex] = base;
  indices[Iindex+1] = base+1;
  indices[Iindex+2] = base+2;
  indices[Iindex+3] = base+2;
  indices[Iindex+4] = base+1;
  indices[Iindex+5] = base+3;
  Iindex+=6;
}

void RenderCubit::createZWall(Colour col, float offset, int &Vindex, int &Iindex) {
  float vertexX[2] = {(centroid[0] - 0.5f), (centroid[0] + 0.5f)};
  float vertexY[2] = {(centroid[1] - 0.5f), (centroid[1] + 0.5f)};
  float vertexZ = centroid[2] + offset;
  
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      data[Vindex] = vertexX[i];
      data[Vindex+1] = vertexY[j];
      data[Vindex+2] = vertexZ;
      data[Vindex+3] = COLOURS[col][0];
      data[Vindex+4] = COLOURS[col][1];
      data[Vindex+5] = COLOURS[col][2];
      Vindex += 6;
    }
  }
  unsigned int base = Iindex / 6 * 4;

  indices[Iindex] = base;
  indices[Iindex+1] = base+1;
  indices[Iindex+2] = base+2;
  indices[Iindex+3] = base+2;
  indices[Iindex+4] = base+1;
  indices[Iindex+5] = base+3;
  Iindex+=6;
}

void RenderCubit::update(float angle, glm::vec3 center, glm::vec3 axis) {
  for (int i = 0; i < NVERTICES_PER_CUBIT; i++) {
    int index = i*6;
    glm::vec3 location = {data[index], data[index+1], data[index+2]};
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, center);
    trans = glm::rotate(trans, angle, axis);
    trans = glm::translate(trans, -center);

    glm::vec4 result = trans * glm::vec4(location, 1.0f);
    glm::vec3 finalPos = glm::vec3(result);

    data[index] = finalPos[0];
    data[index+1] = finalPos[1];
    data[index+2] = finalPos[2];
  }
}


