#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 9734
#define SIZE 1024
#define MAX_FD 5

char buffer[SIZE];

int main(int argc, char const *argv[])
{
	int clientSocketfd;
	struct sockaddr_in serverAddr;
	//struct sockaddr_in clientAddr;

	clientSocketfd = socket(AF_INET,SOCK_STREAM,0);
	if(clientSocketfd == -1)
		perror("\nError in creating socket");

	/*Configuring settiings for server address structure*/
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("192.168.1.5");  //server's ip address

	if(connect(clientSocketfd,(struct sockaddr *)&serverAddr,sizeof(serverAddr)) == -1)
		perror("\nError in connecting");


	// while(1) {
	// 	int client_size;

	// 	printf("\nServer Waiting");

	// 	client_size = sizeof(clientAddr);
	// 	clientSocketfd = accept(serverSocketfd,(struct sockaddr *)&clientAddr,&client_size);
	// 	if(clientSocketfd == -1)
	// 		perror("\nError in accepting");

		if(write(clientSocketfd,"Who is this?",12) == -1)
			perror("\nError in writing");
		
		if(read(clientSocketfd,buffer,SIZE) == -1)
			perror("\nError in reading");
		
		printf("\nServer send: %s",buffer);

		close(clientSocketfd);
	//}

	return 0;
}

