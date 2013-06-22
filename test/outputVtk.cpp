#include "outputVtk.h"

#include <iostream>
#include <vector>
using namespace std;

#include <cstdio>

typedef vector<Vec3f> VecPos;
typedef vector<int> VecBid;

namespace {

enum { X, Y, Z};

const char* Header1 = "# vtk DataFile Version 3.0\nCutlib test\nASCII\nDATASET POLYDATA\n";
const char* Header2 = "SCALARS bid int 1\nLOOKUP_TABLE defalut\n";


void extractData(const Vec3f center, const float d[3],
                 const float pos6[6], const BidType bid6[6],
                 VecPos& vecPosM, VecBid& vecBidM,
                 VecPos& vecPosP, VecBid& vecBidP)
{
  if (bid6[X_M] > 0) {
    vecBidM.push_back(bid6[X_M]); 
    vecPosM.push_back(center + Vec3f::xaxis() * (-d[X]*pos6[X_M]));
  }
  if (bid6[X_P] > 0) {
    vecBidP.push_back(bid6[X_P]); 
    vecPosP.push_back(center + Vec3f::xaxis() * (+d[X]*pos6[X_P]));
  }

  if (bid6[Y_M] > 0) {
    vecBidM.push_back(bid6[Y_M]); 
    vecPosM.push_back(center + Vec3f::yaxis() * (-d[Y]*pos6[Y_M]));
  }
  if (bid6[Y_P] > 0) {
    vecBidP.push_back(bid6[Y_P]); 
    vecPosP.push_back(center + Vec3f::yaxis() * (+d[Y]*pos6[Y_P]));
  }

  if (bid6[Z_M] > 0) {
    vecBidM.push_back(bid6[Z_M]); 
    vecPosM.push_back(center + Vec3f::zaxis() * (-d[Z]*pos6[Z_M]));
  }
  if (bid6[Z_P] > 0) {
    vecBidP.push_back(bid6[Z_P]); 
    vecPosP.push_back(center + Vec3f::zaxis() * (+d[Z]*pos6[Z_P]));
  }
}


void outputData(const string& file, VecPos& vecPos, VecBid& vecBid)
{
  cout << "output to " << file << endl;
  ofstream out(file.c_str());
  out << Header1;
  out << "POINTS " << vecPos.size() << " float" << endl;
  for (size_t i = 0; i < vecPos.size(); i++) {
    out << vecPos[i] << endl;
  }
  out << "POINT_DATA " << vecBid.size() << endl;
  out << Header2;
  for (size_t i = 0; i < vecBid.size(); i++) {
    out << vecBid[i] << endl;
  }
}

} /* namespace ANONYMOUS */


void outputVtk(const string& file, const int calcType,
               const float org[], const float d[],
               const CutPosArray* cp, const CutBidArray* cb)
{
  size_t nx = cp->getSizeX();
  size_t ny = cp->getSizeY();
  size_t nz = cp->getSizeZ();

  VecPos vecPosM, vecPosP;
  VecBid vecBidM, vecBidP;

  float org0[3];
  if (calcType == 0) {
    // cell
    org0[0] = org[0] + 0.5*d[0]; 
    org0[1] = org[1] + 0.5*d[1]; 
    org0[2] = org[2] + 0.5*d[2]; 
  } else {
    // node
    org0[0] = org[0]; 
    org0[1] = org[1]; 
    org0[2] = org[2]; 
  }

  for (size_t k = 0; k < nz; k++)
    for (size_t j = 0; j < ny; j++)
      for (size_t i = 0; i < nx; i++) {
        Vec3f center(org0[0]+i*d[0], org0[1]+j*d[1], org0[2]+k*d[2]);

        float pos6[6];
        BidType bid6[6];
        cp->getPos(i,j,k,pos6);
        cb->getBid(i,j,k,bid6);
#if 0
        for (int direct = 0; direct < 6; direct++) {
          cout.setf(ios::showpoint);
          cout << " (" << (int)bid6[direct] << "," << pos6[direct] << ")";
        }
        cout << endl;
#endif
        extractData(center, d, pos6, bid6, vecPosM, vecBidM, vecPosP, vecBidP);
      }

  string fileM = file + "_m.vtk";
  string fileP = file + "_p.vtk";
  cout << endl;
  outputData(fileM, vecPosM, vecBidM);
  outputData(fileP, vecPosP, vecBidP);
  
}


#ifdef CUTLIB_OCTREE

void outputVtkLeafCell(const string& file, 
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

    extractData(center, d, pos6, bid6, vecPosM, vecBidM, vecPosP, vecBidP);
  }

  string fileM = file + "_m.vtk";
  string fileP = file + "_p.vtk";
  cout << endl;
  outputData(fileM, vecPosM, vecBidM);
  outputData(fileP, vecPosP, vecBidP);
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

  float pos6[6];
  BidType bid6[6];
  cp->getPos(pos6);
  cb->getBid(bid6);

  extractData(center, d, pos6, bid6,
              vecPosM[level], vecBidM[level], vecPosP[level], vecBidP[level]);


  if (cell->hasChild()) {
    for (TdPos pos = 0; pos < 8; pos++) {
      SklCell* childCell = cell->GetChildCell(pos);
      extractDataTreeCell(childCell, cp, cb, vecPosM, vecBidM, vecPosP, vecBidP);
    }
  }
}


void outputVtkAllCell(const string& file, 
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

  cout << endl;
  for (int level = 0; level <= treeLevel; level++) {
    char levelStr[5];
    sprintf(levelStr, "_L%02d", level);

    string fileM = file + levelStr + "_m.vtk";
    string fileP = file + levelStr + "_p.vtk";
    outputData(fileM, vecPosM[level], vecBidM[level]);
    outputData(fileP, vecPosP[level], vecBidP[level]);
  }

  delete [] vecPosM;
  delete [] vecPosP;
  delete [] vecBidM;
  delete [] vecBidP;
}

#endif //CUTLIB_OCTREE
