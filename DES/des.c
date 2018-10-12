#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tables.h"

unsigned long long keys[16];

char plainText[]  = "ABCDEFGH";
char globalKey[]  = "01230123";

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

void permutation(char binary[], const unsigned char table[], int out_size){
  char aux[out_size + 1];
  int i;
  for(i = 0 ; i < out_size ; i++){
    aux[i] = binary[table[i]-1];
  }
  aux[i] = '\0';
  strcpy(binary, aux);
}

void string_to_binary(char input[], char binary[]){
  int i, j, contador = 0;
  char character, aux[8];
  char *output = (char*)calloc(65, sizeof(char));
  for(i = 0 ; i < strlen(input) ; i++){
    character = input[i];

    for(j = 7; j >= 0; --j){
      (character & 1 << j) ?  strcat(output, "1") : strcat(output, "0");
      contador++;
    }

  }
  output[contador] = '\0';
  strcat(binary, output);
  free(output);
}

void key_schedule(char rightKey[], char leftKey[]){
  char *keybinary = (char*)calloc(65, sizeof(char));
  keybinary[64] = '\0';

  string_to_binary(globalKey, keybinary);
  permutation(keybinary, PC1, 56);
  strncpy(rightKey, keybinary, 28);
  strncpy(leftKey, keybinary + 28, 28);
  rightKey[28] = '\0';
  leftKey[28] = '\0';
}

void rotate(char leftKey[], char rightKey[]){
  int i;
  char aux;
  for(i = 0 ; i < 27 ; i++){
    aux = leftKey[i];
    leftKey[i] = leftKey[i+1];
    leftKey[i+1] = aux;
    aux = rightKey[i];
    rightKey[i] = rightKey[i+1];
    rightKey[i+1] = aux;
  }
}

int main (int argc,char *argv[]) {
    FILE * fp;
    FILE * fk;
    unsigned long long buffer, key;

    char left[33], right[33], leftKey[29], rightKey[29], binaryKey[57];
    char *binary = (char*)calloc(65, sizeof(char));
    int rodada, i;

    if (argc != 3) {
        printf("USAGE: des KEYFILE FILENAME\n");
        return 0;
    }

    fk = fopen(argv[1], "r");
    fp = fopen(argv[2], "r");
    fscanf(fk, "%llx", &key);
    generate_keys(key);

    // while (fscanf(fp, "%llx", &buffer) != 0) {
    //     printf("%llx", des(buffer));
    // }

    /* ---- Initial Permutation ---- */
    string_to_binary(plainText, binary);
    permutation(binary, IP, 64);

    /* ---- Feistel Network --- */
    strncpy(left, binary, 32);
    strncpy(right, binary + 32, 32);
    left[32] = '\0';
    right[32] = '\0';

    // Initial Key Schedule
    key_schedule(leftKey, rightKey);
    for(rodada = 0 ; rodada < 16 ; rodada++){
      for(i = 0 ; i < KSROT[i] ; i++)
        rotate(leftKey, rightKey);

      strcpy(binaryKey, leftKey);
      strcat(binaryKey, rightKey);

      permutation(binaryKey, PC_2, 48);
    }
    return 0;
}
