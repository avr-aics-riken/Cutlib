#ifndef TEST3CHECK_H
#define TEST3CHECK_H

#include <iostream>
using namespace std;

#include "Cutlib.h"
using namespace cutlib;

template<typename CUT_POS, typename CUT_BID>
bool checkCutInfo(const CutPosArrayTemplate<CUT_POS>* cpArray,
                  const CutBidArrayTemplate<CUT_BID>* cbArray,
                  const CUT_POS* cp, const CUT_BID* cb)
{
  size_t nx = cpArray->getSizeX();
  size_t ny = cpArray->getSizeY();
  size_t nz = cpArray->getSizeZ();

  int nerror_pos = 0;
  int nerror_bid = 0;

  for (size_t k = 0; k < nz; k++)
    for (size_t j = 0; j < ny; j++)
      for (size_t i = 0; i < nx; i++) {
        size_t ijk = i + j * nx + k * nx*ny;
        for (int d = 0; d < 6; d++) {
       // if (GetCutPos(cp[ijk], d) !=  cpArray->getPos(i,j,k,d)) nerror_pos++;
       // if (GetCutBid(cb[ijk], d) !=  cbArray->getBid(i,j,k,d)) nerror_bid++;
          if (GetCutPos(cp,ijk,d) !=  cpArray->getPos(i,j,k,d)) nerror_pos++;
          if (GetCutBid(cb,ijk,d) !=  cbArray->getBid(i,j,k,d)) nerror_bid++;
        }
      }

  cout << "# of CutPos errors = " << nerror_pos << endl;
  cout << "# of CutBid errors = " << nerror_bid << endl;
  
  if (nerror_pos == 0 && nerror_bid == 0) {
    cout << "OK." << endl;
    return true;
  } else {
    cout << "NG." << endl;
    return true;
  }
}

#endif /* TEST3CHECK_H */
