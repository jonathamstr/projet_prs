//foncUDP.h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

struct ST_TRAME
{
	int info;
	char num[64];
	int size;
};
typedef struct ST_TRAME trame;
typedef trame* Segment;


int acceptU(int des,struct sockaddr_in adresse,int size);
int connectU(int desc,struct sockaddr_in* adresse,int size);
int envoyerU( struct sockaddr_in adresse , char* name);
int recevoirU(int desc, struct sockaddr_in adresse);
