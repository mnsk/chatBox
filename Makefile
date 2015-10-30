client: client.c
	gcc -o client client.c -Wall
server: server.c
	gcc -o server server.c -Wall

clean:
	rm -rf server client

port:
	sudo fuser -k 9734/tcp #enter port number here