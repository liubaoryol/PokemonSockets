#include <sys/socket.h>   /* for socket(), bind() and connect() */
#include <arpa/inet.h>    /* for sockadddr_in and inet_ntoa */
#include <string.h>       /* for memset() */

#define MAXPENDING 5      /* Maximum outstanding connection requests */

void DieWithError(char *errorMessage);   /* Error handling function */

int CreateTCPServerSocket(unsigned short port){
  int sock;                        /* Socket to create */
  struct sockaddr_in servAddr    ; /* Local address */

  /* Create socket for incoming connections */
  if((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    DieWithError("socket() failed");
  
  
  /* Construct local address structure */
  memset(&servAddr, 0 ,sizeof(servAddr));        /* Zero out structure */
  servAddr.sin_family      = AF_INET;            /* Internet address family */
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY);  /* Any incoming interface */
  servAddr.sin_port        = htons(port);        /* Given port */

  /* Bind to the local address */
  if(bind(sock, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
    DieWithError("bind() failed");

  /* Mark the socket so it will listen for incoming connections */
  if(listen(sock, MAXPENDING) < 0)
    DieWithError("listen() failed");

  return sock;
  
}
