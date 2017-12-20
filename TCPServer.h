#include<stdio.h>          /* for printf() and fprintf() */
#include<sys/socket.h>     /* for socket(), bind() and connect() */
#include<arpa/inet.h>      /* for sockaddr_in and inet_ntoa() */
#include<stdlib.h>         /* for atoi() */
#include<string.h>         /* for memset() */
#include<unistd.h>         /* for close() */
#include "DieWithError.h"  /* error handling function */

/* Socket interaction functions */
#include "AcceptTCPConnection.h" 
#include "CreateTCPServerSocket.h" 
#include "HandleTCPClient.h"

void DieWithError(char *errorMessage);          /* Error handling */
void HandleTCPClient(int clntSocket);           /* TCP client handling */
int CreateTCPServerSocket(unsigned short port); /* Create TCP server socket */
int AcceptTCPConnection(int servSock);          /* Accept TCP connection request */
