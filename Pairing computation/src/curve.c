/*
 * curve.c
 *
 *  Created on: Apr 14, 2011
 *      Author: dple
 */

#include<stdio.h>
#include<NTL/ZZ.h>
#include<NTL/ZZ_p.h>

#include "curve.h"
#include "parameters.h"

NTL_CLIENT

p_curve curve_init(ZZ r, ZZ charac, int a, int b) {
	p_curve rop;
	rop.r = r;
	rop.charac = charac;
	rop.a = 0;
	rop.b = 5;
	cout << endl << "We work on curve E: y^2 = x^3 + " << a << "x + " << b << " over finite field F_p : " << charac << endl;
	cout << "The size of the finite field F_p = " << NumBits(charac) << " bits " << endl;
	cout << "The order of the curve E = " << r << endl;
	return rop;
}

int curve_init(char c, char c1, p_curve & curve) {
	/* Curve parameters */
	//printf("ok \n");
	int k = 1;
	if ((c == '1') && (c1 == '1')) {
		curve.r = to_ZZ(BN_R128);
		curve.charac = to_ZZ(BN_P128);
		curve.a = 0;
		curve.b = 5;
		k = 12;
	} else if ((c == '1') && (c1 == '2')) {
		curve.r = to_ZZ(CB_R128);
		curve.charac = to_ZZ(CB_P128);
		curve.a = 0;
		curve.b = 1;
		k = 9;
	} else if ((c == '1') && (c1 == '3')) {
		curve.r = to_ZZ(KSS_R128);
		curve.charac = to_ZZ(KSS_P128);
		curve.a = 0;
		curve.b = 19;
		k = 18;
	} else if ((c == '2') && (c1 == '1')) {
		curve.r = to_ZZ(CB_R192);
		curve.charac = to_ZZ(CB_P192);
		curve.a = 0;
		curve.b = 1;
		k = 15;
	} else if ((c == '2') && (c1 == '2')) {
		curve.r = to_ZZ(KSS_R192);
		curve.charac = to_ZZ(KSS_P192);
		curve.a = 0;
		curve.b = 21;
		k = 18;
	} else if ((c == '3') && (c1 == '1')) {
		curve.r = to_ZZ(BN_R256);
		curve.charac = to_ZZ(BN_P256);
		curve.a = 0;
		curve.b = 13;
		k = 24;
	} else if ((c == '3') && (c1 == '2')) {
		curve.r = to_ZZ(CB_R256);
		curve.charac = to_ZZ(CB_P256);
		curve.a = 0;
		curve.b = 37;
		k = 27;
	}


	cout << "We work on curve E: y^2 = x^3 + " << curve.a << "x + " << curve.b << " over finite field F_p : " << curve.charac << endl;
	cout << "The size of the finite field F_p = " << NumBits(curve.charac) << " bits " << endl;
	cout << "The order of subgroup of points on the curve E = " << curve.r << endl;
	return k;
}

void curve_clear() {
	;
}
