#include <stdio.h>
#include <openssl/evp.h>
#include <openssl/conf.h>
#include <openssl/err.h>

//Required Random lib
#include <stdlib.h>
#include <time.h>

#include <string.h>

#define BLOCK_SIZE 128

#define CIPHERTEXT "8d20e5056a8d24d0462ce74e4904c1b513e10d1df4a2ef2ad4540fae1ca0aaf9"
#define PLAINTEXT "This is a top secret."

void crack_key();

// The format of the following two functions were based off the examples
// provided from the official openssl wiki:
// https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption
int aes_encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
		unsigned char *iv, unsigned char *ciphertext);

int aes_decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
		unsigned char *iv, unsigned char *plaintext);

// Using aes-128, given the cipher text and the plaintext, with an IV of all
// 0's, crack the key
int main(int argc, char *argv[])
{
	// Initialize the iv
	crack_key();
	// Print it
	return 0;
}

int aes_encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
		unsigned char *iv, unsigned char *ciphertext) {
	EVP_CIPHER_CTX *ctx;

	int len;

	int ciphertext_len;

	/* Create and initialise the context */
	ctx = EVP_CIPHER_CTX_new();

	EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv);

	/* Provide the message to be encrypted, and obtain the encrypted output.
	 *    * EVP_EncryptUpdate can be called multiple times if necessary
	 *       */
	EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len);

	ciphertext_len = len;

	EVP_EncryptFinal_ex(ctx, ciphertext + len, &len);
	ciphertext_len += len;

	// Clean up
	EVP_CIPHER_CTX_free(ctx);

	return ciphertext_len;
}

int aes_decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
		unsigned char *iv, unsigned char *plaintext) {
	EVP_CIPHER_CTX *ctx;

	int len;

	int plaintext_len;

	ctx = EVP_CIPHER_CTX_new();

	EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv);

	EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len);

	plaintext_len = len;

	EVP_DecryptFinal_ex(ctx, plaintext + len, &len);
	plaintext_len += len;

	/* Clean up */
	EVP_CIPHER_CTX_free(ctx);

	return plaintext_len;
}

void crack_key() {
	unsigned char *iv[BLOCK_SIZE];
	memset(iv, 0x00, BLOCK_SIZE);

	//	unsigned char *key[BLOCK_SIZE];
	unsigned char *key = (unsigned char *)"01234567890123456";
	unsigned char *ciphertext[BLOCK_SIZE];

	unsigned char *plaintext = (unsigned char *)PLAINTEXT;

	unsigned char decryptedtext[BLOCK_SIZE];
	int decryptedtext_len, ciphertext_len;

	// Load the library
	ERR_load_crypto_strings();
	OpenSSL_add_all_algorithms();
	OPENSSL_config(NULL);

	unsigned long int count = 0;

	// Encrypt
	ciphertext_len = aes_encrypt(plaintext, strlen((char *)plaintext), key,
			iv, ciphertext);

	printf("Ciphertext:\n");
	BIO_dump_fp (stdout, (const char *)ciphertext, ciphertext_len);
	//	while(memcmp(CIPHERTEXT,test_cipher,64)) {

	decryptedtext_len = aes_decrypt(ciphertext, ciphertext_len, key, iv,
			decryptedtext);

	/* Add a NULL terminator. We are expecting printable text */
	decryptedtext[decryptedtext_len] = '\0';

	/* Show the decrypted text */
	printf("Decrypted text is:\n");
	printf("%s\n", decryptedtext);

	/* Clean up */
	EVP_cleanup();
	ERR_free_strings();
}
