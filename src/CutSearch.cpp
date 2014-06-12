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

void Split(double &ah, double &al, double a)
{
	double c = (1 + 1 << 27) * a;
	double d = c - a;
	ah = c - d;
	al = a - ah;
}

void TwoSum(double &x, double &y, double a, double b)
{
	x = a + b;
	double c = x - a;
	y = (a - (x - c)) + (b - c);
}

void TwoSum2(double &x, double &y, double a, double b)
{
	x = a + b;

	if ( fabs(a) >= fabs(b) )
	{
		y = b - (x - a);
	}
	else
	{
		y = a - (x - b);
	}
}

void TwoDiff(double &x, double &y, double a, double b)
{
	x = a - b;
	double c = x - a;
	y = (a - (x - c)) - (b + c);
}

void TwoProduct(double &x, double &y, double a, double b)
{
	x = a * b;
	double ah = 0.0;
	double al = 0.0;
	double bh = 0.0;
	double bl = 0.0;
	Split(ah, al, a);
	Split(bh, bl, b);
	double e1 = x - ah * bh;
	double e2 = e1 - al * bh;
	double e3 = e2 - ah * bl;
	y = al * bl - e3;
}

void TwoProductFMA(double &x, double &y, double a, double b)
{
	x = a * b;
	y = a * b - x;
}

void VecSum(double *q, double *p, int N)
{
	double pi = p[0];
	double pi_ = 0.0;

	for (int n = 1; n < N; n++)
	{
		TwoSum(pi_, q[n - 1], pi, p[n]);
		pi = pi_;
	}

	q[N - 1] = pi;
}

double SumK(double *p, int N, int K)
{
	if ( K < 0 )
	{
		return 0.0;
	}

	double *q = new double [N];

	for (int n = 0; n < N; n++)
	{
		q[n] = 0.0;
	}

	for (int k = 0; k < K; k++)
	{
		VecSum(q, p, N);

		for (int n = 0; n < N; n++)
		{
			p[n] = q[n];
			q[n] = 0.0;
		}
	}

	double sum = 0.0;

	for (int n = 0; n < N; n++)
	{
		sum += p[n];
	}

	delete [] q;

	return sum;
}

double Det1(double ax, double ay, double bx, double by, double cx, double cy)
{
	double det1 = (ax - cx) * (by - cy) - (ay - cy) * (bx - cx);
	return det1;
}

double Det2(double ax, double ay, double bx, double by, double cx, double cy)
{
	double t1 = 0.0;
	double e1 = 0.0;
	TwoDiff(t1, e1, ax, cx);

	double t2 = 0.0;
	double e2 = 0.0;
	TwoDiff(t2, e2, by, cy);

	double t3 = 0.0;
	double e3 = 0.0;
	TwoDiff(t3, e3, ay, cy);

	double t4 = 0.0;
	double e4 = 0.0;
	TwoDiff(t4, e4, bx, cx);

	double p1 = 0.0;
	double p3 = 0.0;
	TwoProduct(p1, p3, t1, t2);

	double p2 = 0.0;
	double p4 = 0.0;
	TwoProduct(p2, p4, -t3, t4);

	double p5 = 0.0;
	double p9 = 0.0;
	TwoProduct(p5, p9, t1, e2);

	double p6 = 0.0;
	double p10 = 0.0;
	TwoProduct(p6, p10, t2, e1);

	double p7 = 0.0;
	double p11 = 0.0;
	TwoProduct(p7, p11, -t3, e4);

	double p8 = 0.0;
	double p12 = 0.0;
	TwoProduct(p8, p12, -t4, e3);

	double p13 = 0.0;
	double p15 = 0.0;
	TwoProduct(p13, p15, e1, e2);

	double p14 = 0.0;
	double p16 = 0.0;
	TwoProduct(p14, p16, -e3, e4);

	const int N = 16;
/*
	double p[N] =
	{
		p1, p2, p3, p4,
		p5, p6, p7, p8,
		p9, p10, p11, p12,
		p13, p14, p15, p16,
	};
*/
	double p[N] =
	{
		p16, p15, p14, p13,
		p12, p11, p10, p9,
		p8, p7, p6, p5,
		p4, p3, p2, p1,
	};

	double det = SumK(p, N, 2);

	return det;
}

double Det(double ax, double ay, double bx, double by, double cx, double cy)
{
	double det = Det1(ax, ay, bx, by, cx, cy);
	double e = pow(2.0, -53.0);
	double e1 = fabs( (ax - cx) * (by - cy) ) + fabs( (ay - cy) * (bx - cx) );
	double err_max = (3.0*e + 16.0*e*e) * e1;
	if( fabs(det) < err_max )
	{
		double det2 = Det2(ax, ay, bx, by, cx, cy);

/*
		std::cout.setf(std::ios::scientific);
		std::cout.precision(20);

		std::cout << det2 << " " << det << std::endl;
*/

		return det2;
	}

	return det;
}

double Det(double a[], double b[], double c[], double d[]) {
	double t11 = a[0] - d[0];
	double t12 = a[1] - d[1];
	double t13 = a[2] - d[2];
	double t21 = b[0] - d[0];
	double t22 = b[1] - d[1];
	double t23 = b[2] - d[2];
	double t31 = c[0] - d[0];
	double t32 = c[1] - d[1];
	double t33 = c[2] - d[2];

	double det = t13*(t21*t32 - t31*t22) - t23*(t11*t32 - t31*t12) + t33*(t11*t22 - t21*t12);
	double aa = fabs(t13)*(fabs(t21*t32) + fabs(t31*t22));
	double ab = fabs(t23)*(fabs(t11*t32) + fabs(t31*t12));
	double ac = fabs(t33)*(fabs(t11*t22) + fabs(t21*t12));
	double e = pow(2.0, -53.0);
	double err_max = (7.0*e + 76.0*e*e)*(aa + ab + ac);
	if( fabs(det) < err_max )
	{
		double det2d_0 = Det(a[1], a[2], b[1], b[2], c[1], c[2]);
		double det2d_1 = Det(a[2], a[0], b[2], b[0], c[2], c[0]);
		double det2d_2 = Det(a[0], a[1], b[0], b[1], c[0], c[1]);
		double det1 = (b[0] - d[0])*det2d_0 + (b[1] - d[1])*det2d_1 + (b[2] - d[2])*det2d_2;

/*
		std::cout.setf(std::ios::scientific);
		std::cout.precision(20);

		std::cout << det;
		std::cout << " ";
		std::cout << err_max;
		std::cout << " ";
		std::cout << aa;
		std::cout << " ";
		std::cout << ab;
		std::cout << " ";
		std::cout << ac;
		std::cout << std::endl;

		std::cout << det2d_0;
		std::cout << " ";
		std::cout << det2d_1;
		std::cout << " ";
		std::cout << det2d_2;
		std::cout << " ";
		std::cout << det1;
		std::cout << " ";
		std::cout << det;
		std::cout << std::endl;
*/

		return det1;
	}

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

	double sv0 = Det(t0, t1, t2, p0);
	double sve = Det(t0, t1, t2, pe);
	double svw = Det(t0, t1, t2, pw);
	double svn = Det(t0, t1, t2, pn);
	double svs = Det(t0, t1, t2, ps);
	double svt = Det(t0, t1, t2, pt);
	double svb = Det(t0, t1, t2, pb);

	double A[3];
	double B = 0.0;
	CalcAB(A, B, t0, t1, t2);

	if( sv0*sve < 0 ) {
		double sv_12 = Det(p0, t1, t2, pe);
		double sv_01 = Det(p0, t0, t1, pe);
		double sv_20 = Det(p0, t2, t0, pe);
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
		double sv_12 = Det(p0, t1, t2, pw);
		double sv_01 = Det(p0, t0, t1, pw);
		double sv_20 = Det(p0, t2, t0, pw);
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
		double sv_12 = Det(p0, t1, t2, pn);
		double sv_01 = Det(p0, t0, t1, pn);
		double sv_20 = Det(p0, t2, t0, pn);
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
		double sv_12 = Det(p0, t1, t2, ps);
		double sv_01 = Det(p0, t0, t1, ps);
		double sv_20 = Det(p0, t2, t0, ps);
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
		double sv_12 = Det(p0, t1, t2, pt);
		double sv_01 = Det(p0, t0, t1, pt);
		double sv_20 = Det(p0, t2, t0, pt);
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
		double sv_12 = Det(p0, t1, t2, pb);
		double sv_01 = Det(p0, t0, t1, pb);
		double sv_20 = Det(p0, t2, t0, pb);
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
