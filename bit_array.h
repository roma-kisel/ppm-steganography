// bit_array.h
// Solution IJC-DU1, example a), 25.03.2016
// Author: Roman Kiselevich, FIT
// Compiler: gcc 4.8.4
// 	Module to working with bit-arrays

#ifndef _BIT_ARRAY_H
#define _BIT_ARRAY_H

#include <limits.h> // CHAR_BIT
#include "error.h" // fatal_error()

typedef unsigned long* bit_array_t;

// The number of bits in ulong type
#define ULONG_BIT (CHAR_BIT*sizeof(unsigned long))

// The number of bits in a single element of the array
#define BIT_PER_ARRELEM(name) (CHAR_BIT*sizeof(name[0]))

/* 
 * some useful macros to manipulation with bits
 * supports any integer type
 */
#define BITMASK(name, index) (1ULL << ((index) % BIT_PER_ARRELEM(name)))
#define BITSLOT(name, index) (((index) / BIT_PER_ARRELEM(name)))

// returns the value of the bit located in the array at bit-index
#define DU1_GET_BIT(name, index) (((name)[BITSLOT(name, index)] & BITMASK(name, index)) ? 1U : 0U)

// if bit !=0 set 1 on the bit-index otherwise set 0
#define DU1_SET_BIT(name, index, bit) do {						     			 \
			if ((bit)!=0) ((name)[BITSLOT(name, index)] |= BITMASK(name, index));\
			else ((name)[BITSLOT(name, index)] &= ~BITMASK(name, index));	     \
		} while(0)
// end of DU1_SET_BIT

// calculate the number of ulong elements for an effective storage bits
#define ULONG_COUNT_BA(size) (((size)+(CHAR_BIT*sizeof(unsigned long))-1U)/(CHAR_BIT*sizeof(unsigned long)))

/* 
 * declare and initialize the array and initializes it with zeros. 
 * for storage bit-length array used [0] element
 */
#define ba_create(name, size) unsigned long name[ULONG_COUNT_BA(size)+1U] = {size, 0,}							

#ifndef USE_INLINE

// returns the declared size of the array
#define ba_size(name) (name[0])

// similarly, as DU1_SET_BIT, but with checking the allowable range
#define ba_set_bit(name,index,bit) do {										\
			if ((index) >= 0 && (index) < ba_size(name))					\
				DU1_SET_BIT(name, ((index)+(BIT_PER_ARRELEM(name))), bit);	\
			else															\
				fatal_error("index %lu is out of range 0..%lu\n", 			\
					(unsigned long)index, (unsigned long)(ba_size(name)-1));\
		}while(0)
// end of ba_set_bit(name,index,bit)

// similarly, as DU1_GET_BIT, but with checking the allowable range
#define ba_get_bit(name,index) 					 						   \
		(((index) < 0 || (index) >= ba_size(name)) ? 				       \
		fatal_error("index %lu is out of range 0..%lu\n", 				   \
			(unsigned long)(index), (unsigned long)(ba_size(name)-1)), 0 : \
		DU1_GET_BIT(name,((index)+(BIT_PER_ARRELEM(name)))))

#else 

// Declaration some similar inline functions.

static inline unsigned long ba_size(bit_array_t barr) {
	return barr[0];
}

static inline void ba_set_bit(bit_array_t barr, unsigned long index, unsigned bit) {
	if (index < ba_size(barr)) {
		DU1_SET_BIT(barr, index+BIT_PER_ARRELEM(barr), bit);
	}
	else {
		fatal_error("index %lu is out of range 0..%lu\n",
					(unsigned long)index, (unsigned long)(ba_size(barr)-1));
	}
}

static inline unsigned ba_get_bit(bit_array_t barr, unsigned long index) {
	if (index >= ba_size(barr)) {
		fatal_error("index %lu is out of range 0..%lu\n",
					(unsigned long)index, (unsigned long)(ba_size(barr)-1));
	}

	return DU1_GET_BIT(barr, index+BIT_PER_ARRELEM(barr));
}

#endif // USE_INLINE

#endif // _BIT_ARRAY_H