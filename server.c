#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 9000
#define SIZE 1024
#define MAX_FD 5

char buffer[SIZE];

int main(int argc, char const *argv[])
{
	int serverSocketfd,clientSocketfd;
	struct sockaddr_in serverAddr;
	struct sockaddr_in clientAddr;

	serverSocketfd = socket(AF_INET,SOCK_STREAM,0);
	if(serverSocketfd == -1)
		perror("\nError in creating socket");

	/*Configuring settiings for server address structure*/
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(serverSocketfd,(struct sockaddr *)&serverAddr,sizeof(serverAddr)) == -1)
		perror("\nError in assigning name to socket");

	if(listen(serverSocketfd,MAX_FD) == -1) 
		perror("/nError in queueing");

	while(1) {
		int client_size;

		printf("Server Waiting\n");

		client_size = sizeof(clientAddr);
		clientSocketfd = accept(serverSocketfd,(struct sockaddr *)&clientAddr,&client_size);
		if(clientSocketfd == -1)
			perror("\nError in accepting");
		
		if(read(clientSocketfd,buffer,SIZE) == -1)
			perror("\nError in reading");
		
		printf("\nClient send: %s",buffer);

		if(write(clientSocketfd,"This is server.",15) == -1)
			perror("\nError in writing");

		close(clientSocketfd);
	}

	return 0;
}

