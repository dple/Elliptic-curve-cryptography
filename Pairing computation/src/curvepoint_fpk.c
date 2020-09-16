/*
 * curvepoint_fpk.c
 *
 *  Created on: Apr 19, 2011
 *      Author: dple
 */
#include "curvepoint_fpk.h"
#include "utils.h"

void curvepoint_fpk_set_gen(pt_curve_fpk & rop, ZZ_pE x, ZZ_pE y){
	rop.X = x;
	rop.Y = y;
	rop.Z = 1;
	rop.infinity = false;
}

void curvepoint_fpk_init(pt_curve_fpk & rop) {
	rop.X = 1;
	rop.Y = 1;
	rop.Z = 1;
	rop.infinity = false;
}

void curvepoint_fpk_print(const pt_curve_fpk op) {
	cout << endl << "Q.X = " << op.X << endl << "Q.Y = " << op.Y << endl << "Q.Z = " << op.Z << endl;
	return;
}
/*
 * Double a point
 */
pt_curve_fpk curvepoint_fpk_double(const pt_curve_fpk op, ZZ_p a) {
	pt_curve_fpk rop;
	if (op.infinity)
		return op;

	ZZ_pE x2, z2, w, s, s2, s3, t, t2, b, h;
	//if ( 2*P.Y + C.a1*P.X + C.a3 == 0 ) {    // cas 2P = O
	if (op.Y == 0 ) {    // cas 2P = O
		rop.infinity = true;
		rop.X = 0;
		rop.Y = 0;
		rop.Z = 0;
	}
	else {
		x2 = power(op.X, 2);	// X^2
		z2 = power(op.Z, 2);	// Z^2
		w = a*z2 + 3*x2;	// 3X^2 + aZ^2
		s = 2*op.Y*op.Z;		// 2YZ
		s2 = power(s,2);	// 4Y^2Z^2
		s3 = s2*s;			// 8Y^3Z^3
		t = op.Y*s;			// 2Y^2 Z
		//t = T.Z*s;
		t2 = power(t, 2);	// 4Y^4 Z^2
		b = power(op.X + t, 2) - x2 - t2;	// 4XY^2Z
		h = power(w, 2) - 2*b;				//

		// Calcule 2T
		rop.X = h*s;
		rop.Y = w*(b - h) - 2*t2;
		rop.Z = s3;

		rop.infinity = false;
	}

	return rop;
}

// Addition two points on the base field

pt_curve_fpk curvepoint_fpk_add(const pt_curve_fpk P, const pt_curve_fpk Q) {
	// P or Q is infinity
	if (P.infinity)
		return Q;
	if (Q.infinity)
		return P;

	ZZ_pE a, b, c, v, v2, v3, u, u2, r, s;
	pt_curve_fpk R;

	//if ((P.X == Q.X) && (P.Y + Q.Y + C.a1*Q.X + C.a3 == 0)) { // P = -Q
	if ((P.X == Q.X) && (P.Y + Q.Y == 0) && (P.Z == Q.Z)) { // P = -Q, Short Weierstrass curves
		R.infinity = true;
		R.X = 0;
		R.Y = 0;
		R.Z = 1;
	}
	else {	// P != Q
		a = P.Y*Q.Z; b = P.X*Q.Z; c = P.Z*Q.Z;
		u = Q.Y*P.Z - P.Y*Q.Z; u2 = power(u,2);
		v = Q.X*P.Z - P.X*Q.Z; v2 = power(v,2); v3 = v2*v;
		r = v2*P.X*Q.Z; s = u2*c - v3 - 2*r;
		R.X = v*s; R.Y = u*(r - s) - v3*P.Y*Q.Z; R.Z = v3*c;
		R.infinity = false;
	}
	return R;
}

// Compute the Inverse of a Point op, store result in rop:
pt_curve_fpk curvepoint_fpk_neg(const pt_curve_fpk op){
	pt_curve_fpk rop;

	rop.X = op.X;
	rop.Y = -op.Y;
	rop.Z = op.Z;

	return rop;
}

pt_curve_fpk curvepoint_fpk_sub(const pt_curve_fpk P, const pt_curve_fpk Q) {

	return curvepoint_fpk_add(P, curvepoint_fpk_neg(Q));
}

/*
 * Triple of P
 *
 * input P
 * @return 3P
 */

pt_curve_fpk fpk_triple_pt(const pt_curve_fpk P, const ZZ_p a) {
	pt_curve_fpk Q;
	Q = curvepoint_fpk_double(P, a);
	Q = curvepoint_fpk_add(P, Q);
	return Q;
}

// Return 5P
pt_curve_fpk fpk_five_pt(const pt_curve_fpk P, const ZZ_p a) {
	pt_curve_fpk Q;
	Q = curvepoint_fpk_double(P, a);
	Q = curvepoint_fpk_double(Q, a);
	Q = curvepoint_fpk_add(P, Q);
	return Q;
}

// Return 7P
pt_curve_fpk fpk_seven_pt(const pt_curve_fpk P, const ZZ_p a) {
	pt_curve_fpk Q;
	Q = curvepoint_fpk_double(P, a);
	Q = curvepoint_fpk_add(P, Q);
	Q = curvepoint_fpk_double(Q, a);
	Q = curvepoint_fpk_add(P, Q);
	return Q;
}

/*
 * Computer [n]P by using window-based scalar multiplication
 */
pt_curve_fpk curvepoint_fpk_mul(const pt_curve_fpk op, const int scalar, const ZZ_p a){

	pt_curve_fpk rop, P3, P5, P7;
	// Initialisation rop <- point at infinity
	rop.X = 0;
	rop.Y = 0;
	rop.Z = 1;
	rop.infinity = true;

	P3 = fpk_triple_pt(op, a);
	P5 = fpk_five_pt(op, a);
	P7 = fpk_seven_pt(op, a);
	int c = bitcount(scalar);
	int i = c;
	int w = 3;
	int s, j, k, u;
	while (i > 0) {
		if (!bit(scalar, i)){
			rop = curvepoint_fpk_double(rop, a);
			i--;
		}
		else {
			s = (i - w + 1) > 0 ? (i - w + 1) : 0;
			while (!bit(scalar, s))
				s++;
			k = i - s + 1; // The size of current window
			if (i != c)
				for(j = 1; j <= k; j++) 	rop = curvepoint_fpk_double(rop, a);

			u = extract(scalar, s - 1, k); // The value of window

			if (u == 1)
				rop = curvepoint_fpk_add(rop, op);
			if (u == 3)
				rop = curvepoint_fpk_add(rop, P3);
			if (u == 5)
				rop = curvepoint_fpk_add(rop, P5);
			if (u == 7)
				rop = curvepoint_fpk_add(rop, P7);
			i = s - 1;
		}
	}
	return rop;
}


pt_curve_fpk curvepoint_random_fpk(const pt_curve_fpk op, ZZ_p a) {
	pt_curve_fpk rop;
	int scalar = rand();
	return curvepoint_fpk_mul(op, scalar, a);
}
