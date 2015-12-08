//From example file provided by teacher

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFF 50000

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[BUFF];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd,5);

	char *key;
	char *message;

	while(1) {

		clilen = sizeof(cli_addr);
     		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
     		if (newsockfd < 0) 
          		error("ERROR on accept");
		int pid = fork();
		if(pid == 0) {
			bzero(buffer,BUFF);
     			n = read(newsockfd,buffer,BUFF);
     			if (n < 0) error("ERROR reading from socket");
     			//printf("%s\n",buffer);

			//do stuff
			message = strtok(buffer, "~");	
			key = strtok(NULL, "`");	

			message = strtok(message, "\n");
			key = strtok(key, "\n");
			
			//printf("%s\n", key); //debug
			//printf("%s\n", message); //debug

			//encrypt
			
			char set[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
			int strlength = strlen(message);
			int j, n, m, t;
			char cipher[BUFF];
			for(j = 0; j < strlength; j++) {
				n = message[j] - 65;
				m = key[j] - 65;
				if(n == -33)
					n = 26;
				if(m == -33)
					m = 26;
				t = (n+m) % 27;
				if(t < 0)
					t += 27;
				cipher[j] = set[t];
			}

			cipher[j] = '\n';
     			n = write(newsockfd, cipher, BUFF);
     			if (n < 0) error("ERROR writing to socket");
			break;
		}
     		else {
			close(newsockfd);
		}
     		//close(sockfd);
	}
         return 0; 
}
