#include "TCPServer.h"       /* TCP server includes */
#include <pthread.h>         /* for POSIX threads */


#define SERV_PORT 9999            /* Server will run on this port */
void *ThreadMain(void *arg); /* Main program of a thread */

/* Structure of arguments to pass to client thread */
struct ThreadArgs{
  int clntSock;
};

int main(int argc, char *argv[]){
  int servSock;                   /* Socket descriptor for server */
  int clntSock;                   /* Socket descriptor for client */
  unsigned short servPort;        /* Server port */
  pthread_t threadID;             /* Thread ID from pthread_create() */
  struct ThreadArgs *threadArgs;  /* Pointer to argument structure for thread */

  /* Create server socket */
  servPort = SERV_PORT;
  servSock = CreateTCPServerSocket(servPort);
  
  for(;;){ /* Run forever */
    clntSock = AcceptTCPConnection(servSock);

    /* Create separate memory for client argument */
    if((threadArgs = (struct ThreadArgs *) malloc(sizeof(struct ThreadArgs))) == NULL)
      DieWithError("malloc() failed");
    threadArgs->clntSock = clntSock;

    /* Create client thread */
    if(pthread_create(&threadID, NULL, ThreadMain, (void *) threadArgs) != 0)
      DieWithError("pthread_create() failed");
    printf("with thread %i \n",(long int) threadID);
  }
  /* NOT REACHED */
}


void *ThreadMain(void *threadArgs){
  int clntSock; /* Socket descriptot for client connection */

  /* Guarantees that thread resources are deallocated upon return */
  pthread_detach(pthread_self());

  /* Extract socket file descriptor from argument */
  clntSock = ((struct ThreadArgs *) threadArgs) -> clntSock;
  free(threadArgs);  /* Deallocate memory for argument */

  HandleTCPClient(clntSock);
  
  return (NULL);
}
