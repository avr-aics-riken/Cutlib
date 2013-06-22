/*
 * test1 セル中心版/ノード間版テストプログラム
 */
#include "test1Config.h"
#include "outputVtk.h"

#include <iostream>
using namespace std;

#include "Cutlib.h"
using namespace cutlib;

void checkGroup(PolygonGroup *pg, Boundaries* bList)
{
  std::vector<PolygonGroup *>& childGroups = pg->get_children();
  if (childGroups.size() == 0) {
    bList->push_back(Boundary(pg->acq_fullpath(), pg->get_id()));
    cout << "add boundary: name:" << pg->acq_fullpath()
         << " id:" << pg->get_id() << endl;
    return;
  }
  
  std::vector<PolygonGroup*>::iterator it;
  for (it = childGroups.begin(); it != childGroups.end(); it++) {
    checkGroup(*it, bList);
  }
}

Boundaries* createBoundaryList(Polylib* pl)
{
  Boundaries* bList = new Boundaries;
  std::vector<PolygonGroup *>* rootGroups = pl->get_root_groups();
  std::vector<PolygonGroup*>::iterator it = rootGroups->begin();;

  for (it = rootGroups->begin(); it != rootGroups->end(); it++) {
    checkGroup(*it, bList);
  }

  delete rootGroups;

  return bList;
}


int main(int argc, char* argv[])
{
#if 1
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
  cout << "Polygon groups:" << endl;
  for (size_t i = 0; i < conf.stlFiles.size(); i++) {
    cout << "  " << i+1 << ": name=" << Test1Config::file2group(conf.stlFiles[i])
         << " file=" << conf.stlFiles[i] << endl;
  }
#endif
//Polylib* pl = new Polylib;
  Polylib* pl = Polylib::get_instance();
//Boundaries* bList = new Boundaries;

//for (size_t i = 0; i < conf.stlFiles.size(); i++) {
//  string groupName = Test1Config::file2group(conf.stlFiles[i]);
//  pl->make_group("", groupName,  conf.stlFiles[i]);
//  bList->push_back(Boundary(groupName, i+1));
//}
//pl->load("small.xml");
  int r;
  if ((r = pl->load("small.xml")) != PLSTAT_OK) {
    cout << "ERROR in load() : " << r << endl;
    exit(1);
  }
//bList->push_back(Boundary("root/testA", 1));
//bList->push_back(Boundary("root/testB", 2));

  cout << endl;
  pl->show_group_hierarchy();

  Boundaries* bList = createBoundaryList(pl);
//std::vector<PolygonGroup *>* rootGroups = pl->get_root_groups();
//cout << "SIZE = " << rootGroups->size() << endl;




//pl->make_group_tree();
//pl->imports();
#if 1
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
    cout << "CutInfoCell: ";
    ret = CutInfoCell(conf.ista, conf.nlen, conf.org, conf.d,
                        pl, bList, cutPosArray, cutBidArray);
    cout << "return code = " << ret << endl;
  } else {
    cout << "CutInfoNode: ";
    ret = CutInfoNode(conf.ista, conf.nlen, conf.org, conf.d,
                        pl, bList, cutPosArray, cutBidArray);
    cout << "return code = " << ret << endl;
  }
  if (ret != 0) return ret;

  outputVtk("test1", conf.calcType, conf.org, conf.d, cutPosArray, cutBidArray);

  delete cutPosArray;
  delete cutBidArray;
//delete pl;
#endif
  delete bList;
  return 0;
}
