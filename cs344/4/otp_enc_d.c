#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<sys/types.h>
#include<errno.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>

#define USAGE "otp_enc_d [listening_port]"

char key[4096];

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
	if(argc < 3) {
		scanf(i"%s", key);
	}
		
	printf("Encryption Daemon\n");
	printf("Creating Socket:\n");

	struct sockaddr_in server;
	int sock;

	//Create the socket and set it up to accept connections
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(listen(sock, 1) < 0) {
		perror("listen");
		exit(1);
	}
	
	printf("Socket created");
	return 0;
}
