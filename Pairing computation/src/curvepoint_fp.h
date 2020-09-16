/*
 * curvepoint_fp.h
 *
 *  Created on: Apr 14, 2011
 *      Author: dple
 */

#ifndef CURVEPOINT_FP_H_
#define CURVEPOINT_FP_H_

#include<NTL/ZZ.h>
#include<NTL/ZZ_p.h>
#include <stdio.h>

NTL_CLIENT

/*
 * Define structure of a point curve on projective coordinates
 * Z = 1 when points represented in affine
 */
typedef struct _pt_curve {
	ZZ_p X, Y, Z; //  three coordinates. The curve defined on the base field ZZ_p
	bool infinity;
}pt_curve_fp;

void curvepoint_fp_set_gen(pt_curve_fp & rop, const char* x,const char* y, const char* z);

void curvepoint_fp_init(pt_curve_fp & rop);

pt_curve_fp curvepoint_fp_double(const pt_curve_fp op);

pt_curve_fp curvepoint_fp_mul(const pt_curve_fp op, const int scalar, const ZZ_p a);

pt_curve_fp curvepoint_fp_add(const pt_curve_fp op, const pt_curve_fp Q);

pt_curve_fp curvepoint_fp_sub(const pt_curve_fp op, const pt_curve_fp Q);

// Compute the Inverse of a Point op, store result in rop:
pt_curve_fp curvepoint_fp_neg(const pt_curve_fp op);

pt_curve_fp triple_pt(const pt_curve_fp op, const ZZ_p a);

pt_curve_fp five_pt(const pt_curve_fp op, const ZZ_p a);

pt_curve_fp seven_pt(const pt_curve_fp op, const ZZ_p a);

pt_curve_fp curvepoint_random_fp(const pt_curve_fp op, ZZ_p a);

// Print the (Projective) coordinates of a point
void curvepoint_fp_print(const pt_curve_fp point);

/*

// Generate a curve point by copying the coordinates from another curve point
void curvepoint_fp_set(pt_curve_fp point, const pt_curve_fp arg);

void curvepoint_fp_mixadd(pt_curve_fp rop, const pt_curve_fp op1, const pt_curve_fp op2);


// Transform to Affine Coordinates (z=1)
void curvepoint_fp_makeaffine(pt_curve_fp point);

*/

#endif /* CURVEPOINT_FP_H_ */
