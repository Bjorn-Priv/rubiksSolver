#ifndef HRCUBEH
#define HRCUBEH

#include "cubits.h"
#include "standard.h"

//Degrees of rotation 
//CLOCKWISE
//COUNT_COCKWISE
//HALF_CIRCLE
enum Rotation : uint8 {
  CLOCKWISE = 1,
  HALF_CIRCLE = 2,
  COUNT_CLOCKWISE = 3
};

//helper struct to store coordinates on a 3d plane
struct Coords {
  int z;
  int y;
  int x;
};

//enumerator for every move identity on RCube
//R, L, U, D, F, B, M, E, S
enum MoveID {
  R,
  L,
  U,
  D,
  F,
  B,
  M,
  E,
  S, 
  RANDOM
};

//main logical rubiks cube class
class RCube {
  private:
    //holds every cubit 
    //[z][y][x]
    //[0][0][0] is the bottom left corner of white facing front
    Cubit cube[MAX_D][MAX_D][MAX_D]; 

    //bottom left is [0][0]
    //contains the unfolded cube in unfolded die form
    Colour face[MAX_NCOLOURS][MAX_D][MAX_D];

    //sets all colours of the cube to their solved state
    void setCubeColoursSolved();
    //infers cube faces from the Cubit cube
    void inferCubeFaces();
    //sets all cubit types to their correct value
    void setCubeCubitTypes();

    //helper function to set 
    //dest X colour to Y of src
    //dest Y colour to X of src
    //dest Z colour to Z of src
    void swapXYcol(Coords dest, const Cubit &src);
    //helper function to set 
    //dest X colour to Z of src
    //dest Y colour to Y of src
    //dest Z colour to X of src
    void swapXZcol(Coords dest, const Cubit &src);
    //helper function to set 
    //dest X colour to X of src
    //dest Y colour to Z of src
    //dest Z colour to Y of src
    void swapYZcol(Coords dest, const Cubit &src);

    //helper functions to perform moves R, L, and M 
    //with the correct x-axis of the face
    void RLM(Rotation, int x);
    //helper functions to perform moves U, D, and E 
    //with the correct y-axis of the face
    void UDE(Rotation, int y);
    //helper functions to perform moves F, B, and S 
    //with the correct z-axis of the face
    void FBS(Rotation, int z);

    //_ rotation in direction of parameter
    //in respect to white facing front and green facing top
    void R_Rotate(Rotation), L_Rotate(Rotation), U_Rotate(Rotation),
         D_Rotate(Rotation), F_Rotate(Rotation), B_Rotate(Rotation), 
         M_Rotate(Rotation), E_Rotate(Rotation), S_Rotate(Rotation);

  public:
    //default constructor
    RCube();

    //perform move with Move ID move and Rotation rot
    void performMove(MoveID move, Rotation rot);

    //shuffle cube by doing (int)Nmoves different moves
    void shuffleCube(int Nmoves);

    //prints the cube faces in unfolded die form
    void printCube(std::ostream&);

    //getter for specific cubit with coords x, y, z
    Cubit getCubit(int x, int y, int z) const {return cube[z][y][x];}

    //default destructor
    ~RCube(){};
};

#endif