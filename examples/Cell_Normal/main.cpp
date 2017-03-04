#include <iostream>
#include "Cutlib.h"
#include "GridAccessor/Cell.h"
#include "outputVtk.h"
using namespace cutlib;

#include "Config.h"

#ifdef _OPENMP
#include "omp.h"
#endif

int main(int argc, char* argv[])
{
  if (argc != 2) {
    std::cout << "usage: " << argv[0] << " configfile" << std::endl;
    return 1;
  }

#ifdef _OPENMP
  std::cout << "# of OpenMP threads = " << omp_get_max_threads() << std::endl;
#endif

  Config conf;
  std::cout << std::endl << "Read config file: " << argv[1] << std::endl;
  conf.load(argv[1]);
  conf.print();

  std::cout << std::endl << "Polylib setting: " << conf.polylibConf << std::endl;
  Polylib* pl = Polylib::get_instance();
  if (pl->load(conf.polylibConf)) return 1;
  pl->show_group_hierarchy();
  RepairPolygonData(pl);

  double org[3] = { 0.0, 0.0, 0.0 };
  double pitch[3] = { 1.0/conf.ndim[0], 1.0/conf.ndim[1], 1.0/conf.ndim[2] };
  GridAccessor* grid = new Cell(org, pitch);

  CutPosArray* cutPosArray;
  if (conf.cutPosType == "CutPos32") cutPosArray = new CutPos32Array(conf.ndim);
  if (conf.cutPosType == "CutPos8")  cutPosArray = new CutPos8Array(conf.ndim);

  CutBidArray* cutBidArray;
  if (conf.cutBidType == "CutBid8") cutBidArray = new CutBid8Array(conf.ndim);
  if (conf.cutBidType == "CutBid5") cutBidArray = new CutBid5Array(conf.ndim);

  CutNormalArray* cutNormalArray = new CutNormalArray(conf.ndim);

  std::cout << std::endl << "CalcCutInfo: " << std::endl;
  int ret = CalcCutInfo(conf.ista, conf.nlen,
                        grid, pl, cutPosArray, cutBidArray, cutNormalArray);
  std::cout << "return code = " << ret <<  std::endl;

  if (conf.output != "") {
    outputVtk(conf.output, grid, cutPosArray, cutBidArray,
              cutNormalArray, conf.reverseNormal);
  }

  delete grid;
  delete cutPosArray;
  delete cutBidArray;
  delete cutNormalArray;

  return 0;
}
