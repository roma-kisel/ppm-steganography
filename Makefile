# Makefile
# Solution IJC-DU1
# Author: Roman Kiselevich, FIT
# Compiler: gcc 4.8.4


CC = gcc
CFLAGS = -std=c99 -pedantic -O2 -Wall
INLINE = -DUSE_INLINE
EXEC_FILES = primes primes-inline steg-decode


# primes primes-inline steg-decode
all: $(EXEC_FILES)

error.o: error.c 
	$(CC) $(CFLAGS) -c error.c  

eratosthenes.o: eratosthenes.c
	$(CC) $(CFLAGS) -c eratosthenes.c

#primes 
primes: primes.o error.o eratosthenes.o bit_array.h
	$(CC) $(CFLAGS) -o primes primes.o error.o eratosthenes.o 

primes.o: primes.c
	$(CC) $(CFLAGS) -c primes.c  

#primes inline
primes-inline: primes-inline.o error.o eratosthenes-inline.o bit_array.h
	$(CC) $(CFLAGS) -o primes-inline primes-inline.o error.o eratosthenes-inline.o

eratosthenes-inline.o: eratosthenes.c 
	$(CC) $(CFLAGS) $(INLINE) -c -o eratosthenes-inline.o eratosthenes.c 

primes-inline.o: primes.c
	$(CC) $(CFLAGS) $(INLINE) -c -o primes-inline.o primes.c 

#steg decode 
steg-decode: ppm.o error.o eratosthenes.o steg-decode.o bit_array.h
	$(CC) $(CFLAGS) -o steg-decode ppm.o error.o eratosthenes.o steg-decode.o

steg-decode.o: steg-decode.c 
	$(CC) $(CFLAGS) -c steg-decode.c

ppm.o: ppm.c 
	$(CC) $(CFLAGS) -c ppm.c

clean: 
	rm -f *.o $(EXEC_FILES)