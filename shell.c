/*
gcc shell.c
*/



#include <stdio.h>
#include <unistd.h>
#include <string.h>

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

    char *word = strtok(theCommand, " ");
    args[0] = word;
    word = strtok(NULL, " ");
    args[1] = word;
    word = strtok(NULL, " ");
    args[2] = word;

    //printf("%s %s", args[0], args[1]);
    char str1[] = "exit";
    int compare = strcmp(args[0], exit);

    if(compare == 0) {
      printf("exit");
    }

/*
    if(args[2] == "&") {
      printf("&");
    } else {
      printf("not &");
    }
*/
    //printf("%c\n", theCommand[0]);
    should_run = 0;

    
    //int pid = fork();
    //if (pid == 0) {
        //execvp(char *command, char *params[])
    //} else {
        //if (command does not include &) {
          //wait
        //}
    //}
    
    /**
    * After reading user input, the steps are:
    * (1) fork a child process using fork()
    * (2) the child process will invoke execvp()
    * (3) parent will invoke wait() unless command included &
    */
  }
  return 0;
}