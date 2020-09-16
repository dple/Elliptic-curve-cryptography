/*
 * algorithms.c
 *
 *  Created on: Apr 21, 2011
 *      Author: dple
 */

#include<stdio.h>
#include<NTL/ZZ.h>
#include<NTL/ZZ_p.h>
#include<NTL/ZZ_pE.h>

#include "algorithms.h"
#include "curvepoint_fp.h"
#include "curvepoint_fpk.h"
#include "curve.h"

NTL_CLIENT


ZZ_pE littleMillerCubic(pt_curve_fp P, pt_curve_fpk Q, ZZ r, ZZ_p b){

	pt_curve_fp T;
	pt_curve_fp A;
	pt_curve_fp tmp;
	ZZ_pE f1;
	ZZ_p lambda;
	ZZ_p x2, y2, z2, v, w, f, g, h, l, m, n, l0, l1, l2, l3, x3, y3, z3;
	int i, k;
	i = NumBits(r);
	T = P;
	f1 = 1;

	for(k = i-1; k >=2; k--){

		x2 = power(T.X, 2);	// X^2
		y2 = power(T.Y, 2); // Y^2
		z2 = power(T.Z, 2);	// Z^2
		v = b*z2;			// bZ^2
		w = 3*v;
		f = power((T.X + T.Y), 2) - x2 - y2;		// 2XY
		g = power((T.Y + T.Z), 2) - y2 - z2;	 	// 2YZ
		h = 3*w;

		// Calcule 2T
		tmp.X = f*(y2 - h);
		tmp.Y = power((b + h),2) - 3*power((2*w),2);
		tmp.Z = 4*y2*g;

		l0 = x2*(y2 - h); l1 = f*g; l2 = -3*x2*f; l3 = (y2 - v)*(y2 + v);
		f1 = power(f1, 2)*(l0*Q.X + l1*Q.X*Q.X + l2*Q.Y + l3);
		T = tmp;


		if (bit(r, k)){ // when the i-th bit is 1
			// T = T + P; mixed addition with Z_P = 1
			// # Assumptions: Z_P=1.

			x2 = T.X*P.Z; y2 = T.Y*P.Z; z2 = T.Z*P.Z;
			v = T.Z*P.X - x2; w = y2 - T.Z*P.Y; f = power(v, 2);
			g = power(w,2); h = -v*f;
			l = f*x2; m = h + z2*g - 2*l; n = z2*f*w;

			tmp.X = -v*m; tmp.Y = w*(l - m) - h*y2; tmp.Z = z2*h;
			x3 = power(tmp.X, 2); z3 = power(tmp.Z, 2); y3 = power((tmp.X + tmp.Z), 2)- x3 - z3;
			l1 = 2*z3; l0 = y3; l2 = -2*n*tmp.Z; l3 = 2*(x3 + n*tmp.Y);

			f1 = f1*(l0*Q.X + l1*Q.X*Q.X + l2*Q.Y + l3);	// 1M + 2km if (Q.X - P.X) and (Q.Y - P.Y) are precalculed
			T = tmp;

			// Cost ("mixed" addition step): 2M + (3km + 9)m + 2s
			// 								 1M + (k + 9)m + 2s for curves with k even
		}
	}

	x2 = power(T.X, 2);
	z2 = power(T.Z, 2);
	w = b*z2;
	l2 = 2*T.Y*T.Z;
	l3 = T.Y*l2;

	f1 = power(f1, 2)*(Q.Y*l2 - w*(Q.X - T.X) - l3);
	f1 = f1*(Q.X - P.X);

	return f1;
}


ZZ_pE fullMillerCubic(pt_curve_fpk P, pt_curve_fp Q, ZZ r, ZZ_p b){

	pt_curve_fpk T;
	pt_curve_fpk A;
	pt_curve_fpk tmp;
	ZZ_pE f1;

	ZZ_pE x2, y2, z2, v, w, f, g, h, l, m, n, l0, l1, l2, l3, x3, y3, z3;
	int i, k;
	i = NumBits(r);
	T = P;
	f1 = 1;

	for(k = i-1; k >=2; k--){

		x2 = power(T.X, 2);	// X^2
		y2 = power(T.Y, 2); // Y^2
		z2 = power(T.Z, 2);	// Z^2
		v = b*z2;			// bZ^2
		w = 3*v;
		f = power((T.X + T.Y), 2) - x2 - y2;		// 2XY
		g = power((T.Y + T.Z), 2) - y2 - z2;	 	// 2YZ
		h = 3*w;

		// Calcule 2T
		tmp.X = f*(y2 - h);
		tmp.Y = power((b + h),2) - 3*power((2*w),2);
		tmp.Z = 4*y2*g;

		l0 = x2*(y2 - h); l1 = f*g; l2 = -3*x2*f; l3 = (y2 - v)*(y2 + v);
		f1 = power(f1, 2)*(l0*Q.X + l1*Q.X*Q.X + l2*Q.Y + l3);
		T = tmp;


		if (bit(r, k)){ // when the i-th bit is 1
			// T = T + P; mixed addition with Z_P = 1
			// # Assumptions: Z_P=1.

			x2 = T.X*P.Z; y2 = T.Y*P.Z; z2 = T.Z*P.Z;
			v = T.Z*P.X - x2; w = y2 - T.Z*P.Y; f = power(v, 2);
			g = power(w,2); h = -v*f;
			l = f*x2; m = h + z2*g - 2*l; n = z2*f*w;

			tmp.X = -v*m; tmp.Y = w*(l - m) - h*y2; tmp.Z = z2*h;
			x3 = power(tmp.X, 2); z3 = power(tmp.Z, 2); y3 = power((tmp.X + tmp.Z), 2)- x3 - z3;
			l1 = 2*z3; l0 = y3; l2 = -2*n*tmp.Z; l3 = 2*(x3 + n*tmp.Y);

			f1 = f1*(l0*Q.X + l1*Q.X*Q.X + l2*Q.Y + l3);	// 1M + 2km if (Q.X - P.X) and (Q.Y - P.Y) are precalculed
			T = tmp;

			// Cost ("mixed" addition step): 2M + (3km + 9)m + 2s
			// 								 1M + (k + 9)m + 2s for curves with k even
		}
	}

	x2 = power(T.X, 2);
	z2 = power(T.Z, 2);
	w = b*z2;
	l2 = 2*T.Y*T.Z;
	l3 = T.Y*l2;

	f1 = power(f1, 2)*(Q.Y*l2 - w*(Q.X - T.X) - l3);
	f1 = f1*(Q.X - P.X);

	return f1;
}

ZZ_pE littleMiller(pt_curve_fp P, pt_curve_fpk Q, ZZ r, ZZ_p a){

	pt_curve_fp T;
	pt_curve_fp A;
	pt_curve_fp tmp;
	ZZ_pE f1;
	ZZ_p lambda;
	ZZ_p x2, z2, w, s, s2, s3, t, t2, b, h;
	int i, k;
	i = NumBits(r);
	T = P;
	f1 = 1;

	for(k = i-1; k >=2; k--){

		x2 = power(T.X, 2);	// X^2
		z2 = power(T.Z, 2);	// Z^2
		w = a*z2 + 3*x2;	// 3X^2 + aZ^2
		s = 2*T.Y*T.Z;		// 2YZ
		s2 = power(s,2);	// 4Y^2Z^2
		s3 = s2*s;			// 8Y^3Z^3
		t = T.Y*s;			// 2Y^2 Z
		//t = T.Z*s;
		t2 = power(t, 2);	// 4Y^4 Z^2
		b = power(T.X + t, 2) - x2 - t2;	// 4XY^2Z
		h = power(w, 2) - 2*b;				//

		// Calcule 2T
		tmp.X = h*s;
		tmp.Y = w*(b - h) - 2*t2;
		tmp.Z = s3; // Cost: 5m + 6s + 1*a + 7add + 3*2 + 1*3.
					// Source: 2007 Bernstein–Lange.
		f1 = power(f1, 2)*(Q.Y*s3 - s2*w*Q.X + b*w - 2*t2); // 1M + 1S + (2k + 2)m
														  // l = Q.Y - w/s(Q.X - T.X/T.Z) - T.Y/T.Z = (Q.Y*s*T.Z - w(Q.X*T.Z - T.X - T.Y*s))/s*T.Z
														  // l' = l*4T.Y^2*T.Z = l*2t
														  // l' = Q.Y*s3 - s2*w*Q.X -2r2 + b*w
		T = tmp;

		// Cost (doubling step): 2M + 2S + (3k + 7)m + 6s for general curves
		// 						 1M + 1S + (k + 7)m + 6s for curves with k even
		// 						 1M + 1S + (k + 9)m + 3s for a4 = -3 and k even

		if (bit(r, k)){ // when the i-th bit is 1
			// T = T + P; mixed addition with Z_P = 1
			// # Assumptions: Z_P=1.
			// # Cost: 9m + 2s + 6add + 1*2.
			// Source: 1998 Cohen–Miyaji–Ono "Efficient elliptic curve exponentiation using mixed coordinates", formula (3),
			// plus common-subexpression elimination, plus Z2=1.

			t = P.Y*T.Z - T.Y; t2 = power(t, 2);
			s = P.X*T.Z - T.X; s2 = power(s,2); s3 = s2*s;
			h = s2*T.X;
			b = t2*T.Z - s3 - 2*h;
			tmp.X = s*b; tmp.Y = t*(h - b) - s3*T.Y; tmp.Z = s3*T.Z;
			f1 = f1*(Q.Y*s - P.Y*s - t*(Q.X - P.X));	// 1M + 2km if (Q.X - P.X) and (Q.Y - P.Y) are precalculed
			T = tmp;

			// Cost ("mixed" addition step): 2M + (3km + 9)m + 2s
			// 								 1M + (k + 9)m + 2s for curves with k even
		}
	}

	x2 = power(T.X, 2);
	z2 = power(T.Z, 2);
	w = a*z2 + 3*x2;
	s = 2*T.Y*T.Z;
	t = T.Y*s;

	f1 = power(f1, 2)*(Q.Y*s - w*(Q.X - T.X) - t);
	f1 = f1*(Q.X - P.X);

	return f1;
}

ZZ_pE fullMiller(pt_curve_fpk P, pt_curve_fp Q, ZZ r, ZZ_p a){

	pt_curve_fpk T;
	pt_curve_fpk A;
	pt_curve_fpk tmp;
	ZZ_pE f1;

	ZZ_pE x2, z2, w, s, s2, s3, t, t2, b, h;
	int i, k;
	i = NumBits(r);
	T = P;
	f1 = 1;

	for(k = i-1; k >=2; k--){

		x2 = power(T.X, 2);	// X^2
		z2 = power(T.Z, 2);	// Z^2
		w = a*z2 + 3*x2;	// 3X^2 + aZ^2
		s = 2*T.Y*T.Z;		// 2YZ
		s2 = power(s,2);	// 4Y^2Z^2
		s3 = s2*s;			// 8Y^3Z^3
		t = T.Y*s;			// 2Y^2 Z
		//t = T.Z*s;
		t2 = power(t, 2);	// 4Y^4 Z^2
		b = power(T.X + t, 2) - x2 - t2;	// 4XY^2Z
		h = power(w, 2) - 2*b;				//

		// Calcule 2T
		tmp.X = h*s;
		tmp.Y = w*(b - h) - 2*t2;
		tmp.Z = s3; // Cost: 5m + 6s + 1*a + 7add + 3*2 + 1*3.
					// Source: 2007 Bernstein–Lange.

		f1 = power(f1, 2)*(Q.Y*s3 - s2*w*Q.X + b*w - 2*t2); // 1M + 1S + (2k + 2)m
														  // l = Q.Y - w/s(Q.X - T.X/T.Z) - T.Y/T.Z = (Q.Y*s*T.Z - w(Q.X*T.Z - T.X - T.Y*s))/s*T.Z
														  // l' = l*4T.Y^2*T.Z = l*2t
														  // l' = Q.Y*s3 - s2*w*Q.X -2r2 + b*w
		T = tmp;

		// Cost (doubling step): 2M + 2S + (3k + 7)m + 6s for general curves
		// 						 1M + 1S + (k + 7)m + 6s for curves with k even
		// 						 1M + 1S + (k + 9)m + 3s for a4 = -3 and k even

		if (bit(r, k)){ // when the i-th bit is 1
			// T = T + P; mixed addition with Z_P = 1
			// # Assumptions: Z_P=1.
			// # Cost: 9m + 2s + 6add + 1*2.
			// Source: 1998 Cohen–Miyaji–Ono "Efficient elliptic curve exponentiation using mixed coordinates", formula (3),
			// plus common-subexpression elimination, plus Z2=1.

			t = P.Y*T.Z - T.Y; t2 = power(t, 2);
			s = P.X*T.Z - T.X; s2 = power(s,2); s3 = s2*s;
			h = s2*T.X;
			b = t2*T.Z - s3 - 2*h;
			tmp.X = s*b; tmp.Y = t*(h - b) - s3*T.Y; tmp.Z = s3*T.Z;
			f1 = f1*(Q.Y*s - P.Y*s - t*(Q.X - P.X));	// 1M + 2km if (Q.X - P.X) and (Q.Y - P.Y) are precalculed
			T = tmp;

			// Cost ("mixed" addition step): 2M + (3km + 9)m + 2s
			// 								 1M + (k + 9)m + 2s for curves with k even
		}
	}

	x2 = power(T.X, 2);
	z2 = power(T.Z, 2);
	w = a*z2 + 3*x2;
	s = 2*T.Y*T.Z;
	t = T.Y*s;

	f1 = power(f1, 2)*(Q.Y*s - w*(Q.X - T.X) - t);
	f1 = f1*(Q.X - P.X);

	return f1;
}
