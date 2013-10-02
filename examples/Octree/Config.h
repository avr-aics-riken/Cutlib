#ifndef CONFIG_H
#define CONFIG_H

#include "ConfigBase.h"
#include <iostream>
#include <string>
#include "Tuple.h"

class Config : public ConfigBase {

public:

  bool leafCellOnly;

  size_t nRootCell[3];

  int nLevel;

  int dIndex;

  std::string cutPosType;
  std::string cutBidType;

  std::string polylibConf;

  std::string output;

private:

  void parse() {

    leafCellOnly = read<bool>("leafCellOnly", true);


    nLevel = read<int>("nLevel");

    dIndex = read<int>("dIndex");

    Tuple<int,3> nRootCell_t;
    nRootCell_t = read<Tuple<int,3> >("nRootCell");

    for (int i = 0; i < 3; i++) {
      nRootCell[i] = nRootCell_t[i];
    }

    cutPosType = read<std::string>("cutPos");
    cutBidType = read<std::string>("cutBid");

    polylibConf = read<std::string>("polylibConf");

    output = read<std::string>("output", "");

  }


  bool validate() {
    bool ret = true;

    if (!(nRootCell[0] > 0 && nRootCell[1] > 0 && nRootCell[2] > 0)) {
      std::cout << "error: 'nRootCell[3]' must be greater than 0." << std::endl;
      ret = false;
    }
    if (nLevel < 0) {
      std::cout << "error: 'nLevel' must be greater than or equal to 0." << std::endl;
      ret = false;
    }
    if (dIndex < 0) {
      std::cout << "error: 'dIndex' must be greater or equaul to 0." << std::endl;
      ret = false;
    }
    if (!(cutPosType == "CutPos32" || cutPosType == "CutPos8")) {
      std::cout << "error: 'cutPos' must be 'CutPos32' or 'CutPos8'." << std::endl;
      ret = false;
    }
    if (!(cutBidType == "CutBid8" || cutBidType == "CutBid5")) {
      std::cout << "error: 'cutBid' must be 'CutBid8' or 'CutBid5'." << std::endl;
      ret = false;
    }

    return ret;
  }

public:

  void print() const {
    std::cout.setf(std::ios::showpoint);
    std::cout << "  leafCellOnly: " << (leafCellOnly ? "true" : "false") << std::endl;
    std::cout << "  nRootCell:    " << Tuple<size_t,3>(nRootCell) << std::endl;
    std::cout << "  nLevel:       " << nLevel << std::endl;
    std::cout << "  dIndex:       " << dIndex << std::endl;
    std::cout << "  cutPos:       " << cutPosType << std::endl;
    std::cout << "  cutBid:       " << cutBidType << std::endl;
    std::cout << "  polylibConf:  " << polylibConf << std::endl;
    std::cout << "  output:       " << output << std::endl;
  }

};



#endif // CONFIG_H
