#ifndef CUT_TEST_H
#define CUT_TEST_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cassert>
#include "Cutlib.h"

namespace cutlib {

class CutInfoData {

  struct Cut {
    size_t ijk;
    int d;
    int bid;
    float pos;
  
    Cut() : ijk(0), d(0), bid(0), pos(1.0) {}
  
    Cut(size_t ijk, int d, BidType bid, float pos)
      : ijk(ijk), d(d), bid(bid), pos(pos) {}
  
    friend std::ostream& operator<<(std::ostream& os, const Cut& cut) {
      return os << cut.ijk << " " << cut.d << " " << cut.bid << " " << cut.pos;
    }
  
    friend std::istream& operator>>(std::istream& is, Cut& cut) {
      return is >> cut.ijk >> cut.d >> cut.bid >> cut.pos;
    }
  };

  int sx, sy, sz;
  int ex, ey, ez;

  CutPosArray* cutPos;
  CutBidArray* cutBid;

public:

  CutInfoData(CutPosArray* cutPos, CutBidArray* cutBid)
    : cutPos(cutPos), cutBid(cutBid) {
#if 0
    sx = sy = sz = 0;
    ex = cutPos->getSizeX() - 1;
    ey = cutPos->getSizeY() - 1;
    ez = cutPos->getSizeZ() - 1;
    assert (ex == cutBid->getSizeX() - 1);
    assert (ey == cutBid->getSizeY() - 1);
    assert (ez == cutBid->getSizeZ() - 1);
#else
    sx = cutPos->getStartX();
    sy = cutPos->getStartY();
    sz = cutPos->getStartZ();
    assert (sx == cutBid->getStartX());
    assert (sy == cutBid->getStartY());
    assert (sz == cutBid->getStartZ());
    ex = cutPos->getSizeX();
    ey = cutPos->getSizeY();
    ez = cutPos->getSizeZ();
    assert (ex == cutBid->getSizeX());
    assert (ey == cutBid->getSizeY());
    assert (ez == cutBid->getSizeZ());
    ex = ex + sx - 1;
    ey = ey + sy - 1;
    ez = ez + sz - 1;
#endif
  }

  CutInfoData(const std::string& file);

  void write(const std::string& file) const;

  int getStartX() const { return sx; }
  int getStartY() const { return sy; }
  int getStartZ() const { return sz; }

  int getEndX() const { return ex; }
  int getEndY() const { return ey; }
  int getEndZ() const { return ez; }

  CutPosArray* getCutPosArray() const { return cutPos; }

  CutBidArray* getCutBidArray() const { return cutBid; }

};


class CutTest {
  
public:

  // 2.0/256 = 0.0078125
  static bool compare(const CutInfoData& cutInfoData,
                      const CutInfoData& cutInfoData0, float tol = 2.0/256);


};

} // namespace cutlib

#endif // CUT_TEST_H
