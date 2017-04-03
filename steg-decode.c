// steg-decode.c 
// Solution IJC-DU1, example b), 25.03.2016
// Author: Roman Kiselevich, FIT
// Compiler: gcc 4.8.4
// 	Program to decode the hidden messages 
// in the ppm images format.


#include "bit_array.h"
#include "eratosthenes.h"
#include "ppm.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define SIZE_LIMIT 75000000u

int main(int argc, char *argv[]) {
	if (argc != 2) fatal_error("I'll waiting for a file\n");

	struct ppm *image = ppm_read(argv[1]);
	if (image == NULL) 
		fatal_error("reading from file passed unsuccessfully..\n");


	ba_create(myarr, SIZE_LIMIT/2);
	Eratosthenes(myarr);

	unsigned bin_data_length = 3*image->xsize*image->ysize;

	unsigned bit_index = 0;
	unsigned ch_buffer[1] = {0};
	for (unsigned i = 0; i < bin_data_length; ++i) {
		if (ba_get_bit(myarr, i)) continue;

		unsigned bit = DU1_GET_BIT(image->data, CHAR_BIT*i);

		DU1_SET_BIT(ch_buffer, bit_index, bit);
		++bit_index;
		if (bit_index%CHAR_BIT == 0) {
			if (isprint(ch_buffer[0])) 
				putchar(ch_buffer[0]);
			else if (ch_buffer[0] == '\0') 
				break;
			else {
				free(image);
				fatal_error("decoding is unsuccessful (bad format)\n");
			}

			bit_index = 0;
		}
	}
	putchar('\n');

	free(image);
	return 0;
}