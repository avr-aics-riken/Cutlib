#include "outputVtk.h"

#include <iostream>
#include <fstream>
#include <vector>

#include <cstdio>

typedef std::vector<Vec3f> VecPos;
typedef std::vector<int> VecBid;
typedef std::vector<Vec3f> VecNormal;

namespace {

enum { X, Y, Z};

const char* Header1 = "# vtk DataFile Version 3.0\nCutlib test\nASCII\nDATASET POLYDATA\n";
const char* Header2 = "SCALARS bid int 1\nLOOKUP_TABLE defalut\n";
const char* Header3 = "NORMALS normal float\n";


void extractData(const Vec3f center, const double range[6],
                 const float pos6[6], const BidType bid6[6],
                 VecPos& vecPosM, VecBid& vecBidM,
                 VecPos& vecPosP, VecBid& vecBidP)
{
  if (bid6[X_M] > 0) {
    vecBidM.push_back(bid6[X_M]); 
    vecPosM.push_back(center + Vec3f::xaxis() * (-range[X_M]*pos6[X_M]));
  }
  if (bid6[X_P] > 0) {
    vecBidP.push_back(bid6[X_P]); 
    vecPosP.push_back(center + Vec3f::xaxis() * (+range[X_P]*pos6[X_P]));
  }

  if (bid6[Y_M] > 0) {
    vecBidM.push_back(bid6[Y_M]); 
    vecPosM.push_back(center + Vec3f::yaxis() * (-range[Y_M]*pos6[Y_M]));
  }
  if (bid6[Y_P] > 0) {
    vecBidP.push_back(bid6[Y_P]); 
    vecPosP.push_back(center + Vec3f::yaxis() * (+range[Y_P]*pos6[Y_P]));
  }

  if (bid6[Z_M] > 0) {
    vecBidM.push_back(bid6[Z_M]); 
    vecPosM.push_back(center + Vec3f::zaxis() * (-range[Z_M]*pos6[Z_M]));
  }
  if (bid6[Z_P] > 0) {
    vecBidP.push_back(bid6[Z_P]); 
    vecPosP.push_back(center + Vec3f::zaxis() * (+range[Z_P]*pos6[Z_P]));
  }
}


void extractNormalData(const BidType bid6[6], const Normal normal6[6],
                 VecNormal& vecNormalM, VecNormal& vecNormalP)
{
  if (bid6[X_M] > 0) {
    vecNormalM.push_back(Vec3f(normal6[X_M][0],
                               normal6[X_M][1],
                               normal6[X_M][2]));
  }
  if (bid6[X_P] > 0) {
    vecNormalP.push_back(Vec3f(normal6[X_P][0],
                               normal6[X_P][1],
                               normal6[X_P][2]));
  }

  if (bid6[Y_M] > 0) {
    vecNormalM.push_back(Vec3f(normal6[Y_M][0],
                               normal6[Y_M][1],
                               normal6[Y_M][2]));
  }
  if (bid6[Y_P] > 0) {
    vecNormalP.push_back(Vec3f(normal6[Y_P][0],
                               normal6[Y_P][1],
                               normal6[Y_P][2]));
  }

  if (bid6[Z_M] > 0) {
    vecNormalM.push_back(Vec3f(normal6[Z_M][0],
                               normal6[Z_M][1],
                               normal6[Z_M][2]));
  }
  if (bid6[Z_P] > 0) {
    vecNormalP.push_back(Vec3f(normal6[Z_P][0],
                               normal6[Z_P][1],
                               normal6[Z_P][2]));
  }
}


void outputData(std::ofstream& out, VecPos& vecPos, VecBid& vecBid)
{
  out << Header1;
  out << "POINTS " << vecPos.size() << " float" << std::endl;
  for (size_t i = 0; i < vecPos.size(); i++) {
    out << vecPos[i] << std::endl;
  }
  out << "POINT_DATA " << vecBid.size() << std::endl;
  out << Header2;
  for (size_t i = 0; i < vecBid.size(); i++) {
    out << vecBid[i] << std::endl;
  }
}


void outputNormalData(std::ofstream& out, VecNormal& vecNormal)
{
  out << Header3;
  for (size_t i = 0; i < vecNormal.size(); i++) {
    out << vecNormal[i] << std::endl;
  }
}

} // namespace ANONYMOUS


void outputVtk(const std::string& file, const GridAccessor* grid,
               const CutPosArray* cp, const CutBidArray* cb,
               const CutNormalArray* cn,
               bool reverseNormal)
{
  int ista[3] = { 
    cp->getStartX(),
    cp->getStartY(),
    cp->getStartZ(),
  };
  size_t nlen[3] = { 
    cp->getSizeX(),
    cp->getSizeY(),
    cp->getSizeZ()
  };

  VecPos vecPosM, vecPosP;
  VecBid vecBidM, vecBidP;
  VecNormal vecNormalM, vecNormalP;

  for (int k = ista[2]; k < ista[2]+nlen[2]; k++) {
    for (int j = ista[1]; j < ista[1]+nlen[1]; j++) {
      for (int i = ista[0]; i < ista[0]+nlen[0]; i++) {
        float pos6[6];
        BidType bid6[6];
        Normal normal6[6];
        cp->getPos(i, j, k, pos6);
        cb->getBid(i, j, k, bid6);
        if (cn) cn->getNormal(i, j, k, normal6);

        double center_d[3];
        double range_d[6];
        grid->getSearchRange(i, j, k, center_d, range_d);

        Vec3f center(center_d[0], center_d[1], center_d[2]);
        extractData(center, range_d, pos6, bid6, vecPosM, vecBidM, vecPosP, vecBidP);
        if (cn) extractNormalData(bid6, normal6, vecNormalM, vecNormalP);
      }
    }
  }

  if (cn && reverseNormal) {
    for (size_t i = 0; i < vecNormalM.size(); i++) {
      vecNormalM[i] = - vecNormalM[i];
    }
    for (size_t i = 0; i < vecNormalP.size(); i++) {
      vecNormalP[i] = - vecNormalP[i];
    }
  }

  std::string fileM = file + "_m.vtk";
  std::string fileP = file + "_p.vtk";
  std::cout << std::endl;

  std::cout << "output to " << fileM << std::endl;
  std::ofstream outM(fileM.c_str());
  outputData(outM, vecPosM, vecBidM);
  if (cn) outputNormalData(outM, vecNormalM);

  std::cout << "output to " << fileP << std::endl;
  std::ofstream outP(fileP.c_str());
  outputData(outP, vecPosP, vecBidP);
  if (cn) outputNormalData(outP, vecNormalP);
  
}


#ifdef CUTLIB_OCTREE

void outputVtkLeafCell(const std::string& file, 
                       SklTree* tree,
                       CutPosOctree* cp, CutBidOctree* cb)
{
  VecPos vecPosM, vecPosP;
  VecBid vecBidM, vecBidP;

  for (SklCell* cell = tree->GetLeafCellFirst(); cell != 0;
       cell = tree->GetLeafCellNext(cell)) {

    cp->assignData(cell->GetData());
    cb->assignData(cell->GetData());

    float org[3], d[3];
    
    cell->GetOrigin(org[0], org[1], org[2]);
    cell->GetPitch(d[0], d[1], d[2]);
    Vec3f center(org[0]+0.5*d[0], org[1]+0.5*d[1], org[2]+0.5*d[2]);

    double range_d[6];
    range_d[X_M] = range_d[X_P] = d[0];
    range_d[Y_M] = range_d[Y_P] = d[1];
    range_d[Z_M] = range_d[Z_P] = d[2];

    float pos6[6];
    BidType bid6[6];
    cp->getPos(pos6);
    cb->getBid(bid6);
#if 0
    for (int direct = 0; direct < 6; direct++) {
      cout.setf(ios::showpoint);
      cout << " (" << (int)bid6[direct] << "," << pos6[direct] << ")";
    }
    cout << endl;
#endif
    extractData(center, range_d, pos6, bid6, vecPosM, vecBidM, vecPosP, vecBidP);
  }

  std::string fileM = file + "_m.vtk";
  std::string fileP = file + "_p.vtk";
  std::cout << std::endl;

  std::cout << "output to " << fileM << std::endl;
  std::ofstream outM(fileM.c_str());
  outputData(outM, vecPosM, vecBidM);

  std::cout << "output to " << fileP << std::endl;
  std::ofstream outP(fileP.c_str());
  outputData(outP, vecPosP, vecBidP);
}


void extractDataTreeCell(SklCell* cell, CutPosOctree* cp, CutBidOctree* cb,
                         VecPos vecPosM[], VecBid vecBidM[],
                         VecPos vecPosP[], VecBid vecBidP[])
{
  cp->assignData(cell->GetData());
  cb->assignData(cell->GetData());

  int level = cell->GetMyLevel();

  float org[3], d[3];
  cell->GetOrigin(org[0], org[1], org[2]);
  cell->GetPitch(d[0], d[1], d[2]);
  Vec3f center(org[0]+0.5*d[0], org[1]+0.5*d[1], org[2]+0.5*d[2]);

  double range_d[6];
  range_d[X_M] = range_d[X_P] = d[0];
  range_d[Y_M] = range_d[Y_P] = d[1];
  range_d[Z_M] = range_d[Z_P] = d[2];

  float pos6[6];
  BidType bid6[6];
  cp->getPos(pos6);
  cb->getBid(bid6);

  extractData(center, range_d, pos6, bid6,
              vecPosM[level], vecBidM[level], vecPosP[level], vecBidP[level]);

  if (cell->hasChild()) {
    for (TdPos pos = 0; pos < 8; pos++) {
      SklCell* childCell = cell->GetChildCell(pos);
      extractDataTreeCell(childCell, cp, cb, vecPosM, vecBidM, vecPosP, vecBidP);
    }
  }
}


void outputVtkAllCell(const std::string& file, 
                       SklTree* tree, const int treeLevel,
                       CutPosOctree* cp, CutBidOctree* cb)
{
  VecPos* vecPosM = new VecPos[treeLevel+1];
  VecPos* vecPosP = new VecPos[treeLevel+1];
  VecBid* vecBidM = new VecBid[treeLevel+1];
  VecBid* vecBidP = new VecBid[treeLevel+1];

  size_t nx, ny, nz;
  tree->GetSize(nx, ny, nz);
  for (size_t k = 0; k < nz; k++)
    for (size_t j = 0; j < ny; j++)
      for (size_t i = 0; i < nx; i++) {
        SklCell* cell = tree->GetRootCell(i, j, k);
        extractDataTreeCell(cell, cp, cb, vecPosM, vecBidM, vecPosP, vecBidP);
      }

  std::cout << std::endl;
  for (int level = 0; level <= treeLevel; level++) {
    char levelStr[5];
    sprintf(levelStr, "_L%02d", level);

    std::string fileM = file + levelStr + "_m.vtk";
    std::string fileP = file + levelStr + "_p.vtk";

    std::cout << "output to " << fileM << std::endl;
    std::ofstream outM(fileM.c_str());
    outputData(outM, vecPosM[level], vecBidM[level]);

    std::cout << "output to " << fileP << std::endl;
    std::ofstream outP(fileP.c_str());
    outputData(outP, vecPosP[level], vecBidP[level]);
  }

  delete [] vecPosM;
  delete [] vecPosP;
  delete [] vecBidM;
  delete [] vecBidP;
}

#endif //CUTLIB_OCTREE
