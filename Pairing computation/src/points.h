/*
 * points.h
 *
 *  Created on: Apr 19, 2011
 *      Author: dple
 */

#ifndef POINTS_H_
#define POINTS_H_

#include<stdio.h>

#include "curve.h"
#include "curvepoint_fp.h"
#include "curvepoint_fpk.h"

void points_init(char c, char c1, pt_curve_fp & rop,  pt_curve_fpk & ropk, int k);

void points_clear(void);

#endif /* POINTS_H_ */
