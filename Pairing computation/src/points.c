/*
 * points.c
 *
 *  Created on: Apr 19, 2011
 *      Author: dple
 */
#include<NTL/ZZ_pE.h>
#include<stdio.h>

#include "points.h"
#include "parameters.h"
#include "curvepoint_fp.h"
#include "curvepoint_fpk.h"
#include "fpk.h"

NTL_CLIENT

void points_init(char c, char c1, pt_curve_fp & rop, pt_curve_fpk & ropk, int k) {
	ZZ_pE x, y;
	if ((c == '1') && (c1 == '1')) {
		curvepoint_fp_set_gen(rop, BN_CURVEGEN128);
		x = fpk_set_str(k, BN_KGEN128_X);
		y = fpk_set_str(k, BN_KGEN128_Y);
		curvepoint_fpk_set_gen(ropk, x, y);
	} else if ((c == '1') && (c1 == '2')) {
		curvepoint_fp_set_gen(rop, CB_CURVEGEN128);
		x = fpk_set_str(k, CB_KGEN128_X);
		y = fpk_set_str(k, CB_KGEN128_Y);
		curvepoint_fpk_set_gen(ropk, x, y);
	} else if ((c == '1') && (c1 == '3')) {
		curvepoint_fp_set_gen(rop, KSS_CURVEGEN128);
		x = fpk_set_str(k, KSS_KGEN128_X);
		y = fpk_set_str(k, KSS_KGEN128_Y);
		curvepoint_fpk_set_gen(ropk, x, y);
	} else if ((c == '2') && (c1 == '1')) {
		curvepoint_fp_set_gen(rop, CB_CURVEGEN192);
		x = fpk_set_str(k, CB_KGEN192_X);
		y = fpk_set_str(k, CB_KGEN192_Y);
		curvepoint_fpk_set_gen(ropk, x, y);
	} else if ((c == '2') && (c1 == '2')) {
		curvepoint_fp_set_gen(rop, KSS_CURVEGEN192);
		x = fpk_set_str(k, KSS_KGEN192_X);
		y = fpk_set_str(k, KSS_KGEN192_Y);
		curvepoint_fpk_set_gen(ropk, x, y);
	} else if ((c == '3') && (c1 == '1')) {
		curvepoint_fp_set_gen(rop, BN_CURVEGEN256);
		x = fpk_set_str(k, BN_KGEN256_X);
		y = fpk_set_str(k, BN_KGEN256_Y);
		curvepoint_fpk_set_gen(ropk, x, y);
	} else if ((c == '3') && (c1 == '2')) {
		curvepoint_fp_set_gen(rop, CB_CURVEGEN256);
		x = fpk_set_str(k, CB_KGEN256_X);
		y = fpk_set_str(k, CB_KGEN256_Y);
		curvepoint_fpk_set_gen(ropk, x, y);
	}


}

void points_clear(void) {
	;
}
