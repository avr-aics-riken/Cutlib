/*
 * test2  Octree版テストプログラム
 */
#include "test2Config.h"
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

  Test2Config conf(argv[1]);

  cout << "TEST2" << endl;
  cout << "Type of CutInfo calculatin: "
       << (conf.calcType == 0 ? "only leaf cells" : "all cells") << endl;
  cout << "CutPos type: " << (conf.cutPosType == 0 ? "CutPos32" : "CutPos8") << endl;
  cout << "CutBid type: " << (conf.cutBidType == 0 ? "CutBid8" : "CutBid5") << endl;
  cout << "ndim: " << conf.ndim[0] << " " << conf.ndim[1] << " " << conf.ndim[2] << endl;
  cout << "treeLevel: " << conf.treeLevel << endl;
  cout << "dIndex: " << conf.dIndex << endl;
  cout << "org:  " << conf.org[0] << " " << conf.org[1] << " " << conf.org[2] << endl;
  cout << "d:    " << conf.d[0] << " " << conf.d[1] << " " << conf.d[2] << endl;

  Polylib* pl = Polylib::get_instance();
  pl->load(conf.xmlFile);
  cout << endl;
  pl->show_group_hierarchy();

  CutPosOctree *cutPos;
  CutBidOctree *cutBid;
  if (conf.cutPosType == 0) cutPos = new CutPos32Octree(conf.dIndex);
  else                      cutPos = new CutPos8Octree(conf.dIndex);
  
  if (conf.cutBidType == 0) cutBid = new CutBid8Octree(conf.dIndex+cutPos->getSizeInFloat());
  else                      cutBid = new CutBid5Octree(conf.dIndex+cutPos->getSizeInFloat());

  unsigned dLen = conf.dIndex + cutPos->getSizeInFloat() + cutBid->getSizeInFloat();

  SklTree* tree = new SklTree;
  tree->CreateTree(conf.ndim, conf.treeLevel, dLen);
  tree->SetPitch(conf.org, conf.d);

  cout << endl;
  int ret;
  if (conf.calcType == 0) {
    cout << "CutInfoOctreeLeafCell: " << endl;
    ret = CutInfoOctreeLeafCell(tree, pl, cutPos, cutBid);
    cout << "return code = " << ret << endl;
  } else {
    cout << "CutInfoOctreeAllCell: " << endl;
    ret = CutInfoOctreeAllCell(tree, pl, cutPos, cutBid);
//  ret = CutInfoOctreeAllCell0(tree, pl, cutPos, cutBid);
    cout << "return code = " << ret << endl;
  }
  if (ret != 0) return ret;


  if (conf.calcType == 0) {
    outputVtkLeafCell("test2", tree, cutPos, cutBid);
  } else {
    outputVtkAllCell("test2", tree, conf.treeLevel, cutPos, cutBid);
  }

  delete cutPos;
  delete cutBid;
  delete tree;

  return 0;
}
