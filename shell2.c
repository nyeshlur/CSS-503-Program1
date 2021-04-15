#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <fcntl.h>
#include <stdbool.h>

#define MAX_LINE 80 /* The maximum length command */

int main(void)
{
    char *args[MAX_LINE/2 + 1]; /* command line arguments */
    int should_run = 1; /* flag to determine when to exit program */

    while (should_run) {
        printf("osh>");
        fflush(stdout);

        char theCommand[MAX_LINE];
        fgets(theCommand, sizeof(theCommand), stdin);

        bool ampersandFlag = 0;

        printf("theCommand %s", theCommand);

        char *word = strtok(theCommand, " \n"); //change to copyCommand later
        printf("first arg %s", word);

        for (int i = 0; word != NULL; i++) {
            int compareAmpersand = strcmp(word, "&");
            if(compareAmpersand == 0) {
            args[i] = NULL;
            ampersandFlag = 1;
            } else {
            args[i] = word;
            }
            word = strtok(NULL, " \n");
            printf("args %s", word);
        }


        char str1[] = "exit";
        int compare = strcmp(args[0], str1);
        //if exit, stop the loop and exit the program
        if(compare == 0) {
            should_run = 0;
        } else {

            pid_t pid = fork();

            if(pid == 0) { //in child
            

            //the problem line is below, if I use a number smaller than MAX_LINE / 2 + 1, it works but I can't use that number
            //so I'm not sure why I can't check the entirety of args
            //I've run into the same issue in almost all of the loops I have implemented
            //I've pared down shell so that I can just illustrate the issue
            for(int i = 0; i < 10; i++) {
                if(args[i] != NULL) {
                    int outputRedirect = strcmp(args[i], ">");
                    if(outputRedirect == 0) {
                        mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
                        int fd = creat(args[(i + 1)], mode);
                        dup2(fd, 1);
                        args[i] = NULL;
                        args[(i + 1)] = NULL;
                    }
                }
            }
                printf("got here ");
                fflush(stdout);
                execvp(args[0], args);
            } else {
                if(ampersandFlag == 0) {
                    int status;
                    wait(&status);
                }
            }
            
        }
        
        memset(args, 0, sizeof(args));
    }
  return 0;
}