#include <iostream>
#include <cassert>
#include "CutTest.h"

using namespace cutlib;

CutInfoData::CutInfoData(const std::string& file)
{
  std::ifstream is(file.c_str());
  if (!is) {
    std::cout << "*** error: cannot open: " << file << std::endl;
    exit(1);
  }

  is >> sx >> sy >> sz >> std::ws;
  is >> ex >> ey >> ez >> std::ws;
  if (is.fail()) {
    std::cout << "*** error: cannnot read size parameters " << std::endl;
    exit(1);
  }
//std::cout << nx < ", " << ny << ", " << nz  << std::endl;

  size_t nx = ex - sx + 1;
  size_t ny = ey - sy + 1;
  size_t nz = ez - sz + 1;
  size_t n = nx * ny * nz;

//cutPos = new CutPos32Array(nx, ny, nz);
//cutBid = new CutBid8Array(nx, ny, nz);
  cutPos = new CutPos32Array(sx, sy, sz, ex, ey, ez);
  cutBid = new CutBid8Array(sx, sy, sz, ex, ey, ez);

//CutPos32* posData = cutPos->getDataPointer();
  CutPos32* posData = dynamic_cast<CutPos32Array*>(cutPos)->getDataPointer();
//CutBid8*  bidData = cutBid->getDataPointer();
  CutBid8*  bidData = dynamic_cast<CutBid8Array*>(cutBid)->getDataPointer();

  std::string buf;
  std::istringstream ist;
  while (getline(is, buf)) {
    ist.str(buf);
    ist.clear();
    Cut cut;
    ist >> cut;
    if (ist.fail()) {
      std::cout << "*** error: bad lne: " << buf << std::endl;
      exit(1);
    }
  //std::cout << "> " << cut << std::endl;
    assert(0 <= cut.ijk && cut.ijk < n);
    assert(0 <= cut.d && cut.d < 6);
    assert(0 < cut.bid && cut.bid < 256);
  //assert(0.0 <= cut.pos && cut.pos < 1.0);
    assert(0.0 <= cut.pos && cut.pos <= 1.0);
    SetCutBid(bidData[cut.ijk], cut.d, cut.bid);
    SetCutPos(posData[cut.ijk], cut.d, cut.pos);
  }

}



void CutInfoData::write(const std::string& file) const
{
  std::ofstream os(file.c_str());

  os << sx << " " << sy << " " << sz << std::endl;
  os << ex << " " << ey << " " << ez << std::endl;

  size_t nx = ex - sx + 1;
  size_t ny = ey - sy + 1;
  size_t nz = ez - sz + 1;

  for (int k = sz; k <= ez; k++) {
    for (int j = sy; j <= ey; j++) {
      for (int i = sx; i <= ex; i++) {
        size_t ijk = (i-sx) + (j-sy) * nx + (k-sz) * nx*ny;
        for (int d = 0; d < 6; d++) {
          BidType bid = cutBid->getBid(i,j,k,d);
          if (bid > 0) {
            float pos = cutPos->getPos(i,j,k,d);
          //assert(pos < 1.0);
            assert(pos <= 1.0);
            os << Cut(ijk, d, bid, pos) << std::endl;
          }
        }
      }
    }
  }
}


bool CutTest::compare(const CutInfoData& cutInfoData,
                      const CutInfoData& cutInfoData0, float tol)
{
  std::cout << "compare CutInfoData ..." << std::endl;

  int sx = cutInfoData.getStartX();
  int sy = cutInfoData.getStartY();
  int sz = cutInfoData.getStartZ();
  int sx0 = cutInfoData0.getStartX();
  int sy0 = cutInfoData0.getStartY();
  int sz0 = cutInfoData0.getStartZ();
  if (sx != sx0 || sy != sy0 || sz != sz0) {
    std::cout << "*** CutPos strat index = (" << sx << ", " << sy << ", " << sz << ")" << std::endl;
    std::cout << "                expected (" << sx0 << ", " << sy0 << ", " << sz0 << ")" << std::endl;
    std::cout << "... NG." << std::endl;
    return false;
  }

  int ex = cutInfoData.getEndX();
  int ey = cutInfoData.getEndY();
  int ez = cutInfoData.getEndZ();
  int ex0 = cutInfoData0.getEndX();
  int ey0 = cutInfoData0.getEndY();
  int ez0 = cutInfoData0.getEndZ();
  if (ex != ex0 || ey != ey0 || ez != ez0) {
    std::cout << "*** CutPos end index = (" << ex << ", " << ey << ", " << ez << ")" << std::endl;
    std::cout << "              expected (" << ex0 << ", " << ey0 << ", " << ez0 << ")" << std::endl;
    std::cout << "... NG." << std::endl;
    return false;
  }

  CutPosArray* cutPos = cutInfoData.getCutPosArray();
  CutBidArray* cutBid = cutInfoData.getCutBidArray();

  CutPosArray* cutPos0 = cutInfoData0.getCutPosArray();
  CutBidArray* cutBid0 = cutInfoData0.getCutBidArray();

  size_t nx = ex - sx + 1;
  size_t ny = ey - sy + 1;
  size_t nz = ez - sz + 1;

  size_t nDiff = 0;
  float errMax = 0.0;

  const int MaxDiffPrint = 200;

  for (size_t k = sz; k <= ez; k++) {
    for (size_t j = sy; j <= ey; j++) {
      for (size_t i = sx; i <= ex; i++) {
        size_t ijk = (i-sx) + (j-sy) * nx + (k-sz) * nx*ny;
        for (int d = 0; d < 6; d++) {
          BidType bid = cutBid->getBid(i,j,k,d);
          BidType bid0 = cutBid0->getBid(i,j,k,d);
          if (bid != bid0) {
            if (nDiff < MaxDiffPrint) {
              std::cout << "**diff: (" << i << "," << j << "," << k << "):"
                                     << d << std::endl;
              if (bid == 0) {
                std::cout << "    bid = 0" << std::endl;
              } else {
                std::cout << "    bid = " << (int)bid <<  ", pos = "
                          << cutPos->getPos(i,j,k,d) << std::endl;
              }
              if (bid0 == 0) {
                std::cout << "    bid0 = 0" << std::endl;
              } else {
                std::cout << "    bid0 = " << (int)bid0 <<  ", pos0 = "
                          << cutPos0->getPos(i,j,k,d) << std::endl;
              }
            }
            if (nDiff == MaxDiffPrint) {
              std::cout << "** ..." << std::endl;
            }
            nDiff++;
            continue;
          }
          if (bid > 0) {
            float pos = cutPos->getPos(i,j,k,d);
            float pos0 = cutPos0->getPos(i,j,k,d);
            float err = fabsf(pos - pos0);
            errMax = std::max(err, errMax);
          }

        }
      }
    }
  }

  std::cout << "# of bid difference = " << nDiff << std::endl;
  std::cout << "max error = " << errMax << std::endl;

  if (nDiff > 0 || errMax > tol) {
    std::cout << "... NG." << std::endl;
    return false;
  }
  
  std::cout << "... Good." << std::endl;
  return true;
}

