// ppm.c
// Solution IJC-DU1, example b), 25.03.2016
// Author: Roman Kiselevich, FIT
// Compiler: gcc 4.8.4
// Module for working with ppm images

#include "ppm.h"
#include "error.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAX_FILE_NAME_LENGTH 80u
#define SIZE_LIMIT 5000u

/*
 * copy string from src
 * and store it in dest with ppm format
 * For example src:"str" => dest:"str.ppm"
 *
 * dest length is limited to 80(max filename)
 * In case of error return 1 , otherwise return 0
 */
static int fadd_ppmfmt(char *dest, const char *src) {
	char *ppmfmt = ".ppm";
	if (strlen(src) > MAX_FILE_NAME_LENGTH-strlen(ppmfmt)) 
		return 1;
	
	strcpy(dest, src);
	strcat(dest, ppmfmt);
	return 0;
}

struct ppm *ppm_read(const char *filename) {
	/*.ppm format: P6 <ws>+ xsize <ws>+ ysize <ws>+ 
	 * 255 <ws> <BD> <EOF> 
	 */
	char *badformat = "File format does not fit .ppm\n";

	FILE *fp = fopen(filename, "rb"); 
	if (fp == NULL) {
		warning_msg("Open a file passed unsuccessfully\n");
		return NULL;
	}

	// P6 check
	char buffer_P6[3] = {0};
	if ((fscanf(fp, "%2s", buffer_P6)) == EOF)  {
		warning_msg(badformat);
		return NULL;
	}
	if (strcmp(buffer_P6, "P6")){
		warning_msg(badformat);
		return NULL;
	}
	// end of P6 check 

	/*	FSM to read .ppm
	 * 0 - white space and exp_event processing
	 * 1 - xsize 
	 * 2 - ysize 
	 * 3 - # notes
	 * -1 - exit state
	 */
	enum { XSIZE, YSIZE, RGB_255} expected_event = XSIZE;

	int nextch;
	unsigned state = 0;
	unsigned xsize = 0, ysize = 0; 
	unsigned note_counter = 0;
	while (state != -1) {
		if ((nextch = fgetc(fp)) == EOF) {
			warning_msg(badformat);
			return NULL;
		}

		switch (state) {
			case 0:
				if (isspace(nextch)) break;
				else if (isdigit(nextch)) {
					switch (expected_event) {
						case XSIZE:
							state = 1;
							break;
						case YSIZE:
							state = 2;
							break;
						case RGB_255:
							state = -1;
							break;
					}
				}
				else if (nextch == '#') state = 3;
				else {
					warning_msg(badformat);
					return NULL;
				}

				// to save the next char
				fseek(fp, -1, SEEK_CUR);
				break;
			case 1:
				if (!isdigit(nextch)) {
					state = 0;
					fseek(fp, -1, SEEK_CUR); 
					break;
				}

				if (xsize) xsize *= 10;
				else ++expected_event;

				xsize += nextch - '0';
				if (xsize > SIZE_LIMIT) {
					warning_msg("%s\txsize should be less or equal to %u\n", 
													badformat, SIZE_LIMIT);
					return NULL;
				}
				break;
			case 2:
				if (!isdigit(nextch)) {
					state = 0;
					fseek(fp, -1, SEEK_CUR); 
					break;
				}

				if (ysize) ysize *= 10;
				else ++expected_event;

				ysize += nextch - '0';
				if (ysize > SIZE_LIMIT) {
					warning_msg("%s\tysize should be less or equal to %u\n", 
													badformat, SIZE_LIMIT);
					return NULL;
				}
				break;
			case 3:
				if (!note_counter)
					printf("I found some notes in the image...\n");
				++note_counter;

				printf("   note #%u: ", note_counter);
				while ((nextch = fgetc(fp)) != '\n') {
					if (nextch == EOF) {
						warning_msg(badformat);
						return NULL;
					}

					putchar(nextch);
				}
				putchar('\n');
				state = 0;
				break;
		} //switch state 
	} //while 

	// RGB 255 check 
	char buffer_255[3] = {0};
	if ((fscanf(fp, "%3s", buffer_255)) == EOF)  {
		warning_msg(badformat);
		return NULL;
	}
	if (strcmp(buffer_255, "255")){
		warning_msg(badformat);
		return NULL;
	}
	// end of 255 check

	// last ws check
	if ( !isspace((nextch = fgetc(fp))) ) {
		warning_msg(badformat);
		return NULL;
	}

	// image header initialization
	size_t init_size = sizeof(struct ppm) + 3*xsize*ysize;
	struct ppm *img = malloc(init_size);
	if (img == NULL) {
		warning_msg("allocation has passed unsuccessfully\n");
		return NULL;
	}
	img->xsize = xsize;
	img->ysize = ysize;
	unsigned num_bytes = 3*xsize*ysize;
	// end of image header iinitialization

	// reading binary data
	if (fread(img->data, 1, num_bytes, fp) != num_bytes) {
		warning_msg("File reading passed unsuccessfully\n");
		return NULL;
	}

	fseek(fp, 1, SEEK_CUR); // fp points to next char
	if (feof(fp)) warning_msg(badformat); // !EOF ?

	if (fclose(fp)) {
		warning_msg("closing a file passed unsuccessfully\n");
		return NULL;
	}

	return img;
}

int ppm_write(struct ppm *p, const char *filename) {
	char fnameppm[MAX_FILE_NAME_LENGTH+1] = {0};
	if (fadd_ppmfmt(fnameppm, filename)) {
		warning_msg("file name length is too long\n");
		return -1;
	}

	FILE *fp = fopen(fnameppm, "wb");
	if (fp == NULL) {
		warning_msg("open file error\n");
		return -1;
	}

	if (fprintf(fp, "P6\n%u\n%u\n255\n", p->xsize, p->ysize) < 0) {
		warning_msg("writing to a file passed unsuccessfully\n");
		return -1;
	}

	unsigned num_bytes = 3*p->xsize*p->ysize;
	if (fwrite(p->data, 1, num_bytes, fp) != num_bytes) {
		warning_msg("writing to a file passed unsuccessfully\n");
		return -1;
	}

	if (fclose(fp)) {
		warning_msg("closing a file passed unsuccessfully\n");
		return -1;
	}

	return 0;
}