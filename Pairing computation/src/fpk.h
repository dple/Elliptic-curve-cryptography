/*
 * fpk.h
 *
 *  Created on: Apr 19, 2011
 *      Author: dple
 */

#ifndef FPK_H_
#define FPK_H_

#include<NTL/ZZ_pE.h>

NTL_CLIENT

void fpk_init0(int k);

void fpk_init(char, char);

ZZ_pE fpk_set_str(int k, const char* x);

void fpk_clear();


#endif /* FPK_H_ */
