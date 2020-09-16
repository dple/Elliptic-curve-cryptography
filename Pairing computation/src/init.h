/*
 * init.h
 *
 *  Created on: Apr 13, 2011
 *      Author: dple
 */

#ifndef INIT_H_
#define INIT_H_
#include<stdio.h>

#include "curve.h"
#include "curvepoint_fp.h"
#include "curvepoint_fpk.h"

int init_globals(char, char, p_curve&, pt_curve_fp&, pt_curve_fpk&);

void clear_globals();

#endif /* INIT_H_ */
