#include <stdio.h> /* for perror() */
#include <stdlib.h> /* for exit() */

void DieWithError(char *errorMessage){
  /* perror() produce un mensaje hacia la stderr
     que es la salida estándar de error */
  perror(errorMessage);
  exit(1);
}
