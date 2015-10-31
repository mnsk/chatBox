client: client.c
	gcc -o client client.c -pthread -Wall
server: server.c
	gcc -o server server.c -pthread -Wall

clean:
	rm -rf server client

port:
	sudo fuser -k 9734/tcp #enter port number here