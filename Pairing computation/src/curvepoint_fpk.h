/*
 * curvepoint_fpk.h
 *
 *  Created on: Apr 19, 2011
 *      Author: dple
 */

#ifndef CURVEPOINT_FPK_H_
#define CURVEPOINT_FPK_H_

#include<NTL/ZZ_p.h>
#include<NTL/ZZ_pE.h>

NTL_CLIENT
/*
 * Structure of points on the extension field
 */
typedef struct _pt_curve_fpk {
	ZZ_pE X, Y, Z; //  two coordinates. The curve defined on the extension field ZZ_pE
	bool infinity;
}pt_curve_fpk;

void curvepoint_fpk_set_gen(pt_curve_fpk & rop, ZZ_pE x, ZZ_pE y);
//pt_curve_fp curvepoint_fp_double(const pt_curve_fp op);

void curvepoint_fpk_init(pt_curve_fpk & rop);

pt_curve_fpk curvepoint_fpk_double(const pt_curve_fpk op);

pt_curve_fpk curvepoint_fpk_mul(const pt_curve_fpk op, const int scalar, const ZZ_p a);

pt_curve_fpk curvepoint_fpk_add(const pt_curve_fpk op, const pt_curve_fpk Q);

pt_curve_fpk curvepoint_fpk_sub(const pt_curve_fpk op, const pt_curve_fpk Q);

// Compute the Inverse of a Point op, store result in rop:
pt_curve_fpk curvepoint_fpk_neg(const pt_curve_fpk op);

pt_curve_fpk fpk_triple_pt(const pt_curve_fpk op, const ZZ_p a);

pt_curve_fpk fpk_five_pt(const pt_curve_fpk op, const ZZ_p a);

pt_curve_fpk fpk_seven_pt(const pt_curve_fpk op, const ZZ_p a);

pt_curve_fpk curvepoint_random_fpk(const pt_curve_fpk op, ZZ_p a);

void curvepoint_fpk_print(const pt_curve_fpk op);

#endif /* CURVEPOINT_FPK_H_ */
