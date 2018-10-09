#include <stdio.h>
#include <stdlib.h>
#include "tables.h"

unsigned long long keys[16];

unsigned long long general_permutation(unsigned long long R, const unsigned char table[], int in_size, int out_size) {
    unsigned long long res = 0;
    return res;
}

void generate_keys(unsigned long long key) {
}

unsigned int f(unsigned int R, unsigned long long K) {
    unsigned int res;
    return res;
}

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

    while (fscanf(fp, "%llx", &buffer) != EOF) {
        printf("%llx", des(buffer));
    }

    return 0;
}