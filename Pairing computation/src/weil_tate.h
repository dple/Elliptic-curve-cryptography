/*
 * tate_pairing.h
 *
 *  Created on: Apr 20, 2011
 *      Author: dple
 */

#ifndef TATE_PAIRING_H_
#define TATE_PAIRING_H_

//#include<NTL/ZZ.h>
//#include<NTL/ZZ_p.h>
#include<NTL/ZZ_pE.h>

#include "curvepoint_fp.h"
#include "curvepoint_fpk.h"
#include "curve.h"

NTL_CLIENT

ZZ_pE tate(const pt_curve_fp op1, const pt_curve_fpk op2, int k, p_curve Cur);
ZZ_pE weil(const pt_curve_fp op1, const pt_curve_fpk op2, int k, p_curve Cur);

#endif /* TATE_PAIRING_H_ */
