/*
Program 1
Written by Nayana Yeshlur
Base Code provided by OSC 10th edition textbook

This program acts as a shell interface and executes user supplied commands.
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
  char history[MAX_LINE] = ""; //used to store the last command executed

  while (should_run) {
    printf("osh>");
    fflush(stdout);
    
    //reads in what was typed on the command line
    char theCommand[MAX_LINE];
    fgets(theCommand, sizeof(theCommand), stdin);

    //copies theCommand into copyCommand
    char copyCommand[MAX_LINE];
    memcpy(copyCommand, theCommand, MAX_LINE);

    bool ampersandFlag = 0;
    bool emptyFlag = 0;

    char *word = strtok(copyCommand, " \n");

    int historyCompare = strcmp(word, "!!");

      //Checks to see if the history command was called and then either copies history into theCommand
      //or theCommand into history (if not !!)
      //if history is empty, displays message indicating so
      if(historyCompare == 0) {
        int historyEmpty = strcmp(history, "");
        if(historyEmpty == 0) {
          printf("History is empty.\n");
          emptyFlag = 1;
        } else {
          memcpy(theCommand, history, MAX_LINE);
          word = strtok(theCommand, " \n");
        }
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
      //if the history command (!!) was inputted but there is no history, the user is prompted again
      //nothing is executed
    }  else if (emptyFlag == 1) {
      should_run = 1;
    } else {

      int pid = fork();
      if (pid == 0) { //child

        //generalized output redirection
        for(int i = 0; i < 5; i++) {
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
        for(int i = 0; i < 5; i++) {
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

        //piping, fork a process from the child (create grandchild)
        for(int i = 0; i < 5; i++) {
          if(args[i] != NULL) {
            int pipeCheck = strcmp(args[i], "|");
            if(pipeCheck == 0) {
              enum{READ, WRITE};
              pid_t pid2;
              int pipeFD[2];

              if (pipe(pipeFD) < 0)
              {
                perror("Error in creating pipe");
                exit(EXIT_FAILURE);
              }

              pid2 = fork();

              if(pid2 == 0) { //grandchild
              
                //keeping everything to the left of the pipe and nulling out everything to the right
                for(int j = i; j < 6; j++) {
                  args[j] = NULL;
                }
                
                //close read side of pipe
                //redirect write side from stdout to pipe
                close(pipeFD[READ]); 
                dup2(pipeFD[WRITE], 1);

                execvp(args[0], args);
                perror("execvp unsuccessful");
                exit(EXIT_FAILURE);

              } else { //child
                int status2;
                wait(&status2);

                char *rightArgs[MAX_LINE/2 + 1];

                //close write side of pipe
                //redirect read side from stdin to pipe
                close(pipeFD[WRITE]);
                dup2(pipeFD[READ], 0);
                
                //copy everything right of the | symbol into rightArgs to be executed
                for(int j = 0; j < i; j++) {
                  rightArgs[j] = args[j + i + 1];
                }
                
                execvp(rightArgs[0], rightArgs);
                perror("execvp unsuccessful");
                exit(EXIT_FAILURE);

              }

            }
          }
        }

          execvp(args[0], args);
          perror("execvp unsuccessful");
          exit(EXIT_FAILURE);


      } else { //parent
        if(ampersandFlag == 0) {
          wait(&status);
        }
      }
    }
    

   memset(args, 0, sizeof(args));
  }
  return 0;
}