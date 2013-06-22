#include <iostream>
#include "Cutlib.h"
#include "GridAccessor/NUCell.h"
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
  pl->load(conf.polylibConf);
  pl->show_group_hierarchy();
  RepairPolygonData(pl);

  double px = 1.0 / conf.ndim[0];
  double py = 1.0 / conf.ndim[1];
  double pz = 1.0 / conf.ndim[2];

  double* x = new double[conf.ndim[0]+1];
  double* y = new double[conf.ndim[1]+1];
  double* z = new double[conf.ndim[2]+1];

  for (int i = 0; i < conf.ndim[0]; i++) {
    x[i] = (i*i) * (px*px);
  }
  for (int j = 0; j < conf.ndim[1]; j++) {
    y[j] = (j*j) * (py*py);
  }
  for (int k = 0; k < conf.ndim[2]; k++) {
    z[k] = (k*k) * (pz*pz);
  }
  x[conf.ndim[0]] = y[conf.ndim[1]] = z[conf.ndim[2]] = 1.0;

  GridAccessor* grid = new NUCell(conf.ndim, x, y, z);

  CutPosArray *cutPosArray;
  if (conf.cutPosType == "CutPos32") cutPosArray = new CutPos32Array(conf.ndim);
  if (conf.cutPosType == "CutPos8")  cutPosArray = new CutPos8Array(conf.ndim);

  CutBidArray *cutBidArray;
  if (conf.cutBidType == "CutBid8") cutBidArray = new CutBid8Array(conf.ndim);
  if (conf.cutBidType == "CutBid5") cutBidArray = new CutBid5Array(conf.ndim);

  std::cout << std::endl << "CalcCutInfo: " << std::endl;
  int ret = CalcCutInfo(conf.ista, conf.nlen, 
                        grid, pl, cutPosArray, cutBidArray);
  std::cout << "return code = " << ret <<  std::endl;

  if (conf.output != "") {
    outputVtk(conf.output, grid, cutPosArray, cutBidArray);
  }

  delete grid;
  delete cutPosArray;
  delete cutBidArray;

  return 0;
}
