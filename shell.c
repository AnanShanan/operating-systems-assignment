#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lmcons.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

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

void cd(char** args) {
    if (args[1] == NULL) {
        printf("\033[1;31mUsage: cd <directory>\033[0m\n");
        return;
    }

    if (args[1][0] == '\"' && args[1][strlen(args[1]) - 1] == '\"') {
        args[1][strlen(args[1]) - 1] = '\0';
        args[1]++;
    }

    if (chdir(args[1]) != 0) {
        printf("\033[1;31mError: Directory '%s' does not exist.\033[0m\n", args[1]);
    }
}
void cp(char** args) {
    if (args[1] == NULL || args[2] == NULL) {
        printf("\033[1;31mUsage: cp <source> <destination>\033[0m\n");
        return;
    }

    HANDLE srcFile = CreateFile(args[1], GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (srcFile == INVALID_HANDLE_VALUE) {
        printf("\033[1;31mError: Source file '%s' does not exist.\033[0m\n", args[1]);
        return;
    }

    HANDLE dstFile = CreateFile(args[2], GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (dstFile == INVALID_HANDLE_VALUE) {
        printf("\033[1;31mError: Could not create destination file '%s'.\033[0m\n", args[2]);
        CloseHandle(srcFile);
        return;
    }

    char buffer[1024];
    DWORD bytesRead, bytesWritten;
    while (ReadFile(srcFile, buffer, sizeof(buffer), &bytesRead, NULL) && bytesRead > 0) {
        if (!WriteFile(dstFile, buffer, bytesRead, &bytesWritten, NULL) || bytesWritten != bytesRead) {
            printf("\033[1;31mError: Could not write to destination file '%s'.\033[0m\n", args[2]);
            break;
        }
    }

    CloseHandle(srcFile);
    CloseHandle(dstFile);
    printf("\033[1;32mFile '%s' copied to '%s'.\033[0m\n", args[1], args[2]);
}

void delete(char** args) {
    if (args[1] == NULL) {
        printf("\033[1;31mUsage: delete <file>\033[0m\n");
        return;
    }

    if (args[1][0] == '\"' && args[1][strlen(args[1]) - 1] == '\"') {
        args[1][strlen(args[1]) - 1] = '\0';
        args[1]++;
    }

    if (remove(args[1]) != 0) {
        printf("\033[1;31mError: Could not delete file '%s'.\033[0m\n", args[1]);
    } else {
        printf("\033[1;32mFile '%s' deleted successfully.\033[0m\n", args[1]);
    }
}

void mypipe(char** args1, char** args2) {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        printf("\033[1;31mError: Could not create pipe.\033[0m\n");
        return;
    }

    pid_t pid = fork();
    if (pid == -1) {
        printf("\033[1;31mError: Could not fork process.\033[0m\n");
        close(pipefd[0]);
        close(pipefd[1]);
        return;
    }

    if (pid == 0) {
        // Child process
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);
        execvp(args1[0], args1);
        printf("\033[1;31mError: Could not execute command '%s'.\033[0m\n", args1[0]);
        exit(1);
    } else {
        // Parent process
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);
        execvp(args2[0], args2);
        printf("\033[1;31mError: Could not execute command '%s'.\033[0m\n", args2[0]);
        exit(1);
    }
}

void move(char** args) {
    if (args[1] == NULL || args[2] == NULL) {
        printf("\033[1;31mUsage: move <source> <destination>\033[0m\n");
        return;
    }

    if (args[1][0] == '\"' && args[1][strlen(args[1]) - 1] == '\"') {
        args[1][strlen(args[1]) - 1] = '\0';
        args[1]++;
    }

    if (args[2][0] == '\"' && args[2][strlen(args[2]) - 1] == '\"') {
        args[2][strlen(args[2]) - 1] = '\0';
        args[2]++;
    }

    if (rename(args[1], args[2]) != 0) {
        printf("\033[1;31mError: Could not move file '%s' to '%s'.\033[0m\n", args[1], args[2]);
    } else {
        printf("\033[1;32mFile '%s' moved to '%s'.\033[0m\n", args[1], args[2]);
    }
}

void echoappend(char** args) {
    if (args[1] == NULL || args[2] == NULL) {
        printf("\033[1;31mUsage: echoappend <string> >> <file>\033[0m\n");
        return;
    }

    if (args[2][0] == '\"' && args[2][strlen(args[2]) - 1] == '\"') {
        args[2][strlen(args[2]) - 1] = '\0';
        args[2]++;
    }

    int fd = open(args[2], O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1) {
        printf("\033[1;31mError: Could not open file '%s'.\033[0m\n", args[2]);
        return;
    }

    dprintf(fd, "%s\n", args[1]);
    close(fd);
    printf("\033[1;32mString '%s' appended to file '%s'.\033[0m\n", args[1], args[2]);
}
