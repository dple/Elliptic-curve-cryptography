/*
 * curve.h
 *
 *  Created on: Apr 14, 2011
 *      Author: dple
 */

#ifndef CURVE_H_
#define CURVE_H_

#include<stdio.h>
#include<NTL/ZZ.h>
#include<NTL/ZZ_p.h>

NTL_CLIENT
/*
 * Define structure of elliptic curves
 */
struct p_curve {
	ZZ_p a; /* parameter a in the curve equation y^2 = x^3 + ax + b */
	ZZ_p b; /* parameter b in the curve equation y^2 = x^3 + ax + b */
	ZZ charac;
	ZZ r; /* order of the curve */
	ZZ x;
	ZZ trace; /* trace of Frobenius */
	ZZ chi; /* p^k/r */
	ZZ chicomp;
	int looplength_eta; // Miller loop length during Eta pairing computation
};

/* Initialize Curve parameters */
p_curve curve_init(ZZ r, ZZ charac, int a, int b);
int curve_init(char c, char c1, p_curve& curve);

/* Free memory used by curve parameters */
void curve_clear(void);

#endif /* CURVE_H_ */
