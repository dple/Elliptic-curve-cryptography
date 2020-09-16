// get_parameters.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <cmath>
#include <cstring>
#include <fstream>

using namespace std;

/*
 * Print help for choosing a family of pairing-friendly elliptic curves
 */


char trapkey(void) {
	int c1, c2, ctr;
	char choice;

	do {
		ctr = 0;
		//printf("\nCommand (h for help)> ");
		c1 = getchar();
		if(c1 != '\n' && c1 != EOF) {
			++ctr;
			while((c2 = getchar()) != EOF && c2 != '\n')
				++ctr;
			if(ctr != 1)
				cout << "Unknown command, press 'h' for a list of available commands";
		}
		choice = (char) c1;
	}
	while(ctr != 1);
	return choice;
}


/*
 * Print help for choosing a security level
 */
char choose_secu(void) {
	char c;
	cout << "\nPlease choose a security level: \n";
	cout << "1. 80-bits security level \n";
	cout << "2. 112-bits security level \n";
	cout << "3. 128-bits security level \n";
	cout << "4. 192-bits security level \n";
	
	do {
		c = trapkey();
		if ((c != '1') && (c != '2') && (c != '3') && (c != '4'))
			cout << "\nUnknown command, press '1' or '2' for choosing a pairing-friendly elliptic curves: ";
		else 
			return c;
	} while(1);	
}

char choose_curves(void) {
	char c;
	cout << "\nPlease choose a method to generate pairing-friendly elliptic curves, i.e, MNT, Cocks-Pinch, Brezing-Weng (1, 2, 3): \n";
	cout << "1. MNT families of elliptic curves (k = 3, 4, 6)\n";
	cout << "2. Cocks-Pinch pairing-friendly elliptic curves (for any embedding degree, rho-value = 2) \n";
	cout << "3. Brezing-Weng families of elliptic curves (for any embedding degree, 1 < rho-value < 2) \n";
	do {
		c = trapkey();
		if ((c != '1') && (c != '2') && (c != '3'))
			cout << "\nUnknown command, press '1' or '2' for choosing a pairing-friendly elliptic curves: ";
		else 
			return c;
	} while(1);	
}

int choose_p(void) {
	int i;
	//char szInput [4];
	cout << "Enter the size of finite field p: ";
	//fgets(szInput, 4, stdin);
	//i = atoi(szInput);
	cin >> i;
  return i;
}


int choose_k(char c) {
	int k;
	cout << "Enter an embedding degree k. ";
	if (c == '1') 
		cout << "You choose MNT curves, thus the embedding degree must be 3, 4, or 6:";
	else { 
		if (c == '2')
			cout << "You choose Cocks-Pinch curves, thus the embedding degree must be less than 25:";
		else
			cout << "You choose Brezing Weng curves, thus the embedding degree must be less than 50:";
	}
	cin >> k;
  return k;
}


int main(int argc,char **argv)
{
	ofstream myfile;
	myfile.open ("parameters.txt");
	char c_curve, c_secu;
	int k, sizep;
	c_curve = choose_curves();
	myfile << "// The method to generate pairing-friendly elliptic curves is:\n";
	myfile << "c := " << c_curve << ";\n";
	sizep = choose_p();
	myfile << "// The size of the finite field is:\n";
	myfile << "sp := " << sizep << ";\n";
	k = choose_k(c_curve);
	myfile << "// The embedding degree k.\n";
	myfile << "k := " << k << ";\n";
	myfile.close();	
	return 0;
}


