/*foncUDP
*Bibliotheque contenant les fonctions de couche transport pour les TP-PRS
*


*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "foncUDP.h"

/*On fait la connection avec le UDP. On a besoin de descriptor de socket, la struct avec l'adresse donc on
 * fera la "connection" en envoyant le SYN-ACK, le size du message a recevoir et une struct vide donc
 * on envoyera les octets.
 */
int acceptU(int des,struct sockaddr_in adresse,int size, struct sockaddr_in* nouadr)
{
	int activ;
	int nouport = socket(AF_INET,SOCK_DGRAM,0); //Nouveau socket où on fera l'autre connection.
	int valid = 1;
	//struct sockaddr_in nouadr;
	char msg[size]; //Buffer des messages de taille size
	socklen_t taille= sizeof(adresse); //Taille de adresse garder en taille pour le recvfrom et sendto.


		/*On  reçcoit le premiere message pour faire la connection.
		 * On verificer que soit "SYN", puis on envoye le "SYN-ACK", on atendre le "ACK"
		 * En fin on envoyer le nouvelle adresse du socket donc on utilisera pour envoyer les octets.
		 */
		int rec= recvfrom(des,msg,size,0,(struct sockaddr*)&adresse,&taille);
		printf("%s %d\n",msg,strcmp(msg,"SYN")); //Test Effacer Apres
		if(strcmp(msg,"SYN")==0)
		{
			printf("Entre\n");
			strcpy(msg,"SYN-ACK");
			printf("Msg Envoye: %s\n",msg);
			sendto(des,msg,strlen(msg)+1,0,(struct sockaddr*)&adresse,sizeof(adresse));
			activ=recvfrom(des,msg,size,0,(struct sockaddr*)&adresse,&taille);
			printf("%s \n",msg);
			if(strcmp(msg,"ACK")==0)
			{

				if (nouport < 0) {
					perror("cannot create socket\n");
					return -1;
				}

				setsockopt(nouport, SOL_SOCKET, SO_REUSEADDR, &valid, sizeof(int));
					nouadr->sin_family= AF_INET;
					nouadr->sin_port= htons(ntohs(adresse.sin_port)+1);
					nouadr->sin_addr.s_addr= htonl(INADDR_ANY);


				while (bind(nouport, (struct sockaddr*) nouadr, sizeof(*nouadr)) == -1  ) {
					perror("Bind fail\n");
					nouadr->sin_port= htons(ntohs(nouadr->sin_port)+1);
				}


				sprintf(msg, "%d", ntohs(nouadr->sin_port));
				sendto(des,msg,strlen(msg)+1,0,(struct sockaddr*)&adresse,sizeof(adresse));
				printf("Connection avec le client a été fait correctement %d %d %s\n",ntohs(nouadr->sin_port),ntohs(adresse.sin_port),msg);
				return nouport;
			}
		}
	printf("Echec\n");
	return -1;
}

int connectU(int desc,struct sockaddr_in* adresse,int size)
{

	char msg[size];
	socklen_t taille= sizeof(*adresse);
	printf("%d\n",taille);


    strcpy(msg,"SYN");
	int sent= sendto(desc,msg,strlen(msg)+1,0,(struct sockaddr*)adresse,sizeof(*adresse));
	printf("%d\n",sent);
	int rec= recvfrom(desc,msg,size,0,(struct sockaddr*)adresse,&taille);
	printf("%s %d \n",msg,rec);


	if(strcmp("SYN-ACK",msg)==0){
		strcpy(msg,"ACK");
		if(sendto(desc,msg,strlen(msg)+1,0,(struct sockaddr*)adresse,sizeof(*adresse))>=0)
		{
			recvfrom(desc,msg,size,0,(struct sockaddr*)adresse,&taille);

			printf("Succes: Connection fait %s\n",msg);
			return atoi(msg);
		}
	}

	printf("Echec\n");
	return -1;
}

int envoyerU( struct sockaddr_in adresse , char* name)
{
	char msg[1024];
	int seg=0;
	int desc = socket(AF_INET,SOCK_DGRAM,0);
	int size;
	Segment buffer = (Segment)malloc(sizeof(trame));
	FILE *fl = fopen(name,"r");
	socklen_t taille=sizeof(adresse);
	sendto(desc,name,strlen(name)+1,0,(struct sockaddr*)&adresse,sizeof(adresse));

	size=fread(buffer->num,1,64,fl);
	while(size==64){
		buffer->info=seg;
		seg++;
		buffer->size=size;
		sendto(desc,buffer,sizeof(trame),0,(struct sockaddr*)&adresse,sizeof(adresse));
		printf("Segment %d envoyé en attende du ACK\n",buffer->info);
		recvfrom(desc,msg,1024,0,(struct sockaddr*)&adresse,&taille);
		printf("ACK %s reçu, envoyant le prochaine segmnet",msg);
		size=fread(buffer->num,1,64,fl);
		}
	buffer->size=size;
	buffer->info=seg;
	sendto(desc,buffer,sizeof(trame),0,(struct sockaddr*)&adresse,sizeof(adresse));
	fclose(fl);
	}

int recevoirU(int desc, struct sockaddr_in adresse)
{	socklen_t taille = sizeof(adresse);
	char name[1024];
	char msg[1024];
	Segment buffer=(Segment)malloc(sizeof(trame));
	FILE *fl;
	recvfrom(desc,name,1024,0,(struct sockaddr*)&adresse,&taille);
	printf("%s\n",name);
	fl = fopen(name,"w");
	recvfrom(desc,buffer,sizeof(trame),0,(struct sockaddr*)&adresse,&taille);
	while(buffer->size==64)
	{
		printf("Segment %d reçu, envoyant le ACK\n",buffer->info);
		sprintf(msg, "%d", buffer->info);
		sendto(desc,msg,sizeof(msg)+1,0,(struct sockaddr*)&adresse,sizeof(adresse));
		fwrite(buffer->num,1,64,fl);
		recvfrom(desc,buffer,sizeof(trame),0,(struct sockaddr*)&adresse,&taille);
		}
	fwrite(buffer->num,1,buffer->size,fl);
	fclose(fl);
	return 1;
	}
