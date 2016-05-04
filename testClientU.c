//test Client 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include "foncUDP.h"
#define RCVSIZE 1024


int main (int argc, char *argv[]) {

  struct sockaddr_in adresse, nouadr;
  int port= 5001;
  int valid= 1;
  char msg[RCVSIZE];
  char blanmsg[RCVSIZE];
  int desce = socket(AF_INET, SOCK_DGRAM, 0);

  //create socket
  int desc= socket(AF_INET, SOCK_DGRAM, 0);
  if(argc < 3){
	perror("Non assez nombre de arguments\n");
	return -1;
	}
  // handle error
  if (desc < 0) {
    perror("cannot create socket\n");
    return -1;
  }

  
  inet_aton(argv[1],&adresse.sin_addr);
  inet_aton(argv[1],&nouadr.sin_addr);	
  adresse.sin_family= AF_INET;
  nouadr.sin_family= AF_INET;
  adresse.sin_port= htons(atoi(argv[2]));
  port = connectU(desc,&adresse,RCVSIZE);
  printf("%d\n",port);
  nouadr.sin_port = htons(port);
  strcpy(msg,"Chose");
  sendto(desce,msg,strlen(msg)+1,0,(struct sockaddr*)&nouadr,sizeof(nouadr));
  printf("%s\n",msg);
  envoyerU(nouadr,"projet.pdf");
  /*socklen_t taille= sizeof(adresse);
     memset(msg, '\0', sizeof(msg));
     strcpy(msg,"SYN\0");
	int sent= sendto(desc,msg,strlen(msg),0,(struct sockaddr*)&adresse,sizeof(adresse));
	//printf("sent %d\n", sent);
	int rec= recvfrom(desc,msg,strlen(msg),0,(struct sockaddr*)&adresse,&taille);
	//printf("rec %d\n", rec);
	 memset(msg, '\0', sizeof(msg));
	strcpy(msg,"ACK\0");
	sendto(desc,msg,strlen(msg),0,(struct sockaddr*)&adresse,sizeof(adresse));
	* */
close(desc);
return 0;
}
