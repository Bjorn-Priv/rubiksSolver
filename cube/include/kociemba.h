#ifndef HKOCIEMBAH
#define HKOCIEMBAH

#include "macros.h"
#include <vector>

int const NCORNERS = 8;
int const NEDGES = 12;
int const NMOVES1 = 18;
int const NMOVES2 = 10;
int const MAXCO = 2187;
int const MAXCP = 40320;
int const MAXEO = 2048;
int const MAXEP = 40320;
int const MAXSLICE = 495;
int const MAXSLICEP = 24;

enum Edge : uint8 {
  UR,
  UF,
  UL,
  UB,
  DR,
  DF,
  DL,
  DB,
  FR,
  FL,
  BL,
  BR
};

enum Corner : uint8 {
  URF,
  UFL,
  ULB,
  UBR,
  DFR,
  DLF,
  DBL,
  DRB
};

enum Moves {
  U1, U2, U3,
  R1, R2, R3,
  F1, F2, F3,
  D1, D2, D3,
  L1, L2, L3,
  B1, B2, B3
};

class CubieCube {
  public: 
  Corner cp[NCORNERS];
  uint8 co[NCORNERS];

  Edge ep[NEDGES];
  uint8 eo[NEDGES];

  CubieCube();
  CubieCube(const Corner cp_[8],
            const uint8 co_[8],
            const Edge ep_[12],
            const uint8 eo_[12]);

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


  //gets slice coordinate
  int getSlice();
  //sets slice coordinate
  void setSlice(int s);

  //gets UDSlice coordinate value
  int getUDSlicePerm();
  //set UDSlice coordinate
  void setUDSlicePerm(int s);

  void move(Moves m);

  CubieCube operator*(const CubieCube &a) const;

  private: 
};

class Kociemba {
  std::vector<std::vector<int>> COMOVE;
  std::vector<std::vector<int>> EOMOVE;
  std::vector<std::vector<int>> SLICEMOVE;

  std::vector<std::vector<int>> CPMOVE;
  std::vector<std::vector<int>> EPMOVE;
  std::vector<std::vector<int>> SLICEPMOVE;

  std::vector<std::vector<int>> pruneP1;
  std::vector<std::vector<int>> pruneP2;

  void initPhase1();
  void initPhase2();

  public: 
  Kociemba();
};

static const Corner cpU[8]  = { UBR, URF, UFL, ULB, DFR, DLF, DBL, DRB };
static const uint8 coU[8]  = { 0, 0, 0, 0, 0, 0, 0, 0 };
static const Edge epU[12] = { UB, UR, UF, UL, DR, DF, DL, DB, FR, FL, BL, BR };
static const uint8 eoU[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
static const Corner cpR[8]  = { DFR, UFL, ULB, URF, DRB, DLF, DBL, UBR };
static const uint8 coR[8]  = { 2, 0, 0, 1, 1, 0, 0, 2 };
static const Edge epR[12] = { FR, UF, UL, UB, BR, DF, DL, DB, DR, FL, BL, UR };
static const uint8 eoR[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
static const Corner cpF[8]  = { UFL, DLF, ULB, UBR, URF, DFR, DBL, DRB };
static const uint8 coF[8]  = { 1, 2, 0, 0, 2, 1, 0, 0 };
static const Edge epF[12] = { UR, FL, UL, UB, DR, FR, DL, DB, UF, DF, BL, BR };
static const uint8 eoF[12] = { 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0 };
static const Corner cpD[8]  = { URF, UFL, ULB, UBR, DLF, DBL, DRB, DFR };
static const uint8 coD[8]  = { 0, 0, 0, 0, 0, 0, 0, 0 };
static const Edge epD[12] = { UR, UF, UL, UB, DF, DL, DB, DR, FR, FL, BL, BR };
static const uint8 eoD[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
static const Corner cpL[8]  = { URF, ULB, DBL, UBR, DFR, UFL, DLF, DRB };
static const uint8 coL[8]  = { 0, 1, 2, 0, 0, 2, 1, 0 };
static const Edge epL[12] = { UR, UF, BL, UB, DR, DF, FL, DB, FR, UL, DL, BR };
static const uint8 eoL[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
static const Corner cpB[8]  = { URF, UFL, UBR, DRB, DFR, DLF, ULB, DBL };
static const uint8 coB[8]  = { 0, 0, 1, 2, 0, 0, 2, 1 };
static const Edge epB[12] = { UR, UF, UL, BR, DR, DF, DL, BL, FR, FL, UB, DB };
static const uint8 eoB[12] = { 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1 };

const CubieCube MOVE_U(cpU, coU, epU, eoU);
const CubieCube MOVE_R(cpR, coR, epR, eoR);
const CubieCube MOVE_F(cpF, coF, epF, eoF);
const CubieCube MOVE_D(cpD, coD, epD, eoD);
const CubieCube MOVE_L(cpL, coL, epL, eoL);
const CubieCube MOVE_B(cpB, coB, epB, eoB);

#endif