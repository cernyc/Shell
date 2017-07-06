#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

    

int exit_shell(char **args)
{
  return 0;
}

    
char * read_new_line(){
    
    int bufsize = 1024;
    int position = 0;
    char * buffer = malloc(sizeof(char) * bufsize);
    int c;
    
     if (!buffer) {
        printf("error in read new line (!buffer)");
      }
    
    while (1) {
        
        c = getchar();
    
        if (c == EOF || c == '\n') {
          buffer[position] = '\0';
          return buffer;
        } else {
          buffer[position] = c;
        }
        position++;
    
        // If we have exceeded the buffer, reallocate.
        if (position >= bufsize) {
          bufsize += 1024;
          buffer = realloc(buffer, bufsize);
          if (!buffer) {
            printf("error in read new line (!buffer2)");
          }
        }
      }
}




#define TOK_END " \t\r\n\a"

char ** split_line(char *line)
{
  int bufsize = 64, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token, **tokens_backup;

  if (!tokens) {
    printf("error in split line (!tokens)");
  }

  token = strtok(line, TOK_END);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += 64;
      tokens_backup = tokens;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
		    free(tokens_backup);
        printf("error in split line (!tokens2)");
      }
    }
    token = strtok(NULL, TOK_END);
  }
  tokens[position] = NULL;
  return tokens;
}




int exe(char **args)
{
  int i;

  if (strcmp(args[0], "exit") == 0){
    return 0;
  }
  
  if (args[0] == NULL) {
    printf ("empty command");
    return 1;
  }
  
  return launch(args);
}



int launch(char **args) {
    
    int rc = fork();

    if (rc == 0) {
        if (execvp(args[0], args) == -1) {
          perror("lsh");
        }
        else{
        execvp(args[0], args);
        }
        // We should never reach here
        perror("execvp");
        return -1;
    }
    
    // Parent waits for child to finish
    else if (rc > 0) {
        printf("Parent will wait for child to terminate...\n");
        
        // The basic wait system call suspends the calling process until one
        // of its children terminates
        int rc = wait(NULL);
        if (rc < 0) {
            perror("wait");
            return -1;
        }
        printf("Parent is done waiting!\n");
    }
    else {
        perror("fork");
    }
    return 1;
}

void run_shell(){
    
    char * rl;
    char ** input;
    int state;
    
    do {
        printf ("shell$ ");
        rl = read_new_line();
        input = split_line(rl);
        state = exe(input);
    } while (state);
    
}


int main(int argc, char **argv)
{
  run_shell();
   return EXIT_SUCCESS;
}

