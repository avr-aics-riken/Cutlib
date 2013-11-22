#ifndef CONFIG_H
#define CONFIG_H

#include "ConfigBase.h"
#include <iostream>
#include <string>
#include "Tuple.h"


class Config : public ConfigBase {

public:

  size_t ndim[3];

  int ista[3];
  size_t nlen[3];

  std::string cutPosType;
  std::string cutBidType;

  std::string polylibConf;

  std::string output;

  bool reverseNormal;

private:

  void parse() {

    Tuple<int,3> ndim_t;
    Tuple<int,3> ista_t;
    Tuple<int,3> nlen_t;

    ndim_t = read<Tuple<int,3> >("ndim");
    ista_t = read<Tuple<int,3> >("ista", 0);
  //nlen_t = read<Tuple<int,3> >("nlen", ndim_t);
    nlen_t = read<Tuple<int,3> >("nlen", ndim_t + 1);

    for (int i = 0; i < 3; i++) {
      ndim[i] = ndim_t[i];
      ista[i] = ista_t[i];
      nlen[i] = nlen_t[i];
    }

    cutPosType = read<std::string>("cutPos");
    cutBidType = read<std::string>("cutBid");

    polylibConf = read<std::string>("polylibConf");

    output = read<std::string>("output", "");

    reverseNormal = read<bool>("reverseNormal", false);

  }


  bool validate() {
    bool ret = true;

    if (!(ndim[0] > 0 && ndim[1] > 0 && ndim[2] > 0)) {
      std::cout << "error: 'ndim[3]' must be greater than 0." << std::endl;
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
    std::cout << "  ndim:           " << Tuple<size_t,3>(ndim) << std::endl;
    std::cout << "  ista:           " << Tuple<int,3>(ista) << std::endl;
    std::cout << "  nlen:           " << Tuple<size_t,3>(nlen) << std::endl;
    std::cout << "  cutPos:         " << cutPosType << std::endl;
    std::cout << "  cutBid:         " << cutBidType << std::endl;
    std::cout << "  polylibConf:    " << polylibConf << std::endl;
    std::cout << "  output:         " << output << std::endl;
    std::cout << "  reverse normal: " << (reverseNormal ? "on" : "off")  << std::endl;
  }

};



#endif // CONFIG_H
