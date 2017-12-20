#include <stdio.h>             /* for printf() and fprintf() */
#include <sys/socket.h>        /* for recv() and send() */
#include <unistd.h>            /* for close() */
#include "MessageDefinition.h" /* structures for messages */

#define RCVBUFSIZE 32     

void DieWithError(char *errorMessage);            /* Error handling function */

void HandleTCPClient(int clntSocket){

  char buffer[RCVBUFSIZE];     /* buffer for echo string */
  int recvMsgSize;             /* Size of received message */
  unsigned short code;         /* Code for the message */
  char serverResponse[22];     /* for store confirmation message */

  strcpy(serverResponse, "Data has been received");

  if((recvMsgSize = recv(clntSocket, buffer, RCVBUFSIZE, 0)) < 0)
    DieWithError("recv() failed");

  while(recvMsgSize > 0){
    
    /* Confirmation message back to client */
    if(send(clntSocket, serverResponse, sizeof(serverResponse), 0) != sizeof(serverResponse))
      DieWithError("recv() failed");    
    
    /* See if there is more data to receive */
    if((recvMsgSize = recv(clntSocket, buffer, RCVBUFSIZE, 0)) < 0)
      DieWithError("recv() failed");
    
  }

  printf("Data Received\n");
  close(clntSocket);    /* Close client socket */
  
};
