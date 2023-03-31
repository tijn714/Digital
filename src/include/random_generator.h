#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CHARMAP "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"

char *generate_key(int length) {
    char *key = (char *) malloc(sizeof(char) * (length + 1));
    srand(time(NULL));
    for (int i = 0; i < length; i++) {
        key[i] = CHARMAP[rand() % (sizeof(CHARMAP) - 1)];
    }
    key[length] = '\0';
    return key;

    free(key);
}