#include "Cutlib.h"
using namespace cutlib;

#include <string>

void outputVtk(const std::string& file, const GridAccessor* grid,
               const CutPosArray* cp, const CutBidArray* cb,
               const CutNormalArray* cn = 0,
               bool reverseNormal = false);


#ifdef CUTLIB_OCTREE
void outputVtkLeafCell(const std::string& file, 
                       SklTree* tree,
                       CutPosOctree* cp, CutBidOctree* cb);

void outputVtkAllCell(const std::string& file, 
                       SklTree* tree, const int treeLevel,
                       CutPosOctree* cp, CutBidOctree* cb);
#endif
