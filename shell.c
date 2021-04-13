/*
Program 1
Written by Nayana Yeshlur
Base Code provided by OSC 10th edition textbook
gcc shell.c
./a.out
*/

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
  int status;
  char history[MAX_LINE];

  while (should_run) {
    printf("osh>");
    fflush(stdout);
    
    char theCommand[MAX_LINE];
    fgets(theCommand, sizeof(theCommand), stdin);
    char copyCommand[MAX_LINE];
    memcpy(copyCommand, theCommand, MAX_LINE);

    bool ampersandFlag = 0;

    char *word = strtok(copyCommand, " \n");

    int historyCompare = strcmp(word, "!!");

      if(historyCompare == 0) {
        memcpy(theCommand, history, MAX_LINE);
        word = strtok(theCommand, " \n");
      } else {
        memcpy(history, theCommand, MAX_LINE);
        word = strtok(theCommand, " \n");
      }


    for (int i = 0; word != NULL; i++) {
        int compareAmpersand = strcmp(word, "&");
        if(compareAmpersand == 0) {
          args[i] = NULL;
          ampersandFlag = 1;
        } else {
          args[i] = word;
        }
        word = strtok(NULL, " \n");
    }
    

    char str1[] = "exit";
    int compare = strcmp(args[0], str1);

    if(compare == 0) {
      should_run = 0;
    } else {

      int pid = fork();
      if (pid == 0) {

        if(args[1] != NULL) {
          int outputRedirect = strcmp(args[1], ">");
          int inputRedirect = strcmp(args[1], "<");

          if(outputRedirect == 0) {
            mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
            int fd = creat(args[2], mode);
            dup2(fd, 1);
            args[1] = NULL;
            args[2] = NULL;
          }

          if(inputRedirect == 0) {
            int fd2 = open(args[2], O_RDONLY);
            dup2(fd2, 0);
            args[1] = NULL;
            args[2] = NULL;
          }
        }

        int pipeCheck = strcmp(args[2], ">");
        if(pipeCheck == 0) {
          enum {READ, WRITE};
          pid_t pid2;
          int pipeFD[2];

          if (pipe(pipeFD) < 0) {
            perror("Error in creating pipe");
            exit(EXIT_FAILURE);
          }

          pid2 = fork();

          if(pid2 == 0) {
            
          }
        }
        
          execvp(args[0], args);

          exit(0);

      } else {
        if(ampersandFlag == 0) {
          wait(&status);
        }
      }
    }
    

   memset(args, 0, sizeof(args));
  }
  return 0;
}