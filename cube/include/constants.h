#ifndef HCONSTANTH
#define HCONSTANTH

#include "macros.h"

//max number of colours on rubiks cube
const uint64 MAX_NCOLOURS = 6;
//max dimension of rubiks cube
const uint64 MAX_D = 3;

//number of cubits in entire cube
const int NCUBITS = 27;
//number of float used for every vertex
//3 for coords, 3 for colours
const int NFLOATS_PER_VERTEX = 6;
//number of vertices per cubit 
const int NVERTICES_PER_CUBIT = 24;
//number of triangles to be drawn per cubit
const int NTRIANGLES_PER_CUBIT = 12;
//number of integers per triangle to indicate corners
const int NINTS_PER_TRIANGLE = 3;
#endif