/*
 * fpk.c
 *
 *  Created on: Apr 19, 2011
 *      Author: dple
 */

#include<stdio.h>
#include<NTL/ZZ.h>
#include<NTL/ZZ_p.h>
#include<NTL/ZZ_pE.h>
#include<NTL/ZZ_pX.h>
#include<NTL/ZZ_pEX.h>
#include <NTL/ZZ_pXFactoring.h>
#include<string.h>



#include "fp.h"
#include "fpk.h"
#include "parameters.h"

//using std::string;

NTL_CLIENT

void fpk_init0(int k) {
	ZZ_pX Pol;             // polynome pour F_q^k
	BuildIrred(Pol, k);
	ZZ_pE::init(Pol);
}

void fpk_init(char c, char c1) {
	if ((c == '1') && (c1 == '1')) {
		fp_init(c, c1);
		ZZ_pX Pol;             // polynome pour F_q^12
		//BuildIrred(Pol, 12);
		//Pol = X^12 + 5
		SetCoeff(Pol,12,1);
		SetCoeff(Pol,0,5);
		ZZ_pE::init(Pol);
	} else if ((c == '1') && (c1 == '2')) {
		ZZ_pX Pol;             // irrecducible polynomial for F_q^9
		//BuildIrred(Pol, 9);
		// Pol = X^9 + X + 1
		SetCoeff(Pol,9,1);
		SetCoeff(Pol,1,1);
		SetCoeff(Pol,0,1);
		ZZ_pE::init(Pol);
	} else if ((c == '1') && (c1 == '3')) {
		ZZ_pX Pol;             // irrecducible polynomial for F_q^18
		//BuildIrred(Pol, 18);
		// Pol = X^18 + X + 3
		SetCoeff(Pol,18,1);
		SetCoeff(Pol,1,1);
		SetCoeff(Pol,0,3);
		ZZ_pE::init(Pol);
	} else if ((c == '2') && (c1 == '1')) {
		ZZ_pX Pol;             // irrecducible polynomial for F_q^15
		//BuildIrred(Pol, 15);
		SetCoeff(Pol,15,1);
		SetCoeff(Pol,1,1);
		SetCoeff(Pol,0,13);
		ZZ_pE::init(Pol);
	} else if ((c == '2') && (c1 == '2')) {
		ZZ_pX Pol;             // irrecducible polynomial for F_q^18
		//BuildIrred(Pol, 18);
		SetCoeff(Pol,18,1);
		SetCoeff(Pol,1,1);
		SetCoeff(Pol,0,76);
		ZZ_pE::init(Pol);
	} else if ((c == '3') && (c1 == '1')) {
		ZZ_pX Pol;             // irrecducible polynomial for F_q^24
		//BuildIrred(Pol, 24);
		SetCoeff(Pol,24,1);
		SetCoeff(Pol,0,11);
		ZZ_pE::init(Pol);
	} else if ((c == '3') && (c1 == '2')) {
		ZZ_pX Pol;             // irrecducible polynomial for F_q^27
		//BuildIrred(Pol, 27);
		SetCoeff(Pol,27,1);
		SetCoeff(Pol,1,1);
		SetCoeff(Pol,0,26);
		ZZ_pE::init(Pol);
	}

}

ZZ_pE fpk_set_str(int k, const char* x) {
	int begin, h;
	ZZ_pE rop;
	ZZ_pX var;
	string str = x;
	string s, s1;
	const char* c;
	begin = 0;
	h = str.find(",");
	for(int i = 0; i < k; i++) {
		s.assign(str,begin, h);
		s1.assign(str,h+1,str.size());
		str = s1; c= s.c_str();
		SetCoeff(var,i,to_ZZ_p(to_ZZ(c)));
		h = str.find(",");
	}
	rop = to_ZZ_pE(var);
	return rop;
}

void fpk_clear() {

	return;
}
