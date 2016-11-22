#include <stdio.h>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/conf.h>
#include <openssl/err.h>

#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 128

#define CIPHERTEXT "8d20e5056a8d24d0462ce74e4904c1b513e10d1df4a2ef2ad4540fae1ca0aaf9"
#define PLAINTEXT "This is a top secret."

unsigned char *aes_encrypt(EVP_CIPHER_CTX *e, unsigned char *plaintext, int *len)
{
	// clen for maxiumum cipher length size 
	int clen = *len + AES_BLOCK_SIZE;

	// variable for tracking encryption length
	int flen = 0;

	unsigned char *ciphertext = malloc(clen);

	// run the encryption
	EVP_EncryptInit_ex(e, NULL, NULL, NULL, NULL);
	EVP_EncryptUpdate(e, ciphertext, &clen, plaintext, *len);
	EVP_EncryptFinal_ex(e, ciphertext+clen, &flen);

	return ciphertext;
}

// Using aes-128, given the cipher text and the plaintext, with an IV of all
// 0's, crack the key
int main(int argc, char *argv[])
{
	FILE* file = fopen("words.txt", "r"); /* should check the result */

	char line[256];

	unsigned long int count = 0;

	unsigned char decryptedtext[AES_BLOCK_SIZE];
	unsigned char *ciphertext;
	unsigned char *real_ciphertext = (unsigned char *)CIPHERTEXT;
	unsigned char *plaintext = (unsigned char *)PLAINTEXT;

	ssize_t read;

	int match = 0;
	while (fgets(line, sizeof(line), file)) {
		printf("%s", line);

		int strlength = strlen(line);

		unsigned char key[16];

		for(int i = 0; i < 16; i++) {
			key[i] = ' ';
		}

		if(strlength > 16) {
			for(int i = 0; i < strlength; i++) {
				key[i] = line[i];
				if(i == 15) break;
			}
		} else {
			for(int i = 0; i < strlength-1; i++) {
				key[i] = line[i];
			}
		}

		//printf("%d\n", strlen(key));
		//int count = 0;

		// Need to track the enc/dec process	
		EVP_CIPHER_CTX enc, dec;

		// Initialize contexts
		EVP_CIPHER_CTX_init(&enc);
		EVP_EncryptInit_ex(&enc, EVP_aes_128_cbc(), NULL, key, 0x0);
		EVP_CIPHER_CTX_init(&dec);
		EVP_DecryptInit_ex(&dec, EVP_aes_128_cbc(), NULL, key, 0x0);

		int len = 16;
		// Context, key, length variable
		ciphertext = aes_encrypt(&enc, (unsigned char *)plaintext, &len);
		
		printf("cipher: ");
		for(int i = 0; i < strlen(ciphertext); i++) {
			printf("%02x", ciphertext[i]);
		}

		printf("\n");
	}
}

