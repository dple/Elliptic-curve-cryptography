/*
 * tlpairing.c
 *
 *  Created on: Apr 13, 2011
 *      Author: dple
 */

#include<stdio.h>

#include "init.h"
#include "fp.h"
#include "fpk.h"
#include "utils.h"
#include "parameters.h"
#include "curvepoint_fp.h"
#include "curvepoint_fpk.h"
#include "curve.h"
#include "cpucycles.h"
#include "weil_tate.h"


unsigned int SECU_LEVEL;

/*
 * Print help comments
 */
static void print_help(void) {
	printf("Menu: list of functions can be executed \n\n");
	printf("l: Choose another security level\n");
	printf("c: Choose another family of Pairing-friendly elliptic curves\n");
	printf("t: Tate Pairing computation\n");
	printf("w: Weil Pairing computation\n");
//	printf("o: Optimal Ate Pairing computation\n");
	printf("n: Generate two new random points\n");
	printf("s: Set P and Q to predefined generators\n");
	printf("p: Customize parameters\n");
	printf("q: Quit the program \n");
}

/*
 * Print help for choosing a security level
 */
static void print_secu_level(void) {
	printf("\nPlease choose a security level: \n");
	printf("1. 128-bits security level \n");
	printf("2. 192-bits security level \n");
	printf("3. 256-bits security level \n");

}

/*
 * Print help for choosing a family of pairing-friendly elliptic curves
 */

static void print_curves(char c) {
	if (c == '1') {
		printf("1. Barreto-Naehrig elliptic curves (k = 12)\n");
		printf("2. Cubic twisted elliptic curves (k = 9)\n");
//		printf("3. KSS curves \n");
	}
	else if (c == '2') {
	//	printf("1. Barreto-Naehrig curves \n");
		printf("2. Cubic twisted elliptic curves (k = 15)\n");
		printf("2. KSS elliptic curves (k = 18)\n");
	}
	else if (c == '3') {
		printf("1. Barreto-Naehrig elliptic curves \n");
		printf("2. Cubic twisted elliptic curves (k = 27)\n");
	}

}

//p_curve Cur;
//pt_curve_fp fp_curve_gen;

int k = 1;
ZZ p;

int main(int argc, char* argv[]) {

	char choice, choice1, oldchoice;
	uint64 start, stop;
	char string[1024];                               // A long string
	char str0[1024*28];
	ZZ charac, order;
	int a, b;

	// Choose a security level
	do {
		print_secu_level();
		choice = trapkey();
		if (choice == '1') {
			SECU_LEVEL = 128;
			printf("We work on 128-bits security level \n");
			//break;
		}
		else if (choice == '2') {
			SECU_LEVEL = 192;
			printf("We work on 192-bits security level \n");
			//break;
		}
		else if (choice == '3') {
			SECU_LEVEL = 256;
			printf("We work on 256-bits security level \n");
			//break;
		}
		else {
			printf("\nUnknown command, press '1', '2', or '3' for choosing one security level \n");
		}
		// Choose one curve
		print_curves(choice);
		do {
			choice1 = trapkey();
			if ((choice1 != '1') && (choice1 != '2')) // && (choice1 != '3'))
				printf("\nUnknown command, press '1' or '2' for choosing a pairing-friendly elliptic curves: ");
			else {
				break;
			}
		} while(1);

	} while (fp_init(choice, choice1) == 0);

	// Choose a family of curves

//	if (fp_init(choice, choice1) != 0) {
	fpk_init(choice, choice1);
	p_curve Cur;
	pt_curve_fp fp_curve_gen, P;
	pt_curve_fpk fpk_curve_gen, Q;

	k = init_globals(choice, choice1, Cur, fp_curve_gen, fpk_curve_gen);
	P = fp_curve_gen;
	Q = fpk_curve_gen;
	ZZ_pE pairing;
	oldchoice = choice;
	for(;;)
	{
		printf("\nCommand (h for help)> ");
		choice = trapkey();

		// Compute Tate pairing
		if (choice == 't') {
			printf("Computing Tate pairing !\n");
			start = cpucycles();
			pairing = tate(P, Q, k, Cur);
			stop = cpucycles();
			printf("Clock cycles : %lld cycles \n", (stop - start));
		}
		// Computing Weil pairing
		else if (choice == 'w') {
			printf("Computing Weil pairing !\n");
			start = cpucycles();
			pairing = weil(P, Q , k, Cur);
			stop = cpucycles();
			printf("Clock cycles : %lld cycles \n", (stop - start));
		}
		// Change one security level
		else if (choice =='l') {
			print_secu_level();
			do {
				choice = trapkey();

				if (choice == '1') {
					SECU_LEVEL = 128;
					printf("We work on 128-bits security level \n");
					break;
				}
				else if (choice == '2') {
					SECU_LEVEL = 192;
					printf("We work on 192-bits security level \n");
					break;
				}
				else if (choice == '3') {
					SECU_LEVEL = 256;
					printf("We work on 256-bits security level \n");
					break;
				}
				else {
					printf("Unknown command, press '1', '2', or '3' for choosing one security level \n");
				}
			} while (1);
			// Choose a family of curves
			print_curves(choice);
			do {
				choice1 = trapkey();
				if ((choice1 != '1') && (choice1 != '2') && (choice1 != '3'))
					printf("Unknown command, press '1' or '2' or '3' for choosing a pairing-friendly elliptic curves: ");
				else {
					break;
				}
			} while(1);

			fp_init(choice, choice1);
			fpk_init(choice, choice1);
			k = init_globals(choice, choice1, Cur, fp_curve_gen, fpk_curve_gen);
			P = fp_curve_gen;
			Q = fpk_curve_gen;
		}

		// Show the help
		else if (choice == 'h') {
	      print_help();
	    }

		// Generate two new random points for pairing computation
		else if (choice == 'n') {
			printf("Generate two new random points \n");
			P = curvepoint_random_fp(fp_curve_gen, Cur.a);
			cout << "New P: "; //P = [" << P.X << ", " << P.Y << ", " << P.Z << "]" << endl;
			curvepoint_fp_print(P);
			Q = curvepoint_random_fpk(fpk_curve_gen, Cur.a);
			cout << "and new Q : "; // << Q.X << ", " << Q.Y << ", " << Q.Z << endl;
			curvepoint_fpk_print(Q);
		}

		// Change elliptic curve
		else if (choice == 'c') {
			print_curves(oldchoice);
			do {
				choice1 = trapkey();
				if ((choice1 != '1') && (choice1 != '2'))
					printf("Unknown command, press '1' or '2' for choosing a pairing-friendly elliptic curves: ");
				else {
					break;
				}
			} while(1);

			fp_init(oldchoice, choice1);
			fpk_init(oldchoice, choice1);
			k = init_globals(oldchoice, choice1, Cur, fp_curve_gen, fpk_curve_gen);
			P = fp_curve_gen;
			Q = fpk_curve_gen;
	    }
		else if (choice == 's') {
			P = fp_curve_gen;
			Q = fpk_curve_gen;
			cout << "P : "; // << P.X << ", " << P.Y << ", " << P.Z << "]" << endl;
			curvepoint_fp_print(P);
			cout << "and Q : ";// << Q.X << ", " << Q.Y << ", " << Q.Z << endl;
			curvepoint_fpk_print(Q);
		}
		else if (choice == 'p') {
			// Users customize parameters
			cout << endl << "Please enter the characteristic of finite field, p = " << endl;
			cin.getline(string, 1024, '\n' );              // Input goes into string
			charac = to_ZZ(string);
			fp_init0(charac);
			cout << endl << "Please enter the order of subgroup of elliptic curve points, r = " << endl;
			cin.getline(string, 1024, '\n' );
			order = to_ZZ(string);
			cout << "Create an elliptic curve y^2 = x^3 + ax + b, now! " << endl;
			cout << "Please enter the value a = ";
			a = getNumber();
			cout << "Please enter the value b = ";
			b = getNumber();
			curve_init(order, charac, a, b);

			cout << endl << "Please enter the embedding degree k = ";
			k = getNumber();
			fpk_init0(k);
			// Initialize the generator points now
			cout << "Give a generator of elliptic curve over the base finite field !" << endl << "P.X = ";
			cin.getline(string, 1024, '\n');
			P.X = to_ZZ_p(to_ZZ(string));
			cout << "P.Y = ";
			cin.getline(string, 1024, '\n');
			P.Y = to_ZZ_p(to_ZZ(string));
			cout << "P.Z = ";
			cin.getline(string, 1024, '\n');
			P.Z = to_ZZ_p(to_ZZ(string));
			//curvepoint_fp_set_gen(fp_curve_gen, str0, str1, str2);
			ZZ_pE x, y;
			cout << "Give a generator of elliptic curve over the extension finite field !" << endl << "Q.X = ";
			cin.getline(str0, 1024*28, '\n');
			x = fpk_set_str(k, str0);
			cout << "Q.Y = ";
			cin.getline(str0, 1024*28, '\n');
			y = fpk_set_str(k, str0);
			curvepoint_fpk_set_gen(fpk_curve_gen, x, y);
			printf("\nPairing computation of the points (in projective coordinates):\n");
			cout << endl << "P : "; //<< rop.X << ", " << rop.Y << ", " << rop.Z << "]" << endl;
			curvepoint_fp_print(fp_curve_gen);

			cout << endl << "and Q :";// << ropk.X << ", " << ropk.Y << ", " << ropk.Z << "]" << endl;
			curvepoint_fpk_print(fpk_curve_gen);

		}
		// Quit the program
		else if (choice == 'q') {
			clear_globals();
			printf("Exiting gracefully...\n");
			return 0;
		}
		else if (choice != '\n') {
			printf("Unknown command, press 'h' for a list of available commands \n");
		}

	}

}
