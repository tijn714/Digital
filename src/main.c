#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

void delay(int milliseconds) {
  clock_t time_end;
  time_end = clock() + milliseconds * CLOCKS_PER_SEC/1000;
  while (clock() < time_end) {
  }
}

#ifdef _WIN32
    #define CLEAR_COMMAND "cls"
#else 
    #define CLEAR_COMMAND "clear"
#endif

#ifdef _WIN32
    #include <windows.h>
    #define OS_NAME "Windows"
    #define OPEN_COMMAND "start %s"
    #define CLOSE_COMMAND "taskkill /IM firefox.exe"
#elif __APPLE__
    #include <ApplicationServices/ApplicationServices.h>
    #define OS_NAME "macOS"
    #define OPEN_COMMAND "open %s"
    #define CLOSE_COMMAND "killall firefox"
#else
    #include <unistd.h>
    #define OS_NAME "Linux"
    #define OPEN_COMMAND "xdg-open %s"
    #define CLOSE_COMMAND "pkill firefox"
#endif


#define PASSWORD_TO_EXIT 'Password123'

void open_browser() {
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
        keybd_event(VK_F11, 0, 0, 0);
        keybd_event(VK_F11, 0, KEYEVENTF_KEYUP, 0);
    #elif __APPLE__
        CGEventRef event = CGEventCreateKeyboardEvent(NULL, (CGKeyCode)144, true);
        CGEventSetFlags(event, kCGEventFlagMaskCommand | kCGEventFlagMaskControl);
        CGEventPost(kCGSessionEventTap, event);
        CFRelease(event);
    #elif LINUX
        system("xdotool key F11");
    #endif
}

bool is_browser_open() {
    // Determine the command to check if the browser is open based on the operating system
    char command[1024];
    #ifdef _WIN32
        sprintf(command, "tasklist /FI \"IMAGENAME eq firefox.exe\" /FI \"WINDOWTITLE eq %s\"", "index");
    #elif __APPLE__
        sprintf(command, "ps -ax | grep firefox | grep -v grep | awk '{print $1}'");
    #else
        sprintf(command, "ps -ef | grep firefox | grep -v grep | awk '{print $2}'");
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

    // Return true if the browser is open, false otherwise
    return strlen(output) != 0;
}

void wait_for_browser_close() {
    while (true) {
        if (!is_browser_open()) {
            // If the browser is not open, prompt the user to confirm that they want to exit the program
            printf("Browser closed. Please enter the super secret password to exit: ");
            char c = getchar();
            if (c == PASSWORD_TO_EXIT) {
                break;
            } else {
                open_browser();
            }
        }
    }
}

int main(int argc, char *argv[]) {
    system(CLEAR_COMMAND);
    open_browser();
    delay(10000);
    system(CLOSE_COMMAND);
    wait_for_browser_close();
    return 0;
}