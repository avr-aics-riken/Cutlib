#include <iostream>
#include "Cutlib.h"
#include "outputVtk.h"
using namespace cutlib;

#include "Config.h"

int main(int argc, char* argv[])
{
  if (argc != 2) {
    std::cout << "usage: " << argv[0] << " configfile" << std::endl;
    return 1;
  }

  Config conf;
  std::cout << std::endl << "Read config file: " << argv[1] << std::endl;
  conf.load(argv[1]);
  conf.print();

  std::cout << std::endl << "Polylib setting: " << conf.polylibConf << std::endl;
  Polylib* pl = Polylib::get_instance();
  if (pl->load(conf.polylibConf)) return 1;
  pl->show_group_hierarchy();
  RepairPolygonData(pl);

  CutPosOctree *cutPos;
  if (conf.cutPosType == "CutPos32") cutPos= new CutPos32Octree(conf.dIndex);
  if (conf.cutPosType == "CutPos8")  cutPos= new CutPos8Octree(conf.dIndex);

  CutBidOctree *cutBid;
  if (conf.cutBidType == "CutBid8") cutBid= new CutBid8Octree(conf.dIndex+cutPos->getSizeInFloat());
  if (conf.cutBidType == "CutBid5") cutBid= new CutBid5Octree(conf.dIndex+cutPos->getSizeInFloat());

  unsigned dLen = conf.dIndex + cutPos->getSizeInFloat() + cutBid->getSizeInFloat();

  SklTree* tree = new SklTree;
  tree->CreateTree(conf.nRootCell, conf.nLevel, dLen);

  float org[3] = { 0.0, 0.0, 0.0 };
  float d[3] = {
    1.0 / conf.nRootCell[0],
    1.0 / conf.nRootCell[1],
    1.0 / conf.nRootCell[2],
  };
  tree->SetPitch(org, d);

  std::cout << std::endl;
  int ret;
  if (conf.leafCellOnly) {
    std::cout << "CalcCutInfoOctreeLeafCell: " << std::endl;
    ret = CalcCutInfoOctreeLeafCell(tree, pl, cutPos, cutBid);
  } else {
    std::cout << "CalcCutInfoOctreeAllCell: " << std::endl;
    ret = CalcCutInfoOctreeAllCell(tree, pl, cutPos, cutBid);
//  ret = CalcCutInfoOctreeAllCell0(tree, pl, cutPos, cutBid);
  }
  std::cout << "return code = " << ret << std::endl;
  if (ret != 0) return ret;

  if (conf.output != "") {
    if (conf.leafCellOnly) {
      outputVtkLeafCell(conf.output, tree, cutPos, cutBid);
    } else {
      outputVtkAllCell(conf.output, tree, conf.nLevel, cutPos, cutBid);
    }
  }

  delete cutPos;
  delete cutBid;
  delete tree;

  return 0;
}
