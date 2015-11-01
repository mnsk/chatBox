#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#define PORT 9734
#define SIZE 1024
#define MAX_FD 5

pthread_mutex_t arrayMutex = PTHREAD_MUTEX_INITIALIZER;

int array[MAX_FD] = {0};  //array for storing file descriptors
int i = 0;

void *servThread (void *data) {
	time_t ltime;
	char buffer[SIZE];
	char buffer_write[SIZE];
	int *clientSocketfd = (int *)data;
	int j,k;
	k = *clientSocketfd;

	printf("\nClient %d connected...",*clientSocketfd);

	char statement[17] = "You are Client: ";
	char ch[1];
	sprintf(ch,"%d",*clientSocketfd);
	strcat(statement,ch);

	if(write(*clientSocketfd,statement,strlen(statement)) == -1) {
		perror("\nCant send client number");
	}
	// for checking array of fds
	printf("Array: ");
	for(j = 0; j< MAX_FD; j++) {
		printf("%d ",array[j]);
	}
	printf("\n");

		while(1) {

				*clientSocketfd = k;     //workaround for the bug(file descriptor getting lost)

				memset(buffer, 0, SIZE);	
				memset(buffer_write, 0, SIZE);  //making the buffer 0

				int ret = read(*clientSocketfd,buffer,SIZE);
				if(ret == -1) {
					perror("\nError in reading");
					break;
				}
		
				//if(strncmp(buffer,"exit",4) != 0) {
		
				printf("\nClient %d: %s",*clientSocketfd,buffer);

				if(ret > 0) {	
					//pthread_mutex_lock(&arrayMutex);
					for(j = 0; j < i; j++) {
						if(array[j] != *clientSocketfd) {
						//pthread_mutex_lock(&arrayMutex);
						/*giving a time stamp and concatinating the buffer*/ 
							ltime=time(NULL);
							strcat(buffer_write,asctime(localtime(&ltime)));
							strcpy(statement,"Client ");
							strcat(statement,ch);
							strcat(statement,": ");
							strcat(buffer_write,statement);
							strcat(buffer_write,buffer);
							
						/*broadcasting to all file descriptors except own one*/
							if(write(array[j],buffer_write,strlen(buffer_write)) == -1) {
								perror("\nError in writing");
								break;
							}
						//pthread_mutex_unlock(&arrayMutex);
						}
					}
					//pthread_mutex_unlock(&arrayMutex);
				}
				else if(ret == 0) {
					printf("Closing client %d\n",*clientSocketfd);
					close(*clientSocketfd);
					break;
				}

			}
		pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
	pthread_t servThreadID;
	int serverSocketfd,clientSocketfd;
	struct sockaddr_in serverAddr;
	struct sockaddr_in clientAddr;
	//int arr[MAX_FD];

	serverSocketfd = socket(AF_INET,SOCK_STREAM,0);
	if(serverSocketfd == -1){
		perror("\nError in creating socket");
		return 0;
	}
	printf("Socket created...\n");

	/*Configuring settiings for server address structure*/
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(serverSocketfd,(struct sockaddr *)&serverAddr,sizeof(serverAddr)) == -1) {
		perror("\nError in assigning name to socket");
		return 0;
	}
	printf("Binding complete..\n");

	if(listen(serverSocketfd,MAX_FD) == -1) {
		perror("/nError in queueing");
		return 0;
	}

	while(1) {
		int client_size;

		printf("\nServer Waiting...");

		client_size = sizeof(clientAddr);
		clientSocketfd = accept(serverSocketfd,(struct sockaddr *)&clientAddr,&client_size);
		if(clientSocketfd == -1) {
			perror("\nError in accepting");
			return 0;
		}
	
		if(i < MAX_FD) {
			array[i] = clientSocketfd;	
			if(pthread_create(&servThreadID,NULL,servThread,&clientSocketfd) != 0) {
					printf("Error in creating thread.\n");
					return 0;

			}			
			i++;
		}
		else{
			printf("Server Busy...\n");
			break;
		}

	}
	pthread_mutex_destroy(&arrayMutex);

	return 0;
}

