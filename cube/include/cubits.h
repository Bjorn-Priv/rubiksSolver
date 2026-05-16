#ifndef HCUBITH
#define HCUBITH

#include "constants.h"

//colour of the face of the cubit
//WHITE
//GREEN
//RED
//ORANGE
//BLUE
//YELLOW
//EMPTY
enum Colour : uint8 {
  WHITE,
  GREEN,
  RED,
  ORANGE,
  BLUE,
  YELLOW,
  EMPTY
};

//type of the cubit
//CORNER
//EDGE
//MIDDLE
//CENTROID
enum CubitType : uint8 {
  CORNER,
  EDGE,
  MIDDLE,
  CENTROID
};

//single cube of the the entire rubiks cube
//can be a CORNER, SIDE, or MIDDLE cubit
//these respectively have 3, 2 and 1 colours
//assume for all cubits orientation is based
//on bottom left corner of white face being [0][0][0]
//then every cubit has a sticker pointing toward
//an x direction, y direction, and or z direction 
//depending on which face it is currently on 
//and which way it is oriented
//colours start on EMPTY
struct Cubit {
  //CORNER, SIDE, or MIDDLE
  //3, 2, or 1 colours
  CubitType type = MIDDLE;

  //colour facing x direction
  Colour X = EMPTY;
  //colour facing y direction
  Colour Y = EMPTY;
  //colour facing z direction
  Colour Z = EMPTY;
};

#endif