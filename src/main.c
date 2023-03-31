#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

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

    // When we are in the browser, we must set it to full screen so that the user cannot see the desktop
    // We do this by sending a key combination to the browser
    // The key combination is different for each operating system
    #ifdef _WIN32
        system("powershell -Command \"[System.Windows.Forms.SendKeys]::SendWait('^{F11}')\"");
    #elif __APPLE__
        system("osascript -e 'tell application \"System Events\" to key code 144 using {control down, command down}'");
    #elif LINUX
        system("xdotool key F11");
    #endif

    // If the user closes the browser, prompt them to confirm that they want to exit the program, if not, open the browser again
    while (true) {
        // Determine the command to check if the browser is open based on the operating system
        #ifdef _WIN32
            sprintf(command, TASKLIST_COMMAND, "index");
        #elif __APPLE__
            sprintf(command, PS_COMMAND);
        #else
            sprintf(command, PS_COMMAND);
        #endif

        // Execute the command
        FILE *fp = popen(command, "r");
        if (fp == NULL) {
            printf("Failed to run command\n" );
            exit(1);
        }

        // Read the output of the command
        char output[1024];
        fgets(output, sizeof(output) - 1, fp);

        // Close the command
        pclose(fp);

        // If the browser is not open, prompt the user to confirm that they want to exit the program
        if (strlen(output) == 0) {
            printf("Browser closed. Exit program? (y/n): ");
            char c = getchar();
            if (c == 'y') {
                break;
            } else {
                open();
            }
        }
    }
}

int main(int argc, char *argv[]) {
    open();
    return 0;
}
