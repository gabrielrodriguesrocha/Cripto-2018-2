#include <stdio.h>
#include <stdlib.h>
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
    unsigned long long Er, Sk;
    unsigned char block, row, col;
    unsigned int i, res;
    unsigned long long bit_selection = 0x3f;
    Er = general_permutation(R, E, 32, 48);
    Sk = Er ^ K;
    for (i = 0, res = 0, row = 0, col = 0; i < 8; i++) {
        block = (Sk & (bit_selection << (42 - i * 6))) >> (42 - i * 6);
        row = (((block & 0x20) >> 5) << 1) | (block & 0x1); // first and last bits
        col = (block & 0x1e) >> 1; // middle bits
        res = (sbox[i][row * 16 + col]) << (32 - 4*(i + 1)) | res;
    }
    return general_permutation(res, P, 32, 32);
}

/* ---- DES Cipher --- */
// Runs through 16 rounds of ciphering action
unsigned long long des(unsigned long long input) {
    unsigned int right, left, aux;
    int i;
    printf("Plaintext: %llX\n", input);
    
    /* ---- Initial Permutation ---- */
    input = general_permutation(input, IP, 64, 64);
    printf("Initial Permutation: %llX\n\n", input);
    
    /* ---- Feistel Network ---- */
    left  = input >> 32;
    right = input;

    for(i = 0 ; i < 16 ; i ++){
      printf("Round %02d:\n", i + 1);
      printf("Key used = %llX\n", keys[i]);
      printf("Left side = %08llX\n", left);
      printf("Right side = %08llX\n\n", right);
      aux   = left;
      left  = right;
      right = aux ^ f(right, keys[i]);
    }

    /* ---- Final Permutation ---- */
    input = (unsigned long long) right << 32 | left;
    printf("Swap: %llX\n\n", input);
    input = general_permutation(input, IPinverse, 64, 64);
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
       printf("\nCiphered text: %llX\n", des(buffer));
     }

    fclose(fk);
    fclose(fp);

    return 0;
}
