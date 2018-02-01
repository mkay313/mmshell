#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include "commands.h"

#define MAXLINE 128
#define MAXCOM 10

int main() {
/* initializes the stack for dirs, pushd & popd */
    int top = -1;
    char line[MAXLINE], *cmds[MAXLINE], s_dirs[MAXCOM][MAXLINE];

    for(;;) {
        printDirectory(0);
        if (fgets(line, MAXLINE, stdin) == NULL) {
            printf("Error: %s (wat?)\n", strerror(errno));
            exit(1);
        }
        int res = parseInput(line, cmds);
        if (res == 0) {
            continue;
        }

        /* if the line matches a functionality then go with it */
        else if (strcmp(cmds[0], "exit") == 0) {
            exitShell(cmds[1], res);
        }
        else if (strcmp(cmds[0], "cd") == 0) {
            changeDirectory(cmds[1]);
        }
        else if (strcmp(cmds[0], "help") == 0) {
            printHelp();
        }
        else if(strcmp(cmds[0], "dirs") == 0) {
            printDirs(top, s_dirs);
        }
        else if(strcmp(cmds[0], "pushd") == 0) {
            if (pushd(top, cmds[1], s_dirs, res) == 0) {
                top++;
                printDirs(top, s_dirs);
            }
        }
        else if(strcmp(cmds[0], "popd") == 0) {
            if (popd(top, s_dirs) == 0) {
                top--;
                printDirs(top, s_dirs);
            }
        }
        else {

            /* else just go with cmds[0] */
            int pid;
            if((pid = fork()) >= 0) {
                if(pid == 0) {

                    /* if cmds[0] doesn't match anything then print the error message and move along */
                    if(execvp(cmds[0], cmds) < 0) {
                        printf("   %s is  n o t  the command you're looking for.\nHere's the error: %s\n", cmds[0], strerror(errno));
                        exit(1);
                    }

                /* if everything went well then wait for the child to finish its work */
                } else {
                    int child_status;
                    wait(&child_status);
                }
            }
            else {
                printf("Forking failed...\n");
            }
        }
    }

}
