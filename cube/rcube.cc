#include "include/rcube.h"

RCube::RCube() {
  setCubeCubitTypes();
  setCubeColoursSolved();
}

void RCube::setCubeColoursSolved() {
  //set Z directions
  for (size_t i = 0; i < MAX_D; i++) {
    for (size_t j = 0; j < MAX_D; j++) {
      cube[0][i][j].Z = WHITE; //front
      cube[2][i][j].Z = YELLOW; //back
      cube[i][0][j].Y = BLUE; //bottom
      cube[i][2][j].Y = GREEN; //top
      cube[i][j][0].X = RED; //left
      cube[i][j][2].X = ORANGE; //right
    } //for
  } //for
} //setCubeColoursSolved

void RCube::inferCubeFaces() {
  for (size_t i = 0; i < MAX_D; i++) {
    for (size_t j = 0; j < MAX_D; j++) {
      face[WHITE][i][j] = cube[0][i][j].Z;
      face[YELLOW][i][j] = cube[2][MAX_D-i-1][j].Z;
      face[RED][i][j] = cube[MAX_D-j-1][i][0].X;
      face[ORANGE][i][j] = cube[j][i][2].X;
      face[GREEN][i][j] = cube[i][2][j].Y;
      face[BLUE][i][j] = cube[MAX_D-i-1][0][j].Y;
    } //for
  } //for
} //inferCubeFaces

void RCube::setCubeCubitTypes() {
  //  grid sums:
  //  2   3   4
  //  1   2   3
  //  0   1   2
  for (size_t i = 0; i < MAX_D; i++) {
    for (size_t j = 0; j < MAX_D; j++) {
      int sum = i+j;
      if (sum == 3 || sum == 1) { //edges of grid
        cube[0][i][j].type = EDGE;
        cube[1][i][j].type = MIDDLE;
        cube[2][i][j].type = EDGE;

      } else if (sum == 2 && i == j) { //middle of grid
        cube[0][i][j].type = MIDDLE;
        cube[1][i][j].type = CENTROID;
        cube[2][i][j].type = MIDDLE;

      } else { //corners of grid
        cube[0][i][j].type = CORNER;
        cube[1][i][j].type = EDGE;
        cube[2][i][j].type = CORNER;
      }
    } //for
  } //for
} //setCubeCubitTypes

void RCube::printCube(std::ostream &os) {
  inferCubeFaces();

  for (size_t i = 0; i < MAX_NCOLOURS; i++) {
    os << "Colour: " << i << "\n";
    for (int j = (MAX_D-1); j >= 0; j--) {
      for (size_t k = 0; k < MAX_D; k++) {
        os << (int)face[i][j][k] << " ";
      } //for
      os << "\n";
    } //for
  } //for
} //printCube