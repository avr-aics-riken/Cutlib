/*
 * test1 セル中心版/ノード間版テストプログラム
 */
#include "test1Config.h"
#include "outputVtk.h"

#include <iostream>
using namespace std;

#include "Cutlib.h"
using namespace cutlib;


int main(int argc, char* argv[])
{
  if (argc != 2) {
    cout << "usage: " << argv[0] << " configfile" << endl;
    return 1;
  }

  Test1Config conf(argv[1]);

  cout << "TEST1" << endl;
  cout << "Type of CutInfo calculatin: "
       << (conf.calcType == 0 ? "on cells" : "on nodes") << endl;
  cout << "CutPos type: " << (conf.cutPosType == 0 ? "CutPos32" : "CutPos8") << endl;
  cout << "CutBid type: " << (conf.cutBidType == 0 ? "CutBid8" : "CutBid5") << endl;
  cout << "ndim: " << conf.ndim[0] << " " << conf.ndim[1] << " " << conf.ndim[2] << endl;
  cout << "ista: " << conf.ista[0] << " " << conf.ista[1] << " " << conf.ista[2] << endl;
  cout << "nlen: " << conf.nlen[0] << " " << conf.nlen[1] << " " << conf.nlen[2] << endl;
  cout << "org:  " << conf.org[0] << " " << conf.org[1] << " " << conf.org[2] << endl;
  cout << "d:    " << conf.d[0] << " " << conf.d[1] << " " << conf.d[2] << endl;

  Polylib* pl = Polylib::get_instance();
  pl->load(conf.xmlFile);

  cout << endl;
  pl->show_group_hierarchy();

  if (conf.calcType != 0) {
    // calc on nodes
    conf.ndim[0] += 1;
    conf.ndim[1] += 1;
    conf.ndim[2] += 1;
  }

  CutPosArray *cutPosArray;
  CutBidArray *cutBidArray;
  if (conf.cutPosType == 0) cutPosArray = new CutPos32Array(conf.ndim);
  else                      cutPosArray = new CutPos8Array(conf.ndim);
  
  if (conf.cutBidType == 0) cutBidArray = new CutBid8Array(conf.ndim);
  else                      cutBidArray = new CutBid5Array(conf.ndim);
  
  cout << endl;
  int ret;
  if (conf.calcType == 0) {
    cout << "CutInfoCell: " << endl;
    ret = CutInfoCell(conf.ista, conf.nlen, conf.org, conf.d,
                        pl, cutPosArray, cutBidArray);
    cout << "return code = " << ret << endl;
  } else {
    cout << "CutInfoNode: " << endl;
    ret = CutInfoNode(conf.ista, conf.nlen, conf.org, conf.d,
                        pl, cutPosArray, cutBidArray);
    cout << "return code = " << ret << endl;
  }
  if (ret != 0) return ret;

  outputVtk("test1", conf.calcType, conf.org, conf.d, cutPosArray, cutBidArray);

  delete cutPosArray;
  delete cutBidArray;

  return 0;
}
