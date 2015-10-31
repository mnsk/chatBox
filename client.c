#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#define PORT 9734
#define SIZE 1024
#define MAX_FD 5

char buffer_write[SIZE];
char buffer_read[SIZE];


void *rcvThread (void *data) {
	int *clientSocketfd = (int *)data;

	while(1) {
		memset(buffer_read, 0, SIZE);

		int ret = read(*clientSocketfd,buffer_read,SIZE);

		if( ret == -1) {
			perror("\nError in reading");
			close(*clientSocketfd);
			break;
		}
		if(ret > 0)
		printf("\nServer send: %s",buffer_read);
		else if(ret == 0) {
			printf("Closed\n");
			close(*clientSocketfd);
			break;
		}

	}
	pthread_exit(NULL);
}
	

int main(int argc, char const *argv[])
{
	pthread_t rcvId;
	int clientSocketfd,n;
	struct sockaddr_in serverAddr;
	//struct sockaddr_in clientAddr;

	clientSocketfd = socket(AF_INET,SOCK_STREAM,0);
	if(clientSocketfd == -1) {
		perror("\nError in creating socket");
		return 0;
	}

	/*Configuring settiings for server address structure*/
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  //server's ip address


	if(connect(clientSocketfd,(struct sockaddr *)&serverAddr,sizeof(serverAddr)) == -1) {
		perror("\nError in connecting");
		return 0;
	}

	if(pthread_create(&rcvId,NULL,rcvThread,&clientSocketfd) != 0) {
			printf("Error in creating thread.\n");
			return 0;
		}
	
	while(1) {
		memset(buffer_write, 0, SIZE);
		n = 0;

		printf("\nmsg@ ");
		while((buffer_write[n++]=getchar())!='\n');

		if(write(clientSocketfd,buffer_write,strlen(buffer_write)) == -1) {
			perror("\nError in writing");
			break;
		}

	// if(strncmp(buffer_write,"exit",4) != 0) {
		
	// 	if(write(clientSocketfd,buffer_write,strlen(buffer_write)) == -1)
	// 		perror("\nError in writing");
	// }
	// else { 

	// 	if(write(clientSocketfd,"exit",4) == -1)
	// 		perror("\nError in writing");
	// 	close(clientSocketfd);
	// 	break;
	// }
			
	}

	return 0;
}

