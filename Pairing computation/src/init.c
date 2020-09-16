/*
 * init.c
 *
 *  Created on: Apr 13, 2011
 *      Author: dple
 */

#include<NTL/ZZ.h>
#include<NTL/ZZ_p.h>
#include<NTL/ZZ_pE.h>
#include<NTL/ZZ_pX.h>
#include<NTL/ZZ_pEX.h>

#include "init.h"
#include "fp.h"
#include "fpk.h"
#include "curve.h"
#include "curvepoint_fp.h"
#include "curvepoint_fpk.h"
#include "points.h"

NTL_CLIENT

/*
 * Initialize global parameters
 */
int init_globals(char c, char c1, p_curve & curve, pt_curve_fp & rop, pt_curve_fpk & ropk) {
	int k;
	printf("\nInitializing global parameters ... \n");
	// Initialize the curves
	k = curve_init(c, c1, curve);
	// Initialize the points
	points_init(c, c1, rop, ropk, k);
	printf("\nPairing computation of the points (in projective coordinates):\n");
	cout << endl << "P : "; //<< rop.X << ", " << rop.Y << ", " << rop.Z << "]" << endl;
	curvepoint_fp_print(rop);

	cout << endl << "and Q :";// << ropk.X << ", " << ropk.Y << ", " << ropk.Z << "]" << endl;
	curvepoint_fpk_print(ropk);
	return k;
}

/*
 * Clear global parameters
 */
void clear_globals() {
	printf("\nClearing global parameters ...\n");
	return;
}
