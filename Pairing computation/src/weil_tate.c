/*
 * tate_pairing.c
 *
 *  Created on: Apr 20, 2011
 *      Author: dple
 */
#include<stdio.h>
#include<NTL/ZZ.h>
#include<NTL/ZZ_p.h>
#include<NTL/ZZ_pE.h>

#include "algorithms.h"
#include "weil_tate.h"
#include "curvepoint_fp.h"
#include "curvepoint_fpk.h"
#include "curve.h"

NTL_CLIENT


ZZ_pE tate(const pt_curve_fp op1, const pt_curve_fpk op2, int k, p_curve Cur) {
	ZZ_pE f;
	ZZ p = Cur.charac;
	ZZ r = Cur.r;
	ZZ_p a = Cur.a;
	ZZ_p b = Cur.b;
	ZZ final_exp = (power(p,k) - 1)/r;
	printf("The bit number of the order r is : %ld \n", NumBits(r));
	if (k % 2 == 1) {
		f = littleMillerCubic(op1, op2, r, b);
	} else {
		f = littleMiller(op1, op2, r, a);
	}
	f = power(f, final_exp);
	cout << "Pairing computation e(P, Q) is :"  << f << endl;
	return f;
}

ZZ_pE weil(const pt_curve_fp op1, const pt_curve_fpk op2, int k, p_curve Cur) {
	ZZ_pE f, g, pairing;
	ZZ p = Cur.charac;
	ZZ r = Cur.r;
	ZZ_p a = Cur.a;
	ZZ_p b = Cur.b;
	printf("The bit number of the order r is : %ld \n", NumBits(r));
	if (k % 2 == 1) {
		f = littleMillerCubic(op1, op2, r, b);
		g = fullMillerCubic(op2, op1, r, b);
	} else {
		f = littleMiller(op1, op2, r, a);
		g = fullMiller(op2, op1, r, a);
	}
	pairing = f/g;
	cout << "Pairing computation e(P, Q) is :"  << pairing << endl;
	return pairing;
}
