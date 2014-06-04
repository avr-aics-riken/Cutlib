/// @file
/// @brief 交点情報計算クラス 実装
///

#include "CutSearch.h"
#include "TargetTriangle.h"

#ifdef CUTLIB_TIMING
#include "CutTiming.h"
#endif

namespace cutlib {

/// 最近接交点の探索.
///
///  @param[in] center 計算基準点座標
///  @param[in] range  6方向毎の計算基準線分の長さ
///  @param[out] pos6  交点座標値配列
///  @param[out] bid6  境界ID配列
///  @param[out] tri6  交点ポリゴンポインタ配列
///
///  @note pos6には計算基準線分長で規格化する前の値を格納
///
void CutSearch::search(const double center[], const double range[],
                       double pos6[], BidType bid6[],
                       Triangle* tri6[]) const
{
  Vec3r min(center[X]-range[X_M], center[Y]-range[Y_M], center[Z]-range[Z_M]);
  Vec3r max(center[X]+range[X_P], center[Y]+range[Y_P], center[Z]+range[Z_P]);

  clearCutInfo(range, pos6, bid6, tri6);

  std::vector<std::string>::const_iterator pg;
  for (pg = pgList->begin(); pg != pgList->end(); ++pg) {

#ifdef CUTLIB_TIMING
    Timer::Start(SEARCH_POLYGON);
#endif

    std::vector<Triangle*>* tList = pl->search_polygons(*pg, min, max, false);

#ifdef CUTLIB_TIMING
    Timer::Stop(SEARCH_POLYGON);
#endif

    std::vector<Triangle*>::const_iterator t;
    for (t = tList->begin(); t != tList->end(); ++t) {
      int exid = (*t)->get_exid();
      if (0 < exid && exid < 256) {
        BidType bid = exid;
        checkTriangle2(*t, bid, center, range, pos6, bid6, tri6);
      }
    } 

    delete tList;
  }
}


/// 三角形ポリゴンの交点調査.
///
///  @param[in] t      対象三角形ポリゴン
///  @param[in] bid    対象三角形ポリゴンの境界ID
///  @param[in] center 計算基準点座標
///  @param[in] range  6方向毎の計算基準線分の長さ
///  @param[in,out] pos6  交点座標値配列
///  @param[in,out] bid6  境界ID配列
///  @param[in,out] tri6  交点ポリゴンポインタ配列
///
///  @note pos6には計算基準線分長で規格化する前の値を格納
///
void CutSearch::checkTriangle(Triangle* t, BidType bid,
                              const double center[], const double range[],
                              double pos6[], BidType bid6[],
                              Triangle* tri6[])
{
  TargetTriangle triangle(t);
  double p, pos;

  if (triangle.intersectX(center[Y], center[Z], p)) {
    if (p >= center[X]) {
      pos = p - center[X];
      if (pos < pos6[X_P]) {
        pos6[X_P] = pos;
        bid6[X_P] = bid;
        tri6[X_P] = t;
      }
    }
    if (p <= center[X]) {
      pos = center[X] - p;
      if (pos < pos6[X_M]) {
        pos6[X_M] = pos;
        bid6[X_M] = bid;
        tri6[X_M] = t;
      }
    }
  }

  if (triangle.intersectY(center[Z], center[X], p)) {
    if (p >= center[Y]) {
      pos = p - center[Y];
      if (pos < pos6[Y_P]) {
        pos6[Y_P] = pos;
        bid6[Y_P] = bid;
        tri6[Y_P] = t;
      }
    }
    if (p <= center[Y]) {
      pos = center[Y]- p;
      if (pos < pos6[Y_M]) {
        pos6[Y_M] = pos;
        bid6[Y_M] = bid;
        tri6[Y_M] = t;
      }
    }
  }

  if (triangle.intersectZ(center[X], center[Y], p)) {
    if (p >= center[Z]) {
      pos = p - center[Z];
      if (pos < pos6[Z_P]) {
        pos6[Z_P] = pos;
        bid6[Z_P] = bid;
        tri6[Z_P] = t;
      }
    }
    if (p <= center[Z]) {
      pos = center[Z] - p;
      if (pos < pos6[Z_M]) {
        pos6[Z_M] = pos;
        bid6[Z_M] = bid;
        tri6[Z_M] = t;
      }
    }
  }

}

double SignedVolume(double a[], double b[], double c[], double d[]) {
	double t11 = a[0] - d[0];
	double t12 = a[1] - d[1];
	double t13 = a[2] - d[2];
	double t21 = b[0] - d[0];
	double t22 = b[1] - d[1];
	double t23 = b[2] - d[2];
	double t31 = c[0] - d[0];
	double t32 = c[1] - d[1];
	double t33 = c[2] - d[2];

	double det = t11*(t22*t33 - t23*t32) - t21*(t12*t33 - t13*t32) + t31*(t12*t23 - t13*t22);

	return det;
}

void CalcAB(double A[], double &B, double t0[], double t1[], double t2[]) {
	double a0[3];
	double b0[3];
	double a1[3];
	double b1[3];
	double a2[3];
	double b2[3];
	double c0[3];
	double c1[3];
	double c2[3];

	for(int i=0; i<3; i++) {
		a0[i] = t1[i] - t0[i];
		b0[i] = t2[i] - t0[i];
		a1[i] = t2[i] - t1[i];
		b1[i] = t0[i] - t1[i];
		a2[i] = t0[i] - t2[i];
		b2[i] = t1[i] - t2[i];
	}

	c0[0] = a0[1]*b0[2] - a0[2]*b0[1];
	c0[1] = a0[2]*b0[0] - a0[0]*b0[2];
	c0[2] = a0[0]*b0[1] - a0[1]*b0[0];
	c1[0] = a1[1]*b1[2] - a1[2]*b1[1];
	c1[1] = a1[2]*b1[0] - a1[0]*b1[2];
	c1[2] = a1[0]*b1[1] - a1[1]*b1[0];
	c2[0] = a2[1]*b2[2] - a2[2]*b2[1];
	c2[1] = a2[2]*b2[0] - a2[0]*b2[2];
	c2[2] = a2[0]*b2[1] - a2[1]*b2[0];

	B = 0.0;
	for(int i=0; i<3; i++) {
		A[i] = c0[i] + c1[i] + c2[i];
		B += c0[i]*t0[i] + c1[i]*t1[i] + c2[i]*t2[i];
	}
}

void CutSearch::checkTriangle2(Triangle* t, BidType bid,
                              const double center[], const double range[],
                              double pos6[], BidType bid6[],
                              Triangle* tri6[])
{
  TargetTriangle triangle(t);
  double p, pos;

  Vertex   n = t->get_normal();
  Vertex** v = t->get_vertex();
	double t0[3];
	double t1[3];
	double t2[3];
	double p0[3];
	double pe[3];
	double pw[3];
	double pn[3];
	double ps[3];
	double pb[3];
	double pt[3];
	for(int i=0; i<3; i++) {
		t0[i] = (*v[0])[i];
		t1[i] = (*v[1])[i];
		t2[i] = (*v[2])[i];
	}
	p0[0] = center[X];
	p0[1] = center[Y];
	p0[2] = center[Z];
	pe[0] = center[X] + range[X_P];
	pe[1] = center[Y];
	pe[2] = center[Z];
	pw[0] = center[X] - range[X_M];
	pw[1] = center[Y];
	pw[2] = center[Z];
	pn[0] = center[X];
	pn[1] = center[Y] + range[Y_P];
	pn[2] = center[Z];
	ps[0] = center[X];
	ps[1] = center[Y] - range[Y_M];
	ps[2] = center[Z];
	pt[0] = center[X];
	pt[1] = center[Y];
	pt[2] = center[Z] + range[Z_P];
	pb[0] = center[X];
	pb[1] = center[Y];
	pb[2] = center[Z] - range[Z_M];

	double sv0 = SignedVolume(t0, t1, t2, p0);
	double sve = SignedVolume(t0, t1, t2, pe);
	double svw = SignedVolume(t0, t1, t2, pw);
	double svn = SignedVolume(t0, t1, t2, pn);
	double svs = SignedVolume(t0, t1, t2, ps);
	double svt = SignedVolume(t0, t1, t2, pt);
	double svb = SignedVolume(t0, t1, t2, pb);

	double A[3];
	double B = 0.0;
	CalcAB(A, B, t0, t1, t2);

	if( sv0*sve < 0 ) {
		double sv_12 = SignedVolume(p0, t1, t2, pe);
		double sv_01 = SignedVolume(p0, t0, t1, pe);
		double sv_20 = SignedVolume(p0, t2, t0, pe);
		if( (sv_12 <= 0 && sv_01 <= 0 && sv_20 <= 0) || 
				(sv_12 >= 0 && sv_01 >= 0 && sv_20 >= 0) ) {
				double y = p0[Y];
				double z = p0[Z];
				double x = (B - A[Y]*y - A[Z]*z) / A[X];
	      pos = x - center[X];
				if (pos < pos6[X_P] && pos > 0) {
					pos6[X_P] = pos;
					bid6[X_P] = bid;
					tri6[X_P] = t;
				}
		}
	}

	if( sv0*svw < 0 ) {
		double sv_12 = SignedVolume(p0, t1, t2, pw);
		double sv_01 = SignedVolume(p0, t0, t1, pw);
		double sv_20 = SignedVolume(p0, t2, t0, pw);
		if( (sv_12 <= 0 && sv_01 <= 0 && sv_20 <= 0) || 
				(sv_12 >= 0 && sv_01 >= 0 && sv_20 >= 0) ) {
				double y = p0[Y];
				double z = p0[Z];
				double x = (B - A[Y]*y - A[Z]*z) / A[X];
				pos = center[X] - x;
				if (pos < pos6[X_M] && pos > 0) {
					pos6[X_M] = pos;
					bid6[X_M] = bid;
					tri6[X_M] = t;
				}
		}
	}

	if( sv0*svn < 0 ) {
		double sv_12 = SignedVolume(p0, t1, t2, pn);
		double sv_01 = SignedVolume(p0, t0, t1, pn);
		double sv_20 = SignedVolume(p0, t2, t0, pn);
		if( (sv_12 <= 0 && sv_01 <= 0 && sv_20 <= 0) || 
				(sv_12 >= 0 && sv_01 >= 0 && sv_20 >= 0) ) {
				double x = p0[X];
				double z = p0[Z];
				double y = (B - A[Z]*z - A[X]*x) / A[Y];
				pos = y - center[Y];
				if (pos < pos6[Y_P] && pos > 0) {
					pos6[Y_P] = pos;
					bid6[Y_P] = bid;
					tri6[Y_P] = t;
				}
		}
	}

	if( sv0*svs < 0 ) {
		double sv_12 = SignedVolume(p0, t1, t2, ps);
		double sv_01 = SignedVolume(p0, t0, t1, ps);
		double sv_20 = SignedVolume(p0, t2, t0, ps);
		if( (sv_12 <= 0 && sv_01 <= 0 && sv_20 <= 0) || 
				(sv_12 >= 0 && sv_01 >= 0 && sv_20 >= 0) ) {
				double x = p0[X];
				double z = p0[Z];
				double y = (B - A[Z]*z - A[X]*x) / A[Y];
				pos = center[Y]- y;
				if (pos < pos6[Y_M] && pos > 0) {
					pos6[Y_M] = pos;
					bid6[Y_M] = bid;
					tri6[Y_M] = t;
				}
		}
	}

	if( sv0*svt < 0 ) {
		double sv_12 = SignedVolume(p0, t1, t2, pt);
		double sv_01 = SignedVolume(p0, t0, t1, pt);
		double sv_20 = SignedVolume(p0, t2, t0, pt);
		if( (sv_12 <= 0 && sv_01 <= 0 && sv_20 <= 0) || 
				(sv_12 >= 0 && sv_01 >= 0 && sv_20 >= 0) ) {
				double x = p0[X];
				double y = p0[Y];
				double z = (B - A[X]*x - A[Y]*y) / A[Z];
				pos = z - center[Z];
				if (pos < pos6[Z_P] && pos > 0) {
					pos6[Z_P] = pos;
					bid6[Z_P] = bid;
					tri6[Z_P] = t;
				}
		}
	}

	if( sv0*svb < 0 ) {
		double sv_12 = SignedVolume(p0, t1, t2, pb);
		double sv_01 = SignedVolume(p0, t0, t1, pb);
		double sv_20 = SignedVolume(p0, t2, t0, pb);
		if( (sv_12 <= 0 && sv_01 <= 0 && sv_20 <= 0) || 
				(sv_12 >= 0 && sv_01 >= 0 && sv_20 >= 0) ) {
				double x = p0[X];
				double y = p0[Y];
				double z = (B - A[X]*x - A[Y]*y) / A[Z];
				pos = center[Z] - z;
				if (pos < pos6[Z_M] && pos > 0) {
					pos6[Z_M] = pos;
					bid6[Z_M] = bid;
					tri6[Z_M] = t;
				}
		}
	}


/*
  if (triangle.intersectX(center[Y], center[Z], p)) {
    if (p >= center[X]) {
      pos = p - center[X];
      if (pos < pos6[X_P]) {
        pos6[X_P] = pos;
        bid6[X_P] = bid;
        tri6[X_P] = t;
      }
    }
    if (p <= center[X]) {
      pos = center[X] - p;
      if (pos < pos6[X_M]) {
        pos6[X_M] = pos;
        bid6[X_M] = bid;
        tri6[X_M] = t;
      }
    }
  }

  if (triangle.intersectY(center[Z], center[X], p)) {
    if (p >= center[Y]) {
      pos = p - center[Y];
      if (pos < pos6[Y_P]) {
        pos6[Y_P] = pos;
        bid6[Y_P] = bid;
        tri6[Y_P] = t;
      }
    }
    if (p <= center[Y]) {
      pos = center[Y]- p;
      if (pos < pos6[Y_M]) {
        pos6[Y_M] = pos;
        bid6[Y_M] = bid;
        tri6[Y_M] = t;
      }
    }
  }

  if (triangle.intersectZ(center[X], center[Y], p)) {
    if (p >= center[Z]) {
      pos = p - center[Z];
      if (pos < pos6[Z_P]) {
        pos6[Z_P] = pos;
        bid6[Z_P] = bid;
        tri6[Z_P] = t;
      }
    }
    if (p <= center[Z]) {
      pos = center[Z] - p;
      if (pos < pos6[Z_M]) {
        pos6[Z_M] = pos;
        bid6[Z_M] = bid;
        tri6[Z_M] = t;
      }
    }
  }
*/

}


} // namespace cutlib
