/*
gcc shell.c
*/



#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

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

    char *word = strtok(theCommand, " \n");
    args[0] = word;
    word = strtok(NULL, " \n");
    args[1] = word;
    word = strtok(NULL, " \n");
    args[2] = word;

    //printf("%s %s", args[0], args[1]);
    char str1[] = "exit";
    int compare = strcmp(args[0], str1);

    if(compare == 0) {
      should_run = 0;
    } else {

      int pid = fork();
      if (pid == 0) {
        execvp(args[0], args);
        printf("here");
      } else {
        if(args[2] == NULL) {
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
  }
  return 0;
}