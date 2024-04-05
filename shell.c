#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lmcons.h>
#include <unistd.h>

void Welcome();
void getLocation();

int main() {
    Welcome();    // Call the Welcome function
    getLocation();  // Call the getLocation function

    // Your code here
    return 0;   // Return 0 to indicate successful execution
}

void Welcome() {
    printf("aSPY//YASa\n");
    printf("apyyyyCY//////////YCa |\n");
    printf("sY//////YSpcs scpCY//Pp | Welcome to myShell\n");
    printf("ayp ayyyyyyySCP//Pp syY//C | Version 2.4.3\n");
    printf("AYAsAYYYYYYYY///Ps cY//S |\n");
    printf("pCCCCY//p cSSps y//Y | https://github.com/<user>\n");
    printf("SPPPP///a pP///AC//Y |\n");
    printf("A//A cyP////C | Have fun!\n");
    printf("p///Ac sC///a |\n");
    printf("P////YCpc A//A | Craft packets like it is your last\n");
    printf("scccccp///pSP///p p//Y | day on earth.\n");
    printf("sY/////////y caa S//P | -- Lao-Tze\n");
    printf("cayCyayP//Ya pY/Ya |\n");
    printf("sY/PsY////YCc aC//Yp\n");
    printf("sc sccaCY//PCypaapyCP//YSs\n");
    printf("spCPY//////YPSps\n");
    printf("ccaacs\n");
}

void getLocation() {
    char username[UNLEN + 1];
    DWORD usernameLen = UNLEN + 1;

    if (GetUserName(username, &usernameLen)) {
        printf("Username: %s\n", username);
    } else {
        printf("Failed to retrieve username.\n");
    }
}
char** splitArgument(char* str) {
    char** arguments = (char**)malloc(sizeof(char*) * 100);
    int i = 0;
    char* token = strtok(str, " ");
    while (token != NULL) {
        arguments[i] = (char*)malloc(sizeof(char) * (strlen(token) + 1));
        strcpy(arguments[i], token);
        token = strtok(NULL, " ");
        i++;
    }
    arguments[i] = NULL;
    return arguments;
}
void logout(char* str) {
    char* trimmed = str;
    while (*trimmed == ' ') {
        trimmed++;
    }
    if (strcmp(trimmed, "exit") == 0) {
        printf("\033[1;32mExiting the shell...\033[0m\n");
        exit(0);
    } else if (strncmp(trimmed, "exit ", 5) == 0) {
        printf("\033[1;32mExiting the shell with code %d...\033[0m\n", atoi(trimmed + 5));
        exit(atoi(trimmed + 5));
    } else {
        printf("\033[1;31mInvalid command: %s\033[0m\n", str);
    }
}
