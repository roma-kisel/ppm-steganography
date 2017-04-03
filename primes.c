// primes.c
// Solution IJC-DU1, example a), 25.03.2016
// Author: Roman Kiselevich, FIT
// Compiler: gcc 4.8.4
// 	The program calculates the prime numbers 
// in a range, 0..MAX_NUMBER
// and output the last 10 primes in this range


#include "bit_array.h"
#include "eratosthenes.h"
#include <stdio.h>

#define MAX_NUMBER 202000000u
#define MAX_PRIME_COUNT 10

int main() {
	ba_create(myarr, MAX_NUMBER);
	Eratosthenes(myarr);

	unsigned primes_count = 0;
	for (unsigned i = ba_size(myarr)-1; 
			primes_count != MAX_PRIME_COUNT; --i) {
		
		if (!ba_get_bit(myarr, i)) {
			++primes_count;
			printf("%u\n", i);
		}
	}
	return 0;
}