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
  //shuffleCube(20);
  // //flip bottom right corner of white
  // cube[0][0][2].X = WHITE;
  // cube[0][0][2].Y = ORANGE;
  // cube[0][0][2].Z = BLUE;
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

void RCube::swapXYcol(Coords dest, const Cubit &src) {
  cube[dest.z][dest.y][dest.x].X = src.Y;
  cube[dest.z][dest.y][dest.x].Y = src.X;
  cube[dest.z][dest.y][dest.x].Z = src.Z;
} //swapZYcol

void RCube::swapXZcol(Coords dest, const Cubit &src) {
  cube[dest.z][dest.y][dest.x].X = src.Z;
  cube[dest.z][dest.y][dest.x].Z = src.X;
  cube[dest.z][dest.y][dest.x].Y = src.Y;
} //swapZYcol

void RCube::swapYZcol(Coords dest, const Cubit &src) {
  cube[dest.z][dest.y][dest.x].Y = src.Z;
  cube[dest.z][dest.y][dest.x].Z = src.Y;
  cube[dest.z][dest.y][dest.x].X = src.X;
} //swapZYcol

void RCube::RLM(Rotation rot, int x) {
  for (uint8 i = 0; i < rot; i++) {
    //move corners
    Cubit temp = cube[2][2][x]; //back top
    swapYZcol({2, 2, x}, cube[0][2][x]);
    swapYZcol({0, 2, x}, cube[0][0][x]);
    swapYZcol({0, 0, x}, cube[2][0][x]);
    swapYZcol({2, 0, x}, temp);

    //move edges
    temp = cube[1][2][x]; //top middle
    swapYZcol({1, 2, x}, cube[0][1][x]);
    swapYZcol({0, 1, x}, cube[1][0][x]);
    swapYZcol({1, 0, x}, cube[2][1][x]);
    swapYZcol({2, 1, x}, temp);

    //swap middle piece just for funsies
    temp = cube[1][1][x];
    swapYZcol({1, 1, x}, temp);
  } //for
} //RLM

void RCube::R_Rotate(Rotation rot) {
  RLM(rot, 2);
} //R_Rotate

void RCube::L_Rotate(Rotation rot) {
  RLM(rot, 0);
} //L_Rotate

void RCube::M_Rotate(Rotation rot) {
  RLM(rot, 1);
} //M_Rotate

void RCube::UDE(Rotation rot, int y) {
  for (uint8 i = 0; i < rot; i++) {
    //move corners
    Cubit temp = cube[0][y][0]; //grab front left 
    swapXZcol({0, y, 0}, cube[0][y][2]);
    swapXZcol({0, y, 2}, cube[2][y][2]);
    swapXZcol({2, y, 2}, cube[2][y][0]);
    swapXZcol({2, y, 0}, temp);

    //edges
    temp = cube[0][y][1];
    swapXZcol({0, y, 1}, cube[1][y][2]);
    swapXZcol({1, y, 2}, cube[2][y][1]);
    swapXZcol({2, y, 1}, cube[1][y][0]);
    swapXZcol({1, y, 0}, temp);

    //middle
    temp = cube[1][y][1];
    swapXZcol({1, y, 1}, temp);
  } //for
} //UDE

void RCube::U_Rotate(Rotation rot) {
  UDE(rot, 2);
} //U_Rotate

void RCube::D_Rotate(Rotation rot) {
  UDE(rot, 0);
} //D_Rotate

void RCube::E_Rotate(Rotation rot) {
  UDE(rot, 1);
} //E_Rotate

void RCube::FBS(Rotation rot, int z) {
  for (uint8 i = 0; i < rot; i++) {
    //move corners
    Cubit temp = cube[z][0][0]; //grab bottom left
    swapXYcol({z, 0, 0}, cube[z][0][2]);
    swapXYcol({z, 0, 2}, cube[z][2][2]);
    swapXYcol({z, 2, 2}, cube[z][2][0]);
    swapXYcol({z, 2, 0}, temp);

    //edges
    temp = cube[z][0][1];
    swapXYcol({z, 0, 1}, cube[z][1][2]);
    swapXYcol({z, 1, 2}, cube[z][2][1]);
    swapXYcol({z, 2, 1}, cube[z][1][0]);
    swapXYcol({z, 1, 0}, temp);

    //middle
    temp = cube[z][1][1];
    swapXYcol({z, 1, 1}, temp);
  } //for
} //FBS

void RCube::B_Rotate(Rotation rot) {
  FBS(rot, 2);
} //B_Rotate

void RCube::F_Rotate(Rotation rot) {
  FBS(rot, 0);
} //F_Rotate

void RCube::S_Rotate(Rotation rot) {
  FBS(rot, 1);
} //S_Rotate

void RCube::shuffleCube(int moves) {
  int n = moves; 
  //{movenum, rotation}
  std::vector<std::pair<int, int>> sequence;
  
  std::random_device rd;
  std::mt19937 gen(rd());
  
  //two ranges
  std::uniform_int_distribution<> dis_0_9(0, 9);
  std::uniform_int_distribution<> dis_1_3(1, 3);

  if (n > 0) {
    //gen  first item
    int last_num = dis_0_9(gen);
    int associated_num = dis_1_3(gen);
    sequence.push_back({last_num, associated_num});

    //gen the remaining items
    for (int i = 1; i < n; ++i) {
      int current_num;
      do {
        current_num = dis_0_9(gen);
      } while (current_num == last_num);
      
      int current_associated = dis_1_3(gen);
      sequence.push_back({current_num, current_associated});
      
      last_num = current_num;
    } //for
  }

  //Output the results: (0-8 value, 1-3 value)
  for (const auto& item : sequence) {
    performMove(item.first, item.second);
  }
} //shuffleCube

void RCube::performMove(int num, int rot) {
  Rotation rotate;
  if (rot == 1) {
    rotate = CLOCKWISE;
  } else if (rot == 2) {
    rotate = HALF_CIRCLE;
  } else {
    rotate = COUNT_CLOCKWISE;
  }
  
  switch (num) {
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
} //performMove
