// ppm.h
// Solution IJC-DU1, example b), 25.03.2016
// Author: Roman Kiselevich, FIT
// Compiler: gcc 4.8.4
// 	header file for module ppm.c  

#ifndef _PPM_H
#define _PPM_H

/*
 * Structure for storing ppm image
 * .PPM format: P6 <ws>+ xsize <ws>+
 * 				ysize <ws>+ 255 <ws> <BinData>
 */
struct ppm {
	unsigned xsize;
	unsigned ysize;
	char data[];    // RGB bytes total: 3*xsize*ysize 
};

/*
 * initializes using a dynamic memory and reads 
 * the ppm image, returns a pointer to a ppm structure
 * in case of error returns NULL
 */
struct ppm *ppm_read(const char *filename);

/*
 * image content writes to a file with PPM format  
 * in case of error returns a negative number
 */
int ppm_write(struct ppm *p, const char *filename);


#endif 