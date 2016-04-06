#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#define BUFF 700000 

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char* argv[]) {
	
	//testing
	if(argc != 4) {
		printf("usage: otp_enc [plaintext] [key] [port]\n");
		exit(1);
	}

	if(access( argv[2], F_OK) == -1) {
		fprintf(stderr, "File: %s, does not exist\n", argv[2]);
		exit(1);
	}

	if(access( argv[1], F_OK) == -1) {
		fprintf(stderr, "File: %s, does not exist\n", argv[1]);
		exit(1);
	}

	int sockfd, portno, n;
    	struct sockaddr_in serv_addr;
    	struct hostent *server;

    	char buffer[BUFF];
    	if (argc < 3) {
       		fprintf(stderr,"usage %s hostname port\n", argv[0]);
       		exit(0);
    	}
    	portno = atoi(argv[3]);
    	sockfd = socket(AF_INET, SOCK_STREAM, 0);
    	if (sockfd < 0) 
        	error("ERROR opening socket");
    	server = gethostbyname("localhost");
    	if (server == NULL) {
        	fprintf(stderr,"ERROR, no such host\n");
        	exit(0);
   	}
    	bzero((char *) &serv_addr, sizeof(serv_addr));
    	serv_addr.sin_family = AF_INET;
    	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    	serv_addr.sin_port = htons(portno);
    	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        	error("ERROR connecting");

	//printf("Please enter the message: ");
    	bzero(buffer,BUFF);

	//READ FROM FILE
	int keysize, messagesize;

	FILE *fp = fopen(argv[1], "r");	
	char message[BUFF];
	fgets(message, sizeof(message), fp);
	fclose(fp);

	
	char key[BUFF];
	fp = fopen(argv[2], "r");
	fgets(key, sizeof(key), fp);
	fclose(fp);

	struct stat st;
	stat(argv[1], &st);
	messagesize = st.st_size;

	stat(argv[2], &st);
	keysize = st.st_size;

	if(keysize < messagesize) {
		fprintf(stderr, "Error: %s is too short", argv[2]);
		exit(1);
	}

	char encrypt_key[4];
	strcpy(encrypt_key, "e|");	

	//indicate that this string needs encrypting
	strcpy(buffer, encrypt_key);

	strcat(buffer, message);
	strcat(buffer, "~");
	strcat(buffer, key);
	strcat(buffer, "`");

	//SEND STRING

    	n = write(sockfd,buffer,strlen(buffer));
    	if (n < 0) 
        	error("ERROR writing to socket");
    	bzero(buffer,BUFF);
    	n = read(sockfd,buffer,BUFF);
    	if (n < 0) 
        	error("ERROR reading from socket");
    	printf("%s",buffer);
	close(sockfd);	

	exit(0);

}
