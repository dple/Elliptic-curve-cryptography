/*
 * algorithms.h
 *
 *  Created on: Apr 21, 2011
 *      Author: dple
 */

#ifndef ALGORITHMS_H_
#define ALGORITHMS_H_

#include<NTL/ZZ.h>
#include<NTL/ZZ_p.h>
#include<NTL/ZZ_pE.h>

#include "curvepoint_fp.h"
#include "curvepoint_fpk.h"

NTL_CLIENT
/*
 * Miller algorithms for cubic twisted curves
 */
ZZ_pE littleMillerCubic(pt_curve_fp P, pt_curve_fpk Q, ZZ r, ZZ_p b);
ZZ_pE fullMillerCubic(pt_curve_fpk Q, pt_curve_fp P, ZZ r, ZZ_p b);

/*
 * Miller algorithms for even twisted curves
 */
ZZ_pE littleMiller(pt_curve_fp P, pt_curve_fpk Q, ZZ r, ZZ_p a);
ZZ_pE fullMiller(pt_curve_fpk Q, pt_curve_fp P, ZZ r, ZZ_p a);


#endif /* ALGORITHMS_H_ */
