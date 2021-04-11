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

  while (should_run) {
    printf("osh>");
    fflush(stdout);
    
    char theCommand[MAX_LINE];
    fgets(theCommand, sizeof(theCommand), stdin);

    bool ampersandFlag = 0;

    char *word = strtok(theCommand, " \n");

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
        
      //int inputRedirect = strcmp(args[1], "<");
      int outputRedirect = strcmp(args[1], ">");
      if(outputRedirect == 0) {
        mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
        int fd = creat("out.txt", mode);
        dup2(fd, STDOUT_FILENO);
        args[1] = NULL;
        args[2] = NULL;
      }
        execvp(args[0], args);
      } else {
        if(ampersandFlag == 0) {
          wait(&status);
        }
      }
    }
    
    /**
    * After reading user input, the steps are:
    * (1) fork a child process using fork()
    * (2) the child process will invoke execvp()
    * (3) parent will invoke wait() unless command included &
    */

   //memset(args, )
  }
  return 0;
}