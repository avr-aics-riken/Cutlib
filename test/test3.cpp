/*
 * test3 配列インタフェース(セル中心版/ノード間版)テストプログラム
 */
#include "test3Config.h"
#include "test3Check.h"

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

  Test3Config conf(argv[1]);

  cout << "TEST3" << endl;
  cout << "ndim: " << conf.ndim[0] << " " << conf.ndim[1] << " " << conf.ndim[2] << endl;
  cout << "org:  " << conf.org[0] << " " << conf.org[1] << " " << conf.org[2] << endl;
  cout << "d:    " << conf.d[0] << " " << conf.d[1] << " " << conf.d[2] << endl;

  int nerror = 0;

  Polylib* pl = Polylib::get_instance();
  pl->load(conf.xmlFile);
  cout << endl;
  pl->show_group_hierarchy();

  size_t n, n1;
  size_t ndim1[3] = {conf.ndim[0]+1, conf.ndim[1]+ 1, conf.ndim[2]+1};

  n = conf.ndim[0] * conf.ndim[1] * conf.ndim[2];
  n1 = ndim1[0] * ndim1[1] * ndim1[2];

  cout << endl;
  cout << "Check for CutPos32 and CutBid8" << endl;
  CutPos32Array *cutPos32Array;
  CutBid8Array  *cutBid8Array;
  CutPos32 *cutPos32;
  CutBid8 *cutBid8;

  cout << "Calc on cells ... " << endl;
  cutPos32Array = new CutPos32Array(conf.ndim);
  cutBid8Array = new CutBid8Array(conf.ndim);
  cutPos32 = new CutPos32[n];
  cutBid8 = new CutBid8[n];

  CutInfoCell(conf.org, conf.d, pl, cutPos32Array, cutBid8Array);
  CutInfoCell(conf.ndim, conf.org, conf.d, pl, cutPos32, cutBid8);

  if (!checkCutInfo(cutPos32Array, cutBid8Array, cutPos32, cutBid8)) nerror++;

  delete cutPos32Array;
  delete cutBid8Array;
  delete [] cutPos32;
  delete [] cutBid8;

  cout << "Calc on nodes ... " << endl;
  cutPos32Array = new CutPos32Array(ndim1);
  cutBid8Array = new CutBid8Array(ndim1);
  cutPos32 = new CutPos32[n1];
  cutBid8 = new CutBid8[n1];

  CutInfoNode(conf.org, conf.d, pl, cutPos32Array, cutBid8Array);
  CutInfoNode(conf.ndim, conf.org, conf.d, pl, cutPos32, cutBid8);

  if (!checkCutInfo(cutPos32Array, cutBid8Array, cutPos32, cutBid8)) nerror++;

  delete cutPos32Array;
  delete cutBid8Array;
  delete [] cutPos32;
  delete [] cutBid8;

  cout << endl;
  cout << "Check for CutPos8 and CutBid5" << endl;
  CutPos8Array *cutPos8Array;
  CutBid5Array  *cutBid5Array;
  CutPos8 *cutPos8;
  CutBid5 *cutBid5;

  cout << "Calc on cells ... " << endl;
  cutPos8Array = new CutPos8Array(conf.ndim);
  cutBid5Array = new CutBid5Array(conf.ndim);
  cutPos8 = new CutPos8[n];
  cutBid5 = new CutBid5[n];

  CutInfoCell(conf.org, conf.d, pl, cutPos8Array, cutBid5Array);
  CutInfoCell(conf.ndim, conf.org, conf.d, pl, cutPos8, cutBid5);

  if (!checkCutInfo(cutPos8Array, cutBid5Array, cutPos8, cutBid5)) nerror++;

  delete cutPos8Array;
  delete cutBid5Array;
  delete [] cutPos8;
  delete [] cutBid5;

  cout << "Calc on nodes ... " << endl;
  cutPos8Array = new CutPos8Array(ndim1);
  cutBid5Array = new CutBid5Array(ndim1);
  cutPos8 = new CutPos8[n1];
  cutBid5 = new CutBid5[n1];

  CutInfoNode(conf.org, conf.d, pl, cutPos8Array, cutBid5Array);
  CutInfoNode(conf.ndim, conf.org, conf.d, pl, cutPos8, cutBid5);

  if (!checkCutInfo(cutPos8Array, cutBid5Array, cutPos8, cutBid5)) nerror++;

  delete cutPos8Array;
  delete cutBid5Array;
  delete [] cutPos8;
  delete [] cutBid5;


  return nerror;
}
