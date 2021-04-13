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

      //Checks to see if the history command was called and then either copies history into theCommand
      //or theCommand into history (if not !!)
      if(historyCompare == 0) {
        memcpy(theCommand, history, MAX_LINE);
        word = strtok(theCommand, " \n");
      } else {
        memcpy(history, theCommand, MAX_LINE);
        word = strtok(theCommand, " \n");
      }

    //Parses out everything from theCommand and puts it into args[]
    //except & which just switches a flag
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
    //if exit, stop the loop and exit the program
    if(compare == 0) {
      should_run = 0;
    } else {

      int pid = fork();
      if (pid == 0) {

        //generalized output redirection
        for(int i = 0; i < 4; i++) {
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
        

        //generalized input redirection
        for(int i = 0; i < 4; i++) {
          if(args[i] != NULL) {
            int inputRedirect = strcmp(args[i], "<");
            if(inputRedirect == 0) {
              int fd2 = open(args[(i + 1)], O_RDONLY);
              dup2(fd2, 0);
              args[i] = NULL;
              args[(i + 1)] = NULL;
            }
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