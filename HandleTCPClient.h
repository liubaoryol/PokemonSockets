#include <stdio.h>             /* for printf() and fprintf() */
#include <sys/socket.h>        /* for recv() and send() */
#include <unistd.h>            /* for close() */
#include <stdlib.h>            /* for rand() and srand() */
#include <time.h>              /* for time() */
#include "MessageDefinition.h" /* structures for messages */

#define RCVBUFSIZE 1           /* Always receive just one byte */
void DieWithError(char *errorMessage);            /* Error handling function */
void interaction(unsigned char *code, int *state, int clntSocket, int* numAttemps,int randomPoke);  /* Code/state handling function */

void HandleTCPClient(int clntSocket){
  
  unsigned char code = 0;         /* Code for the message */
  int state = 0;                  /* Current state of the server */
  int numAttemps = 3;              /* for store the number of attemps left */
  srand((unsigned)time(NULL));    /* obtain random seed */
  int randomPoke = rand() % 9;    /* generate random number from 0 to 8, the poke client is trying to capture */

  
  /* Now starts the interaction between states, send and receive data until end of connection(state number 7)*/
  while(state != 7){             
    
    interaction(&code, &state, clntSocket, &numAttemps, randomPoke);
    
  }
  
  close(clntSocket);    /* Close client socket */
  
  printf("Termina la conexión, el socket cerró\n");
  
};


/* This function handle all the send and receive interaction
   based on the received code and the state of the thread,
   it can be understood as the transition funcion of the
   state machine,also sends a response */

void interaction(unsigned char* code, int* state, int clntSocket, int* numAttemps, int randomPoke){
  
  char serverResponse[22];     /* for store confirmation message */
  char buffer[RCVBUFSIZE];     /* buffer for received message */
  int recvMsgSize;             /* Size of received message */
  int codeToSend;              /* for store the codes of messages to send */
  
  /* for store messages structures to send */
  
  msg_type1* msgtype1;
  msg_type2* msgtype2;
  msg_type3* msgtype3;
  msg_type4* msgtype4;
  
  msg_type2* getRandomPoke(int randomPoke);                 /* returns the structure for send message type 2 */
  msg_type3* getType3Message(int randomPoke,int *attemps);  /* returns the structure for send message type 3 */
  msg_type4* getType4Message(int randomPoke);               /* returns the structure for send message type 4 */      
  void sendErrorCode(int clntSocket, char* message);        /* sends a error code 40 message to the client */

  
  /* Changing from start state 0 to state 1  */
  
  if(*code == 0 && *state == 0){
    
    /* receive message */
    
    if((recvMsgSize = recv(clntSocket, buffer, RCVBUFSIZE, 0)) < 0)
      DieWithError("recv() failed");        
    
    msg_type1 *msg = (struct msg_type1 *)buffer;
    *code = *((unsigned char *)msg->code);
    printf("Un cliente desea capturar un Poke\n");
    
    /* Expected code at this point: 10 */
    if(*code != 10){
      sendErrorCode(clntSocket, "Error: Unexpected code, expected code 10");
      *state = 7;
      return;
    }
    
    
    *state = 1;
    
    /* Send random Poke */
    msgtype2 = getRandomPoke(randomPoke);
    
    if(send(clntSocket, msgtype2, sizeof(struct msg_type2), 0) != sizeof(struct msg_type2))
      DieWithError("send() failed");

    printf("Veamos si quiere capturar a %i\n",randomPoke);
    
  }else if(*code == 10 && *state == 1 || /* All this cases have the same behavior */
	   *code == 30 && *state == 1 ||
	   *code == 31 && *state == 1 ){
    
    /* receive message */
    
    if((recvMsgSize = recv(clntSocket, buffer, RCVBUFSIZE, 0)) < 0)
      DieWithError("recv() failed");        
    
    msg_type1 *msg = (struct msg_type1 *)buffer;
    *code = *((unsigned char *)msg->code);
    
    /* Expected code at this point: 30 or 31 */
    if(*code != 30 && *code != 31){
      sendErrorCode(clntSocket, "Error: Unexpected code, expected code 30 or 31");
      *state = 7;
      return;
    }
    
    
    /* If received code is 31 just send a 32 and terminate session */
    
    if(*code == 31){

      /* Construct the message */
      codeToSend = 32;
      msgtype1 = (struct msg_type1 *)(unsigned char *)malloc(sizeof (unsigned char));      
      memcpy(msgtype1->code, &codeToSend, sizeof(msgtype1->code));
      
      /* Send it */
      
      if(send(clntSocket, msgtype1, sizeof(struct msg_type1), 0) != sizeof(struct msg_type1))
      DieWithError("send() failed");

      printf("No quiso, digamos adiós :(\n");
      
      /* Change state to final state */
      *state = 7;
      
    }else{ /* Code here must be 30, the client want to catch the Poke */
      
      /* See if our lucky guy actually got the Poke */
      int r = random() % 100;
      if(r >= 55 ){
	
	/* He got it! Get the correct message */      
	msgtype4 = getType4Message(randomPoke);  
	
	/* Send the answer */
	if(send(clntSocket, msgtype4, sizeof(struct msg_type4), 0) != sizeof(struct msg_type4))
	DieWithError("send() failed");

	printf("Lo logro! Enviando imagen...\n");
	
	/* Change state to "wait for terminate session" state */
	*state = 6;
	
      }else{
	
	/* S&#t! Try again, See if the last attemp failed */
	if(*numAttemps == 1){
	  /* This guy is a bad lucky fella, just let him know he got nottin' */	  
	  
	  /* Construct the message */
	  codeToSend = 23;
	  msgtype1 = (struct msg_type1 *)(unsigned char *)malloc(sizeof (unsigned char));      
	  memcpy(msgtype1->code, &codeToSend, sizeof(msgtype1->code));
	  
	  /* Send it */

	  printf("No lo logró, mala suerte, enviando respuesta...\n");
	  
	  if(send(clntSocket, msgtype1, sizeof(struct msg_type1), 0) != sizeof(struct msg_type1))
	    DieWithError("send() failed");
	  
	  /* Change state to "wait for terminate session" state */
	  *state = 6;      		
	  
	}else{
	  /* Still have attemps, Decrease by one and send correct Message */
	  printf("Fallo! le quedan %i intentos\n",*numAttemps - 1);
	  *numAttemps = *numAttemps - 1;
	  msgtype3 = getType3Message(randomPoke, numAttemps);
	  /* Send the answer */      
	  if(send(clntSocket, msgtype3, sizeof(struct msg_type3), 0) != sizeof(struct msg_type3))
	    DieWithError("send() failed");
	  
	  /* State remains the same */
	}
      }                  
    }
    
  }else if(*state == 6){ /* just wait for a 32 message from client */

    /* receive message */
    
    if((recvMsgSize = recv(clntSocket, buffer, RCVBUFSIZE, 0)) < 0)
      DieWithError("recv() failed");
    
    msg_type1 *msg = (struct msg_type1 *)buffer;
    *code = *((unsigned char *)msg->code);
    
    /* Expected code at this point: 32 */
    if(*code != 32){
      sendErrorCode(clntSocket, "Error: Unexpected code, expected code 32");
      printf("Error:No se recibió un mensaje de termino de sesión, vaya modales,nos vamos de todas formas\n");
      *state = 7;
      return;
    }else
      *state = 7;    
    
  }else if(*code == 32){ /* if received code is 32 terminate session, at any state */
    
    *state = 7;
      
  }
  
}


/* Construct and return a message with a random Pokemon code */

msg_type2* getRandomPoke(int randomPoke){
  
  int code = 20;  
  msg_type2 *poke = (struct msg_type2 *)(unsigned char *)malloc(sizeof(unsigned char));  
  memcpy(poke->code, &code, sizeof(poke->code));
  memcpy(poke->idPokemon, &randomPoke, sizeof(poke->idPokemon));  
  return poke;
}

/* Construct and return a message with a random Pokemon 
   code and one number of attemps  */

msg_type3* getType3Message(int randomPoke, int *attemps){
  
  int code = 21;  
  msg_type3 *msg = (struct msg_type3 *)(unsigned char *)malloc(sizeof(unsigned char));  
  memcpy(msg->code, &code, sizeof(msg->code));
  memcpy(msg->idPokemon, &randomPoke, sizeof(msg->idPokemon));
  memcpy(msg->numAttemps, attemps, sizeof(msg->numAttemps));
  return msg;
  
}

/* Construct and return a message with a random Pokemon 
   code and one number of attemps, also the pokeImage file */

msg_type4* getType4Message(int randomPoke){

  char* getPokeFile(int rPoke);
  
  int code = 22;
  int imageSize;
  unsigned char* image;
  FILE *picture;
  char* pokeFile = getPokeFile(randomPoke);
  
  //Get Picture Size
  if((picture = fopen(pokeFile, "r")) == 0)
    DieWithError("fopen() failed");
  fseek(picture, 0, SEEK_END);
  imageSize = ftell(picture);
  fseek(picture, 0, SEEK_SET);

  //Store Picture as Byte Array
  image = (unsigned char *)malloc(imageSize);
  if(!image){
    DieWithError("malloc() failed");
  }
  fread(image,imageSize,sizeof(unsigned char),picture);	

  /* Actually construct and return the message */
  
  msg_type4 *msg = (struct msg_type4 *)(unsigned char *)malloc(sizeof(unsigned char));  
  memcpy(msg->code, &code, sizeof(msg->code));
  memcpy(msg->idPokemon, &randomPoke, sizeof(msg->idPokemon));
  memcpy(msg->imageSize, &imageSize, sizeof(msg->imageSize));
  memcpy(msg->image, image, sizeof(msg->image));  
  
  return msg;
  
}


void sendErrorCode(int clntSocket, char* message){
  int code = 40;
  
  msg_typeErr *err = (struct msg_typeErr *)(unsigned char *)malloc(sizeof(unsigned char));
  
  memcpy(err->code, &code, sizeof(err->code));
  memcpy(err->errString, &message, sizeof(err->errString));
  
  if(send(clntSocket, err, sizeof(err), 0) != sizeof(err))
    DieWithError("send() failed");    
}

/* Return the name of the file that contains the image of the pokemon with Id rpoke */

char* getPokeFile(int rPoke){

  switch(rPoke){

  case 0 :
    
    return "images/bulbasaur.png";
    break;
    
  case 1 :
    
    return "images/ivysaur.png";
    break;

  case 2 :
    
    return "images/venusaur.png";
    break;

  case 3 :
    
    return "images/squirtle.png";
    break;

  case 4 :
    
    return "images/wartortle.png";
    break;

  case 5 :
    
    return "images/blastoise.png";
    break;

  case 6 :
    
    return "images/charmander.png";
    break;

  case 7 :
    
    return "images/charmeleon.png";
    break;
    
  default :
    
    return "images/charizard.png";
    
  }
  
}
