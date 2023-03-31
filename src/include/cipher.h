#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define ROUNDS 50
#define MAX_LEN 100

const char charmap[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/!?@#$%^&*()_+{}[]|\\:;\"'<>,.~`-=";

char* pad(const char* text) {
    int padding = 3 - (strlen(text) % 3);
    if (padding == 0)
    return strdup(text);
    char* result = strdup(text);
    for (int i = 0; i < padding; i++)
    result = strcat(result, " ");
    return result;
}

char* enc(const char* text, const char* key) {
    char* result = (char*) malloc(sizeof(char) * (4 * ceil(strlen(text) / 3.0) + 1));
    char* paddedText = pad(text);
    int keyIndex = 0;
    int resultIndex = 0;

    for (int i = 0; i < strlen(paddedText); i += 3) {
        int n = (paddedText[i] << 16) | (paddedText[i + 1] << 8) | paddedText[i + 2];
        for (int j = 0; j < ROUNDS; j++) {
            n ^= (int)key[keyIndex++ % strlen(key)];
        }
        result[resultIndex++] = charmap[n >> 18];
        result[resultIndex++] = charmap[(n >> 12) & 63];
        result[resultIndex++] = charmap[(n >> 6) & 63];
        result[resultIndex++] = charmap[n & 63];
    }

    // Null terminate the result string
    result[resultIndex] = '\0';
    free(paddedText);
    return result;
}

char* unpad(const char* text) {
    int padding = 0;
    for (int i = strlen(text) - 1; text[i] == ' '; i--)
    padding++;
    return strndup(text, strlen(text) - padding);
}

char* dec(const char* text, const char* key) {
    char* result = (char*) malloc(sizeof(char) * (ceil(strlen(text) / 4.0) * 3 + 1));
    int keyIndex = 0;
    int resultIndex = 0;

    for (int i = 0; i < strlen(text); i += 4) {
        int n = strchr(charmap, text[i]) - charmap;
        n = (n << 6) | (strchr(charmap, text[i + 1]) - charmap);
        n = (n << 6) | (strchr(charmap, text[i + 2]) - charmap);
        n = (n << 6) | (strchr(charmap, text[i + 3]) - charmap);
        for (int j = 0; j < ROUNDS; j++) {
            n ^= (int)key[keyIndex++ % strlen(key)];
        }
        result[resultIndex++] = (n >> 16) & 255;
        result[resultIndex++] = (n >> 8) & 255;
        result[resultIndex++] = n & 255;
    }

    // Null terminate the result string
    result[resultIndex] = '\0';
    char* unpaddedResult = unpad(result);
    free(result);
    return unpaddedResult;
}


// Init function declarations
char* enc(const char*, const char*);
char* dec(const char*, const char*);
extern char* enc(const char*, const char*);
extern char* dec(const char*, const char*);