/*
 * utils.c
 *
 *  Created on: Apr 14, 2011
 *      Author: dple
 */

#include<stdio.h>
#include <iostream>
#include <string>
#include <sstream>

#include "utils.h"

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
				printf("Unknown command, press 'h' for a list of available commands");
		}
		choice = (char) c1;
	}
	while(ctr != 1);
	return choice;
}

int getNumber() {
	int myNumber = 0;
	string input = "";

	while (true) {
		//cout << "Please enter a valid number: ";
		getline(cin, input);

		// This code converts from string to number safely.
		stringstream myStream(input);
		if (myStream >> myNumber)
			break;
		cout << "Invalid number, please try again" << endl;
	}
	return myNumber;
}
/*
#ifdef _WIN32

#include <time.h>

static inline double GetCurrTime()
{
	return clock() / double(CLOCKS_PER_SEC);
}
#else

#include <sys/time.h>
#include <stdio.h>

static inline double GetCurrTime()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec + (double) tv.tv_usec * 1e-6;
}
#endif

static inline uint64 getRdtsc()
{
	#ifdef _MSC_VER
		return __rdtsc();
	#else
		unsigned int eax, edx;
		__asm__ volatile("rdtsc" : "=a"(eax), "=d"(edx));
		return ((uint64)edx << 32) | eax;
	#endif
}
*/

// Accepts a decimal integer and returns a binary coded string
char* dectobin(long i) {
	char* tmp = "";
	while( i > 0 ) {
		/* bitwise AND operation with the last bit */
		(i & 0x1) ? (*tmp++='1') : (*tmp++='0');
		/* bit shift to the right, when there are no bits left the value is 0, so the loop ends */
		i >>= 1;
	}
	return tmp;
}

// Show a decimal number in binary
void showbits(int h) {
	if (h==1)
		printf("%d ",h);
	else {
		showbits(h/2);
		printf("%d ",h%2);
	}
}

// Count the number of 1-bits, the Hamming weight for an integer
int hammingWeight(unsigned int n) {
   int count = 0;
   while (n) {
      count += n & 0x1u;
      n >>= 1;
   }
   return count;
}

// Calculate the Hamming weight for an big integer in ZZ_p

int hammingWeightZZ(ZZ r) {
	int n, i;
	int count = 0;
	n = NumBits(r);

	for (i = n; i >= 0; i--)  {
		if(bit(r, i))
			count++;
	}
	return count;
}

/*
 * Count the number of bits
 */
int bitcount(unsigned int n) {
	int count = 8 * sizeof(int);
	unsigned int tmp = 1;
	tmp <<= count - 1;

	do {
		count--;
		tmp >>=1;
	}while (!(n & tmp)) ;
	return count;
}

// If the bit at position pos is 1, function return true. Else return false
bool bit(int n, int pos) {
	int _bit = 1;
	_bit <<= pos - 1;
	return (n & _bit ? true : false);
}

// Extract a chain of bit from a number n
int extract(int n, int tail, int size) {
	int i, ret = 1;

	for (i = 1; i < size; i++) {
		ret <<= 1;
		ret++;
	}
	ret <<= tail;
	ret = ret & n;
	ret >>= tail;
	return ret;
}
// ROTATE_RIGHT rotates T left n bits.
/*
template<typename T> inline T rotate_right(T input, u32 n) {
	return static_cast<T>((input >> n) | (input << (8*sizeof(T)-n)));
}

/*
 * ROTATE_LEFT rotates T left n bits.
 */
/*
template<typename T> inline T rotate_left(T input, u32 rot) {
	return static_cast<T>((input << rot) | (input >> (8*sizeof(T)-rot)));
}
*/
