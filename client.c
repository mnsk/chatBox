#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define PORT 9734
#define SIZE 1024
#define MAX_FD 5

char buffer_write[SIZE];
char buffer_read[SIZE];

int main(int argc, char const *argv[])
{
	int clientSocketfd,n;
	struct sockaddr_in serverAddr;
	//struct sockaddr_in clientAddr;

	clientSocketfd = socket(AF_INET,SOCK_STREAM,0);
	if(clientSocketfd == -1)
		perror("\nError in creating socket");

	/*Configuring settiings for server address structure*/
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  //server's ip address


	if(connect(clientSocketfd,(struct sockaddr *)&serverAddr,sizeof(serverAddr)) == -1)
		perror("\nError in connecting");
	
	while(1) {
		memset(buffer_write, 0, SIZE);
		memset(buffer_read, 0, SIZE);
		n = 0;

		printf("\nmsg@ ");
		while((buffer_write[n++]=getchar())!='\n');

		if(write(clientSocketfd,buffer_write,strlen(buffer_write)) == -1)
			perror("\nError in writing");
		
		if(read(clientSocketfd,buffer_read,SIZE) == -1)
			perror("\nError in reading");
		
		printf("\nServer send: %s",buffer_read);

		//close(clientSocketfd);
	}

	return 0;
}

