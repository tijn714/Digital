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
    open();
    return 0;
}
