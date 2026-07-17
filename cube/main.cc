#include "include/kociemba.h"
#include <iostream>

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

int COMOVE[MAXCO][NMOVES1];
int EOMOVE[MAXEO][NMOVES1];
int SLICEMOVE[MAXSLICE][NMOVES1];

int main() {
    CubieCube c;

    std::cout << c.getCO() << " : CO\n";
    std::cout << c.getCP() << " : CP\n";
    std::cout << c.getEO() << " : EO\n";
    std::cout << c.getEP() << " : EP\n";
    std::cout << c.getSlice() << " : slice\n";
    //c.setUDSlice(1);
    std::cout << c.getUDSlicePerm() << " : UDslice\n";

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
    



    for (int i = 0; i < 24; i++) {
        c.setUDSlicePerm(i);
        if (c.getUDSlicePerm() != i) {
            std::cout << "WRONG at: " << c.getUDSlicePerm() << "---" << i << "\n";
        }
    }
    
    return 0;
}