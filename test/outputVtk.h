#include "Cutlib.h"
using namespace cutlib;

#include <fstream>
#include <string>
using std::string;

void outputVtk(const string& file, const int calcType,
               const float org[], const float d[],
               const CutPosArray* cp, const CutBidArray* cb);

#ifdef CUTLIB_OCTREE
void outputVtkLeafCell(const string& file, 
                       SklTree* tree,
                       CutPosOctree* cp, CutBidOctree* cb);

void outputVtkAllCell(const string& file, 
                       SklTree* tree, const int treeLevel,
                       CutPosOctree* cp, CutBidOctree* cb);
#endif
