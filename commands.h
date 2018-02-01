#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define MAXCOM 10
#define MAXLINE 128

/* parses the input: https://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm */
int parseInput(char* line, char**cmds) {
    int i=0;
    char delimit[]=" \n";

    cmds[i] = strtok(line, delimit);

    while(cmds[i]!=NULL)
        cmds[++i] = strtok(NULL, delimit);

    cmds[i] = 0;

    return i;
}

/* prints the current directory */

int printDirectory(int value) {
    char cwd[MAXLINE];

    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        printf("Error: %s\n", strerror(errno));
        return -1;
    }
    
    if (value == 0) {
        printf("[%s] ", cwd);
    }
    else {
        printf("%s", cwd);
    }
    return 0;
}

/* changes the active directory */
int changeDirectory(char* value) {
    if (chdir(value) != 0) {
        printf("Error: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

/* exits the shell with or without arguments */

void exitShell(char* value, int noArg) {
    if (noArg == 1) {
        exit(0);
    }
    else {
        char *ptr;
        long ret = -123456; //this is just a very weird value so the ret trick works  ¯\_(ツ)_/¯
        ret = strtol(value, &ptr, 10);
        if (ret) {
            exit(ret);
        }
        else {
            printf("Cannot parse the value of exit; provide a numeric argument for it to work\n");
        }
    }
}

/* prints the directories in the directory stack */

void printDirs(int top_value, char s_dirs[MAXCOM][MAXLINE]) {

    if (top_value == -1)
        printDirectory(1);
    while (top_value > -1) {
        printf("%s ", s_dirs[top_value]);
        top_value--;
    }
    printf("\n");
}

/* pushes a directory from the input onto the stack and sets it as the active directory */

int pushd(int top_value, char* directory, char s_dirs[MAXCOM][MAXLINE], int noArg) {

    if (noArg == 1) {
        printf("pushd: no arguments provided\n");
        return -1;
    }
    if (top_value == MAXCOM - 1) {
        printf("pushd: too many directories on the stack\n");
    }
    else {
        top_value++;
        if (changeDirectory(directory) == 0) {
            strcpy(s_dirs[top_value], directory);
            while (top_value > -1) {
                top_value--;
            }
            return 0;
        }
    }
    return -1;
}

/* pops the upmost directory from the stack and sets it as the active directory */

int popd(int top_value, char s_dirs[MAXCOM][MAXLINE]) {
    if (top_value == -1) {
        printf("popd: no directories on the stack. Perhaps you mean to pushd [ARG]?\n");
        return -1;
    }
    else {
        top_value--;
        if (top_value == -1) {
            printf("popd: no directories on the stack. Perhaps you mean to pushd [ARG]?\n");
        }
        else {
            changeDirectory(s_dirs[top_value]);
            while (top_value > -1)
                top_value--;
        }
        return 0;
    }
}

/* help */

void printHelp() {

    printf("------------------------------------------------------\n");
    printf("This is a simple shell program written by Magdalena Mozgawa (s389479) for homework. No warranty is given.\n");
    printf("It provides the following functionalities:\n");
    printf("  cd [ARG]               changes the active directory\n");
    printf("  exit [STATUS NO]       exits the shell; the argument is optional\n");
    printf("  help                   displays this message\n");
    printf("  pushd [ARG]            accepts a directory as an argument and pushes it onto the directory stack\n");
    printf("  popd                   pops the upmost directory from the directory stack\n");
    printf("  dirs                   displays the directory stack; up to 10 directories can be kept on the stack.\n");
    printf("You can also run any scripts from your PATH or the hard drive:\n");
    printf("  to run a command from /usr/bin simply provide the command with its flags and arguments,\n");
    printf("  to run a script from your drive provide its absolute path\n");
    printf("      Example: /home/user/path/to/script.sh -f argument\n");
    printf("  or its relative path\n");
    printf("      Example:  ../subdirectory/script.sh -f argument\n");
    printf("------------------------------------------------------\n");
}

