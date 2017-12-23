#include <stdio.h>          /* for printf() and fprintf() */
#include <sys/socket.h>     /* for socket(), bind() and connect() */
#include <arpa/inet.h>      /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h>         /* for atoi() */
#include <string.h>         /* for memset() */
#include <unistd.h>         /* for close() */
#include <pthread.h>         /* for POSIX threads */
#include "DieWithError.h"  /* error handling function */
#include "MessageDefinition.h"

#define SERV_PORT 9999            /* Server will run on this port */
#define RCVBUFSIZE 3100

void DieWithError(char *errorMessage); /*Error handling function*/


int main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in pokeServAddr;
	unsigned short servPort;
	char *servIP;
	char *pokeString;
	char pokeBuffer[RCVBUFSIZE];
	unsigned int pokeStringLen;
	int bytesRcvd, bytesSend;

	unsigned int code = 10;

	if((argc != 3)){ /*temp*/
		fprintf(stderr, "Usage: %s <ServerIP> <Port>\n",argv[0]);
		exit(1);
	}

	servIP = argv[1];
	servPort = atoi(argv[2]); /*cast String to int*/

	/*CREATING MSG STRUCTURE*/
	msg_type1 *msg_t1 = (struct msg_type1 *)(unsigned char *)malloc(sizeof (unsigned char));
	
	memcpy(msg_t1->code, &code, sizeof(msg_t1->code));
	
	/*CREATING SOCKET*/
	if((sock=socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		DieWithError("socket () failed");

	memset(&pokeServAddr,0,sizeof(pokeServAddr));

	pokeServAddr.sin_family = AF_INET;
	pokeServAddr.sin_addr.s_addr = inet_addr(servIP);

	pokeServAddr.sin_port = htons(servPort);

	/*CONNECTING SOCKET*/
	if((connect(sock,(struct sockaddr *)&pokeServAddr, sizeof(pokeServAddr))) < 0)
		DieWithError("connect () failed");

	pokeStringLen = sizeof(msg_type1);

	/*SENDING MSG*/
	if((bytesSend = send(sock, msg_t1, sizeof(msg_type1), 0)) != pokeStringLen)
	  DieWithError("send() sent a different number of bytes than expected");
	else
	  printf("Bytes send to server: %i \n",bytesSend);
	
	/*RECEIVING MSG (STARTING POKEMON CAPTURE PROTOCOL 021: "Catch 'em all")*/
	if((bytesRcvd=recv(sock,pokeBuffer,RCVBUFSIZE-1,0)) < 0)
	  DieWithError("recv () failed or connection died prematurely");

	    /* receive message */
    
	msg_type2 *msg = (struct msg_type2 *)pokeBuffer;
	unsigned char codeR = *((unsigned char *)msg->code);
	unsigned char pokeIdR = *((unsigned char *)msg->idPokemon);
		
	printf("Bytes received %i\nServer response:\ncode: %i\n id: %i\n",bytesRcvd,codeR,pokeIdR);


}
