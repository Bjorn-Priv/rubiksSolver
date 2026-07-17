#include "include/kociemba.h"
#include <vector>

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

int constexpr CHOOSE[12][4] {
  {1, 0, 0, 0},
  {1, 1, 0, 0},
  {1, 2, 1, 0},
  {1, 3, 3, 1},
  {1, 4, 6, 4},
  {1, 5, 10, 10},
  {1, 6, 15, 20},
  {1, 7, 21, 35},
  {1, 8, 28, 56},
  {1, 9, 36, 84},
  {1, 10, 45, 120},
  {1, 11, 55, 165}
};

CubieCube::CubieCube() {
  for (int i = 0; i < NCORNERS; i++) {
    cp[i] = (Corner)i;
    co[i] = 0;
  }
  for (int i = 0; i < NEDGES; i++) {
    ep[i] = (Edge)i;
    eo[i] = 0;
  }
}

CubieCube::CubieCube(const Corner cp_[8],
            const uint8 co_[8],
            const Edge ep_[12],
            const uint8 eo_[12]) {
  for(int i=0;i<8;i++) {
    cp[i] = cp_[i];
    co[i] = co_[i];
  }

  for(int i=0;i<12;i++) {
    ep[i] = ep_[i];
    eo[i] = eo_[i];
  }
}

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
    cp[i] = (Corner)j;
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
    ep[i] = (Edge)j;
  }
} //setEperm

int CubieCube::getSlice() {
  int count = -1;
  int sum = 0;
  for (int i = 0; i < NEDGES; i++) {
    if (ep[i] >= FR) {
      count++;
    } else if (count >= 0) {
      sum += CHOOSE[i][count];
    }
  }
  return sum;
}

void CubieCube::setSlice(int s) {
  int count = 3;
  int FRidx = 0;
  int URidx = 0;
  for (int i = NEDGES-1; i >= 0; i--) {
    if (count < 0) {
      ep[i] = Edge(UR + URidx);
      URidx++;
    } else if (s >= CHOOSE[i][count]) {
      s -= CHOOSE[i][count];
      ep[i] = Edge(UR + URidx);
      URidx++;
    } else {
      ep[i] = Edge(FR + FRidx);
      FRidx++;
      count--;
    } 
  }
}

int CubieCube::getUDSlicePerm() {
  int count = 0;
  int arr[4];
  for (int i = 0; i < NEDGES; i++) {
    if (ep[i] >= FR) {
      arr[count] = ep[i];
      count++;
    }
  }

  int sum = 0;
  for (int i = 3; i > 0; i--) {
    int s = 0;
    for (int j = i-1; j >= 0; j--) {
      if (arr[j] > arr[i]) {
        s++;
      }
    }
    sum = (sum+s) * i;
  }
  return sum;
}

void CubieCube::setUDSlicePerm(int s) {
  int FRidx = 0;
  int URidx = 0;

  Edge permutation[4];
  int amounts[4] = {0};

  for (int i = 3; i > 0; i--) {
    int amount = s / FACT[i];
    amounts[i] = amount;
    s -= amount * FACT[i];
  }

  std::vector<Edge> avail = {BR, BL, FL, FR};

  for (int i = 3; i >= 0; i--) {
    permutation[i] = avail[amounts[i]];
    avail.erase(avail.begin() + amounts[i]);
  }
  
  for (int i = NEDGES-1; i >= 0; i--) {
    if (i < 8) {
      ep[i] = Edge(UR + URidx);
      URidx++;
    } else {
      ep[i] = permutation[3 - FRidx];
      FRidx++;
    } 
  }
}

void CubieCube::move(Moves m) {
  switch (m) {
  case U3: *this = *this * MOVE_U * MOVE_U * MOVE_U; break;
  case U2: *this = *this * MOVE_U * MOVE_U; break;
  case U1: *this = *this * MOVE_U; break;

  case R3: *this = *this * MOVE_R * MOVE_R * MOVE_R; break;
  case R2: *this = *this * MOVE_R * MOVE_R; break;
  case R1: *this = *this * MOVE_R; break;
  
  case F3: *this = *this * MOVE_F * MOVE_F * MOVE_F; break;
  case F2: *this = *this * MOVE_F * MOVE_F; break;
  case F1: *this = *this * MOVE_F; break;

  case D3: *this = *this * MOVE_D * MOVE_D * MOVE_D; break;
  case D2: *this = *this * MOVE_D * MOVE_D; break;
  case D1: *this = *this * MOVE_D; break;

  case L3: *this = *this * MOVE_L * MOVE_L * MOVE_L; break;
  case L2: *this = *this * MOVE_L * MOVE_L; break;
  case L1: *this = *this * MOVE_L; break;

  case B3: *this = *this * MOVE_B * MOVE_B * MOVE_B; break;
  case B2: *this = *this * MOVE_B * MOVE_B; break;
  case B1: *this = *this * MOVE_B; break;
  default:break;
  }
}

CubieCube CubieCube::operator*(const CubieCube &a) const {
  CubieCube result;
  for(int i = 0; i < NCORNERS; i++) {
    result.cp[i] = cp[a.cp[i]];
    result.co[i] = (co[a.cp[i]] + a.co[i]) % 3;
  }

  for(int i = 0; i < NEDGES; i++) {
    result.ep[i] = ep[a.ep[i]];
    result.eo[i] = (eo[a.ep[i]] + a.eo[i]) % 2;
  }
  return result;
}

Kociemba::Kociemba() : COMOVE(MAXCO, std::vector<int>(NMOVES1)), EOMOVE(MAXEO, std::vector<int>(NMOVES1)), 
                       SLICEMOVE(MAXSLICE, std::vector<int>(NMOVES1)),
                       CPMOVE(MAXCP, std::vector<int>(NMOVES2)), EPMOVE(MAXEP, std::vector<int>(NMOVES2)), 
                       SLICEPMOVE(MAXSLICEP, std::vector<int>(NMOVES2)) {
  initPhase1();
  initPhase2();
}

void Kociemba::initPhase1() {
  for (int i = 0; i < MAXCO; i++) {
    CubieCube c; 
    c.setCO(i);

    for (int j = 0; j < NMOVES1; j++) {
      CubieCube d = c;
      d.move((Moves)j);
      COMOVE[i][j] = d.getCO();
    }
  }

  for (int i = 0; i < MAXEO; i++) {
    CubieCube c;
    c.setEO(i);

    for (int j = 0; j < NMOVES1; j++) {
      CubieCube d = c;
      d.move((Moves)j);
      EOMOVE[i][j] = d.getEO();
    }
  }

  for (int i = 0; i < MAXSLICE; i++) {
    CubieCube c;
    c.setSlice(i);

    for (int j = 0; j < NMOVES1; j++) {
      CubieCube d = c;
      d.move((Moves)j);
      SLICEMOVE[i][j] = d.getSlice();
    }
  }
}

void Kociemba::initPhase2() {
  Moves phase2[NMOVES2] = {U1,U2,U3,R2,F2,D1,D2,D3,L2,B2};

  for (int i = 0; i < MAXCP; i++) {
    CubieCube c;
    c.setCP(i);
    for (int j = 0; j < NMOVES2; j++) {
      CubieCube d = c;
      d.move(phase2[j]);
      CPMOVE[i][j] = d.getCP();
    }
  }

  for (int i = 0; i < MAXEP; i++) {
    CubieCube c;
    c.setEP(i);

    for (int j = 0; j < NMOVES2; j++) {
      CubieCube d = c;
      d.move(phase2[j]);
      EPMOVE[i][j] = d.getEP();
    }
  }

  for (int i = 0; i < MAXSLICEP; i++) {
    CubieCube c;
    c.setUDSlicePerm(i);

    for (int j = 0; j < NMOVES2; j++) {
      CubieCube d = c;
      d.move(phase2[j]);
      SLICEPMOVE[i][j] = d.getUDSlicePerm();
    }
  }
}