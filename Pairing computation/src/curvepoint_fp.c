/*
 * curvepoint_fp.c
 *
 *  Created on: Apr 14, 2011
 *      Author: dple
 */

#include "curvepoint_fp.h"
#include "curve.h"
#include "utils.h"

void curvepoint_fp_set_gen(pt_curve_fp & rop, const char* x,const char* y,const char* z){
	rop.X = to_ZZ_p(to_ZZ(x));
	rop.Y = to_ZZ_p(to_ZZ(y));
	rop.Z = to_ZZ_p(to_ZZ(z));
}

void curvepoint_fp_print(const pt_curve_fp op) {
	cout << endl << "P.X = [" << op.X << "]" << endl << "P.Y = [" << op.Y << "]" << endl << "P.Z = [" << op.Z << "]" << endl;
	return;
}

pt_curve_fp curvepoint_fp_double(const pt_curve_fp op, ZZ_p a) {
	pt_curve_fp rop;
	if (op.infinity)
		return op;

	ZZ_p x2, z2, w, s, s2, s3, t, t2, b, h;
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

pt_curve_fp curvepoint_fp_add(const pt_curve_fp P, const pt_curve_fp Q) {
	// P or Q is infinity
	if (P.infinity)
		return Q;
	if (Q.infinity)
		return P;

	ZZ_p a, b, c, v, v2, v3, u, u2, r, s;
	pt_curve_fp R;

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
pt_curve_fp curvepoint_fp_neg(const pt_curve_fp op){
	pt_curve_fp rop;

	rop.X = op.X;
	rop.Y = -op.Y;
	rop.Z = op.Z;

	return rop;
}

pt_curve_fp curvepoint_fp_sub(const pt_curve_fp P, const pt_curve_fp Q) {

	return curvepoint_fp_add(P, curvepoint_fp_neg(Q));
}

/*
 * Triple of P
 *
 * input P
 * @return 3P
 */

pt_curve_fp triple_pt(const pt_curve_fp P, const ZZ_p a) {
	pt_curve_fp Q;
	Q = curvepoint_fp_double(P, a);
	Q = curvepoint_fp_add(P, Q);
	return Q;
}

// Return 5P
pt_curve_fp five_pt(const pt_curve_fp P, const ZZ_p a) {
	pt_curve_fp Q;
	Q = curvepoint_fp_double(P, a);
	Q = curvepoint_fp_double(Q, a);
	Q = curvepoint_fp_add(P, Q);
	return Q;
}

// Return 7P
pt_curve_fp seven_pt(const pt_curve_fp P, const ZZ_p a) {
	pt_curve_fp Q;
	Q = curvepoint_fp_double(P, a);
	Q = curvepoint_fp_add(P, Q);
	Q = curvepoint_fp_double(Q, a);
	Q = curvepoint_fp_add(P, Q);
	return Q;
}

/*
 * Computer [n]P by using window-based scalar multiplication
 */
pt_curve_fp curvepoint_fp_mul(const pt_curve_fp op, const int scalar, const ZZ_p a){

	pt_curve_fp rop, P3, P5, P7;
	// Initialisation rop <- point at infinity
	rop.X = 0;
	rop.Y = 0;
	rop.Z = 1;
	rop.infinity = true;

	P3 = triple_pt(op, a);
	P5 = five_pt(op, a);
	P7 = seven_pt(op, a);
	int c = bitcount(scalar);
	int i = c;
	int w = 3;
	int s, j, k, u;
	while (i > 0) {
		if (!bit(scalar, i)){
			rop = curvepoint_fp_double(rop, a);
			i--;
		}
		else {
			s = (i - w + 1) > 0 ? (i - w + 1) : 0;
			while (!bit(scalar, s))
				s++;
			k = i - s + 1; // The size of current window
			if (i != c)
				for(j = 1; j <= k; j++) 	rop = curvepoint_fp_double(rop, a);

			u = extract(scalar, s - 1, k); // The value of window

			if (u == 1)
				rop = curvepoint_fp_add(rop, op);
			if (u == 3)
				rop = curvepoint_fp_add(rop, P3);
			if (u == 5)
				rop = curvepoint_fp_add(rop, P5);
			if (u == 7)
				rop = curvepoint_fp_add(rop, P7);
			i = s - 1;
		}
	}
	return rop;
}


void curvepoint_fp_init(pt_curve_fp & rop) {
	rop.X = 1;
	rop.Y = 1;
	rop.Z = 1;
}

pt_curve_fp curvepoint_random_fp(const pt_curve_fp op, ZZ_p a) {
	pt_curve_fp rop;
	int scalar = rand();
	return curvepoint_fp_mul(op, scalar, a);
}

/*
void curvepoint_fp_init_set(pt_curve_fp rop, const pt_curve_fp op){

	return;
}

// Generate a curve point by copying the coordinates from another curve point
void curvepoint_fp_set(pt_curve_fp point, const pt_curve_fp arg){

	return;
}

void curvepoint_fp_mixadd(pt_curve_fp rop, const pt_curve_fp op1, const pt_curve_fp op2){

	return;
}

void curvepoint_fp_double(pt_curve_fp rop, const pt_curve_fp op){

	return;
}


// Transform to Affine Coordinates (z=1)
void curvepoint_fp_makeaffine(pt_curve_fp point){

	return;
}

// Print the (Projective) coordinates of a point
void curvepoint_fp_print(FILE *outfile, const pt_curve_fp point){

	return;
}
*/
