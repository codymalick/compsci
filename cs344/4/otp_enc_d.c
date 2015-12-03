#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<sys/types.h>
#include<errno.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>

#define USAGE "otp_enc_d [listening_port]"
#define BUFF 4096

char key[4096];

char* encrypt(char* plain[BUFF]);

//otp_enc_d [listening_port]
int main(int argc, char* argv[]) {

	if(argc < 2 || !argv[1]) {
		fprintf(stderr, "USAGE: %s\n", USAGE);
		exit(1);
	}
	int port;
	if(!(port = atoi(argv[1]))) {
		fprintf(stderr, "Invalid Input: argv[1]");
		exit(1);
	}

	//Temp for testing pipes
	//if(argc < 3) {
	//	scanf("%s", key);
	//	printf("key %s\n", key);
	//}
		
	printf("Encryption Daemon\n");
	printf("Creating Socket:\n");

	struct sockaddr_in server;
	int sock;
	char message[BUFF];	

	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = INADDR_ANY;

	//Create the socket and set it up to accept connections
	sock = socket(AF_INET, SOCK_STREAM, 0);

	bind(sock, (struct sockaddr *) &server, sizeof(server));

	if(listen(sock, 1) < 0) {
		perror("listen");
		exit(1);
	}

	int clientsock = accept(sock, NULL, NULL);

	ssize_t r;
	while((r = recv(clientsock, message, sizeof(message), 0)));
	printf("%s\n", message); 
	
	return 0;
}
