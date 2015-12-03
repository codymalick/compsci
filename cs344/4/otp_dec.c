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
        strcpy(ciphertext, argv[1]);

        int strlength = strlen(ciphertext);
        ciphertext[strlength] = '\n';

        //printf("ciphertext %s, key: %s, port: %i\n", ciphertext, key, port);

        int i;
        for(i = 0; i < 4096; i++) {
                if(ciphertext[i] == '\n') {
			printf("%s\n", message);
			exit(0);
                }

		//printf("ciphertext[i]: %i, key[i]: %i, difference: %i, modulus: %i\n", ciphertext[i], key[i], (ciphertext[i] - key[i]), ((ciphertext[i] - key[i]) % 26));
		if((ciphertext[i] - key[i]) < 0) {
			message[i] = ('Z' + ((ciphertext[i] - key[i]) + 1 % 26));
		} else {
	                message[i] = ('A' + ((ciphertext[i] - key[i]) % 26));
		}
        }
        exit(1);
        return 0;
}
