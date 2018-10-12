#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tables.h"

unsigned long long keys[16];


/* ---- Generalized Permutation --- */
// Calculates the permutation of a value R according to a table, taking account the in and out sizes of the permutation
// Can be used with R whose type is unsigned int -- upcasting in C hah
unsigned long long general_permutation(unsigned long long R, const unsigned char table[], int in_size, int out_size) {
    unsigned long long res = 0;
    for (int i = 0; i < out_size; i++) { 
        res = res | (((R >> (in_size - table[i])) & 1) << (out_size - i - 1));
    }
    return res;
}

/* ---- Key Schedule --- */
// Generates all keys and stores them in the array keys
void generate_keys(unsigned long long key) {
  unsigned int i, L, R;

  for (i = 0, L = 0, R = 0; i < 28; i++) {
      L = L | (((key >> (64 - PC1L[i])) & 1) << (28 - i - 1));
      R = R | (((key >> (64 - PC1R[i])) & 1) << (28 - i - 1));
  }
  
  for (i = 0; i < 16; i++) {
      L = (L << KSROT[i] | L >> (28 - KSROT[i])) & 0xfffffff;
      R = (R << KSROT[i] | R >> (28 - KSROT[i])) & 0xfffffff;
      keys[i] = general_permutation((unsigned long long) L << 28 | R, PC2, 56, 48);
  }
}

/* ---- Feistel Function --- */
// Expands the input and does 8 rounds of S-box permutations
unsigned int f(unsigned int R, unsigned long long K) {
    unsigned int res;
    return res;
}

/* ---- DES Cipher --- */
// Runs through 16 rounds of ciphering action
unsigned long long des(unsigned long long input) {
    return input;
}


int main (int argc,char *argv[]) {
    FILE * fp;
    FILE * fk;
    unsigned long long buffer, key;

    if (argc != 3) {
        printf("USAGE: des KEYFILE FILENAME\n");
        return 0;
    }

    fk = fopen(argv[1], "r");
    fp = fopen(argv[2], "r");
    fscanf(fk, "%llx", &key);
    generate_keys(key);

     while (fscanf(fp, "%llx", &buffer) != 0) {
         printf("%llx", des(buffer));
     }

    return 0;
}
