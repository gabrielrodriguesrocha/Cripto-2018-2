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
    unsigned int S[9];

    res = general_permutation((unsigned long long) R, E, 32, 48);
    res = res ^ K;

    S[8] = res & 0x3f
    S[7] = res >> 6  & 0x3f
    S[6] = res >> 12 & 0x3f
    S[5] = res >> 18 & 0x3f
    S[4] = res >> 24 & 0x3f
    S[3] = res >> 30 & 0x3f
    S[2] = res >> 36 & 0x3f
    S[1] = res >> 42 & 0x3f

    S[8] = sbox8[((S[8] >> 5) << 1) & (S[8] & 0x1)][(S[8] >> 1) & 0xf];
    S[7] = sbox7[((S[7] >> 5) << 1) & (S[8] & 0x1)][(S[7] >> 1) & 0xf];
    S[6] = sbox6[((S[6] >> 5) << 1) & (S[8] & 0x1)][(S[6] >> 1) & 0xf];
    S[5] = sbox5[((S[5] >> 5) << 1) & (S[8] & 0x1)][(S[5] >> 1) & 0xf];
    S[4] = sbox4[((S[4] >> 5) << 1) & (S[8] & 0x1)][(S[4] >> 1) & 0xf];
    S[3] = sbox3[((S[3] >> 5) << 1) & (S[8] & 0x1)][(S[3] >> 1) & 0xf];
    S[2] = sbox2[((S[2] >> 5) << 1) & (S[8] & 0x1)][(S[2] >> 1) & 0xf];
    S[1] = sbox1[((S[1] >> 5) << 1) & (S[8] & 0x1)][(S[1] >> 1) & 0xf];

    res = (S[1] << 42) & (S[2] << 36) & (S[3] << 30) & (S[4] << 24) &
          (S[5] << 18) & (S[6] << 12) & (S[7] <<  6) & (S[8]);

    res = (unsigned int)general_permutation((unsigned long long)res, P, 32, 32);

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
    unsigned long long right, left;
    if (argc != 3) {
        printf("USAGE: des KEYFILE FILENAME\n");
        return 0;
    }

    fk = fopen(argv[1], "r");
    fp = fopen(argv[2], "r");
    fscanf(fk, "%llx", &key);
    generate_keys(key);

     while (fscanf(fp, "%llx", &buffer) != EOF) {
       /* ---- Initial Permutation ---- */
       buffer = general_permutation(buffer, IP, 64, 64);

       printf("%llx\n", des(buffer));

       /* ---- Final Permutation ---- */
     }

    return 0;
}
