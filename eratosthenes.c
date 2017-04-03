// eratosthenes.c
// Solution IJC-DU1, example a), 25.03.2016
// Author: Roman Kiselevich, FIT
// Compiler: gcc 4.8.4
// 	The module contains an implementation of the 
// algorithm for finding prime numbers in the specified 
// range that known as the Sieve of Eratosthenes

#include "bit_array.h"
#include "eratosthenes.h"

void Eratosthenes(bit_array_t barr)  {
	for (unsigned i = 0; i*i < ba_size(barr); ++i) {
		if (i < 2) ba_set_bit(barr, i, 1);

		if (ba_get_bit(barr, i) == 0) {
			unsigned n = 0;
			unsigned j = i*i;
			while (j < ba_size(barr)) {
				ba_set_bit(barr, j, 1);
				++n;
				j = i*(i + n);
			}
		}
	} // for
}