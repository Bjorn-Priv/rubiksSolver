#ifndef HRCUBEH
#define HRCUBEH

#include "cubits.h"
#include <random>

//Degrees of rotation 
//CLOCKWISE
//COUNT_COCKWISE
//HALF_CIRCLE
enum Rotation : uint8 {
  CLOCKWISE = 1,
  HALF_CIRCLE = 2,
  COUNT_CLOCKWISE = 3
};

struct Coords {
  int z;
  int y;
  int x;
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

    void swapXYcol(Coords dest, const Cubit &src);
    void swapXZcol(Coords dest, const Cubit &src);
    void swapYZcol(Coords dest, const Cubit &src);

    void RLM(Rotation, int x);
    void UDE(Rotation, int y);
    void FBS(Rotation, int z);

  public:
    //default constructor
    RCube();

    void R_Rotate(Rotation);
    void L_Rotate(Rotation);
    void U_Rotate(Rotation);
    void D_Rotate(Rotation);
    void F_Rotate(Rotation);
    void B_Rotate(Rotation);

    void M_Rotate(Rotation);
    void E_Rotate(Rotation);
    void S_Rotate(Rotation);
    
    void performMove(int num, int rot);

    void shuffleCube(int moves);

    //prints the cube faces in unfolded die form
    void printCube(std::ostream&);

    Cubit getCubit(int x, int y, int z) const {return cube[z][y][x];}

    //default destructor
    ~RCube(){};
};

#endif