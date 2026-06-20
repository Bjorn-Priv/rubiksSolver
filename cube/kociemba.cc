#include "include/kociemba.h"

//factorial lookup table for 0-12 (inclusive)
int constexpr FACT[13] {
  1,
  1,
  2,
  6,
  24,
  120,
  720,
  5040,
  40320,
  362880,
  3628800,
  39916800,
  479001600
};

int CubieCube::getCO() {
  int CO = 0;
  for (int i = 0; i < (NCORNERS-1); i++) CO = CO*3 + co[i];
  return CO;
} //getCO

void CubieCube::setCO(int CO) {
  int parity = 0;
  int rest = 0;
  for (int i = (NCORNERS-2); i >= 0; i--) { 
    co[i] = CO % 3;
    parity += co[i];
    CO /= 3;
  } //for
  //entirety needs divisbile by 3
  rest = parity % 3;
  co[7] = (rest == 0) ? 0 : (3 - rest);
} //setCO

int CubieCube::getEO() {
  int EO = 0;
  for (int i = 0; i < (NEDGES-1); i++) EO = EO*2 + eo[i];
  return EO;
} //getEO

void CubieCube::setEO(int EO) {
  int parity = 0;
  for (int i = (NEDGES-2); i >= 0; i--) { //for all edges but last one
    eo[i] = EO % 2;
    parity += eo[i];
    EO /= 2;
  } //for
  eo[11] = parity % 2; //last edge is decided with parity
} //setEO

int CubieCube::getCP() {
  int perm = 0;

  for (int i = (NCORNERS-1); i > 0; i--) { //right to left
    int count = 0;

    for (int j = i-1; j >= 0; j--) 
      if (cp[j] > cp[i]) count++;

    perm += count*FACT[i];
  } //for
  return perm;
} //getCperm

void CubieCube::setCP(int p) {
  bool avail[NCORNERS];
  for (int i = 0; i < NCORNERS; i++) {
    avail[i] = true;
  }
  
  int perm = 0;
  for (int i = (NCORNERS-1); i >= 0; i--) {
    perm = p / FACT[i];
    p -= perm * FACT[i];
    int count = 0;
    int j;
    for (j = (NCORNERS-1); j >= 0; j--) {
      if (avail[j]) {
        if (count == perm) break;
        count++;
      }
    }
    avail[j] = false;
    cp[i] = j;
  }
} //setCperm

int CubieCube::getEP() {
  int perm = 0;

  for (int i = (NEDGES-1); i > 0; i--) { //for all but last edge
    int count = 0;

    for (int j = i-1; j >= 0; j--) //for all previous edges
      if (ep[j] > ep[i]) count++;

    perm += count*FACT[i];
  } //for
  return perm;
} //getEperm

void CubieCube::setEP(int p) {
  bool avail[NEDGES];
  for (int i = 0; i < NEDGES; i++) {
    avail[i] = true;
  }
  
  int perm = 0;
  for (int i = (NEDGES-1); i >= 0; i--) {
    perm = p / FACT[i];
    p -= perm * FACT[i];
    int count = 0;
    int j;
    for (j = (NEDGES-1); j >= 0; j--) {
      if (avail[j]) {
        if (count == perm) break;
        count++;
      }
    }
    avail[j] = false;
    ep[i] = j;
  }
} //setEperm