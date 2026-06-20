#ifndef HKOCIEMBAH
#define HKOCIEMBAH

#include "macros.h"

int const NCORNERS = 8;
int const NEDGES = 12;

struct CubieCube {
  uint8 cp[NCORNERS];
  uint8 co[NCORNERS];
  uint8 ep[NEDGES];
  uint8 eo[NEDGES];

  //get orientation of corners 
  //8 corners with 3 possible orientations for every corner
  //last corner is implied
  int getCO();
  //set orientation of all corners to t
  void setCO(int t);

  //get orientation of all edges, 
  //12 edges with 2 possible orientations for every edge
  //last edge is implied
  int getEO();
  //set orientation of all edges to f
  void setEO(int f);

  //get position of all corners 
  //8! possible cube permutations for corners
  int getCP();
  //set permutation of corners to p
  void setCP(int p);

  //get permutation of all edges 
  //12! possible cube permutations for edges
  int getEP();
  //set permutation of edges to p
  void setEP(int p);
};

#endif