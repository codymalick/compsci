#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define BUFF 4096

int main(int argc, char* argv[]) {
	
	//testing
	if(argc != 4) {
		printf("usage: otp_enc [plaintext] [key] [port]");
		exit(1);
	}
	
	char message[BUFF];
	char key[BUFF];
	int port = atoi(argv[3]);
	char ciphertext[BUFF];	
	strcpy(key, argv[2]);
	strcpy(message, argv[1]);
	
	int strlength = strlen(message);
	message[strlength] = '\n';

	printf("message: %s, key: %s, port: %i\n", message, key, port);

	int i;
	for(i = 0; i < 4096; i++) {
		if(message[i] == '\n') {
			exit(0); 
		}
		//printf("A: %i, Message[i]: %i, Key[i]: %i\n", 'A', message[i], key[i]);
		
		//printf("%i\n", ('A' + ((message[i] + key[i]) % 26)));

		ciphertext[i] = ('A' + ((message[i] + key[i]) % 26));

		printf("%c", ciphertext[i]);
	}
	exit(1);
	return 0;
}
