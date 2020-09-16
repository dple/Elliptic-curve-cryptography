/*
 * fp.h
 *
 *  Created on: Apr 14, 2011
 *      Author: dple
 */

#ifndef FP_H_
#define FP_H_

#include<NTL/ZZ.h>

NTL_CLIENT
void fp_init0(ZZ charac);

int fp_init(char, char);

void fp_clear();

#endif /* FP_H_ */
