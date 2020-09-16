/*
 * fp.c
 *
 *  Created on: Apr 14, 2011
 *      Author: dple
 */

#include<stdio.h>
#include<NTL/ZZ.h>
#include<NTL/ZZ_p.h>

#include "fp.h"
#include "parameters.h"


NTL_CLIENT

void fp_init0(ZZ charac) {
	ZZ_p::init(charac);
	return;
}

int fp_init(char c, char c1) {
	ZZ car;
	int rop = 0;
	if (c == '1') {
		if (c1 == '1') {
			printf("\nYou choose 128-bits security level and BN curves (the embedding degree k = 12) \n");
			car = to_ZZ(BN_P128);
			rop = 1;
		}
		else if (c1 == '2') {
			printf("\nYou choose 128-bits security level and cubic twisted curves (the embedding degree k = 9) \n");
			car = to_ZZ(CB_P128);
			rop = 1;
		}
/*		else if (c1 == '3') {
			car = to_ZZ(KSS_P128);
			printf("\nYou choose 128-bits security level and KSS curves (the embedding degree k = 18) \n");
			rop = 1;
		}
		*/
	}
	else if (c == '2') {
		if (c1 == '1') {
			printf("\nYou choose 192-bits security level and cubic twisted curves (the embedding degree k = 15) \n");
			car = to_ZZ(CB_P192);
			rop = 1;
			//printf("\nSorry, we have not yet curve example for this curves. Please choose again! \n");
		}
		else if (c1 == '2') {
			printf("\nYou choose 192-bits security level and KSS curves (the embedding degree k = 18) \n");
			car = to_ZZ(KSS_P192);
			rop = 1;
		}
		else if (c1 == '3') {
			//car = to_ZZ(KSS_P192);
			printf("\nYou choose 192-bits security level and KSS curves (the embedding degree k = 18) \n");
			printf("\nSorry, we have not yet curve example for this curves. Please choose again! \n");
		}
	}
	else if (c == '3') {
		if (c1 == '1') {
			printf("\nYou choose 256-bits security level and BN curves (the embedding degree k = 24) \n");
			car = to_ZZ(BN_P256);
			rop = 1;
		}
		else if (c1 == '2') {
			printf("\nYou choose 256-bits security level and cubic twisted curves (the embedding degree k = 27) \n");
			car = to_ZZ(CB_P256);
			rop = 1;
		}
	}

	if (rop == 1) {
		ZZ_p::init(car);
	}
	return rop;

}

void fp_clear() {

	;
}
