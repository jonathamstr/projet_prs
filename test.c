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
	struct sockaddr_in adresse, nouadr, auxadr;
	int valid= 1;
	socklen_t alen;
	char buffer[RCVSIZE];
	int desc= socket(AF_INET,SOCK_DGRAM,0);
	int portData;

	if (argc < 2){
		perror("Il n'y a pas assez arguments\n");
		return -1;
	}

	if (desc < 0) {
		perror("cannot create socket\n");
		return -1;
	}

	setsockopt(desc, SOL_SOCKET, SO_REUSEADDR, &valid, sizeof(int));
	adresse.sin_family= AF_INET;
	adresse.sin_port= htons(atoi(argv[1]));
	adresse.sin_addr.s_addr= htonl(INADDR_ANY);
	printf("%d\n",ntohs(adresse.sin_port));
	if (bind(desc, (struct sockaddr*) &adresse, sizeof(adresse)) == -1) {
		perror("Bind fail\n");
		close(desc);
		return -1;
	}

	//while (1) {
		portData = acceptU(desc,adresse,RCVSIZE,&nouadr);
		auxadr.sin_family= AF_INET;
		auxadr.sin_port= htons(portData);
		auxadr.sin_addr.s_addr= htonl(INADDR_ANY);
		printf("%d\n",ntohs(nouadr.sin_port));
		socklen_t taille= sizeof(nouadr);
		int rec= recvfrom(portData,buffer,RCVSIZE,0,(struct sockaddr*)&auxadr,&taille);
		printf("recv= %s\n", buffer);
		recevoirU(portData,auxadr);
		/*sendto(desc,buffer,strlen(buffer),0,(struct sockaddr*)&adresse,sizeof(adresse));
		printf("%s",buffer);
		recvfrom(desc,buffer,RCVSIZE,0,(struct sockaddr*)&adresse,&taille);
		printf("%s",buffer);*/
	//}

 }
