#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include "include/cipher.h"
#include "include/random_generator.h"


#ifdef _WIN32
    #define OS_NAME "Windows"
    #define OPEN_COMMAND "start %s"
    #define CLOSE_COMMAND "taskkill /IM firefox.exe"
#elif __APPLE__
    #define OS_NAME "macOS"
    #define OPEN_COMMAND "open %s"
    #define CLOSE_COMMAND "killall firefox"
#else
    #define OS_NAME "Linux"
    #define OPEN_COMMAND "xdg-open %s"
    #define CLOSE_COMMAND "pkill firefox"
#endif

#ifdef _WIN32
    #define TASKLIST_COMMAND "tasklist /FI \"WINDOWTITLE eq %s\" /FI \"IMAGENAME eq firefox.exe\""
#elif __APPLE__
    #define PS_COMMAND "ps -ax | grep firefox | grep -v grep | awk '{print $1}'"
#else
    #define PS_COMMAND "ps -ef | grep firefox | grep -v grep | awk '{print $2}'"
#endif


void open() {
    // Determine the name of the HTML file
    char filename[] = "page/index.html";

    // Determine the command to open the file based on the operating system
    char command[1024];
    sprintf(command, OPEN_COMMAND, filename);

    // Execute the command
    printf("Opening '%s' on %s...\n", filename, OS_NAME);
    system(command);
}

int main(int argc, char *argv[]) {
    printf("Please enter the SK1: ");    
    char *sk1 = malloc(1000);
    scanf("%s", sk1);

    printf("Please enter the key for SK1: ");
    char *key1 = malloc(1000);
    scanf("%s", key1);


    printf("Please enter the SK2: ");
    char *sk2 = malloc(1000);
    scanf("%s", sk2);

    printf("Please enter the key for SK2: ");
    char *key2 = malloc(1000);
    scanf("%s", key2);

    // Round 1
    char *sk1_enc_round1 = enc(sk1, key1);
    char *sk2_enc_round1 = enc(sk2, key2);


    // Round 2
    char *sk1_enc_round2 = enc(sk1_enc_round1, key1);
    char *sk2_enc_round2 = enc(sk2_enc_round1, key2);

    // Round 3
    char *sk1_enc_round3 = enc(sk1_enc_round2, key1);
    char *sk2_enc_round3 = enc(sk2_enc_round2, key2);

    // Round 4
    char *sk1_enc_round4 = enc(sk1_enc_round3, key1);
    char *sk2_enc_round4 = enc(sk2_enc_round3, key2);

    // Round 5
    char *sk1_enc_round5 = enc(sk1_enc_round4, key1);
    char *sk2_enc_round5 = enc(sk2_enc_round4, key2);

    // Final Round
    char *sk1_enc = enc(sk1_enc_round5, key1);
    char *sk2_enc = enc(sk2_enc_round5, key2);

    printf("SK1 Encrypted: %s\n", sk1_enc);
    printf("SK2 Encrypted: %s\n", sk2_enc);

    if (strcmp(sk1_enc, sk2_enc) == 0) {
        printf("The SKs are the same!\n");
        open();
    } else {
        printf("The SKs are different!\n");
    }
    return 0;
}
