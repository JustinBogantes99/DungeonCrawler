#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LENGTH 40

void flush_buffer (char *buffer);
void * keyboard_listener (void *args);

void* keyboard_listener (void *args){
  char buffer[MAX_LENGTH + 1];
  memset (buffer, 0, MAX_LENGTH + 1);

  /* Read a line of input from STDIN */
  while (fgets (buffer, MAX_LENGTH, stdin) != NULL)
    {
      /* Try to convert input to integer 7. All other values
         are wrong. */
      long guess = strtol (buffer, NULL, 10);
      if (guess != 7)
        {
          printf ("Wrong. Try again. ");
          /* Clear the buffer for another try. */
          flush_buffer (buffer);
          memset (buffer, 0, MAX_LENGTH);
        }
      else
        {
          /* Successfully read a 7 from input; exit with true */
          printf ("Congratulations!\n");
          pthread_exit ((void*)true);
        }
    }

  /* In case input fails for some reason, exit thread with false */
  pthread_exit ((void*)false);
}

void flush_buffer (char *buffer){
  int chr;
  
  /* Check for '\n' */
  char *newline = strchr (buffer, '\n');

  /* If none found, retrieve and discard rest of the characters */
  if (newline == NULL)
    while (((chr = getchar()) != '\n') && !feof (stdin) &&
           !ferror (stdin))
      ;
}
