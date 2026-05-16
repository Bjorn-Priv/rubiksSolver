#ifndef HRCUBEH
#define HRCUBEH

#include "cubits.h"

//Degrees of rotation 
//CLOCKWISE
//COUNT_COCKWISE
//HALF_CIRCLE
enum Rotation : uint8 {
  CLOCKWISE,
  COUNT_CLOCKWISE,
  HALF_CIRCLE
};

class RCube {
  private:
    //holds every cubit 
    //[z][y][x]
    //[0][0][0] is the bottom left corner of white facing front
    Cubit cube[MAX_D][MAX_D][MAX_D]; 

    //bottom left is [0][0]
    Colour face[MAX_NCOLOURS][MAX_D][MAX_D];

    void setCubeColoursSolved();
    void inferCubeFaces();
    void setCubeCubitTypes();
      
  public:
    RCube();

    bool R_Rotate(Rotation);
    bool L_Rotate(Rotation);
    bool U_Rotate(Rotation);
    bool D_Rotate(Rotation);
    bool F_Rotate(Rotation);
    bool B_Rotate(Rotation);

    bool M_Rotate(Rotation);
    bool E_Rotate(Rotation);
    bool S_Rotate(Rotation);

    void printCube(std::ostream&);

    ~RCube(){};
};

#endif