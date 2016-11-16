#include <stdio.h>
#include <openssl/evp.h>

//Required Random lib
#include <stdlib.h>
#include <time.h>

#include <string.h>

void weak_collision(int argc, char *argv[], int attempt);

int main(int argc, char *argv[])
{
	// Seed random number generator
	srand(time(NULL));

	for(int i = 0; i < 100; i++) {	
		weak_collision(argc, argv, i);
	}
	exit(0);
	// EVP_MD_CTX: Message Digest Context
	EVP_MD_CTX *mdctx;

	// EVP_MD: Message Digest, the method we will use
	const EVP_MD *md;

	// Load the message, need to read in data from file
	char mess[100];

	for(int i = 0; i < strlen(mess); i++) {
			mess[i] = 'A' + rand() % 26;
	}

	// The character array the result will be loaded into
	unsigned char md_value[EVP_MAX_MD_SIZE];

	// Length of the hash?
	int md_len, i;

	// Usage
	if(!argv[1]) {
		printf("Usage: hash digestname\n");
		exit(1);
	}

	// Load all algorithms, probably don't need
	OpenSSL_add_all_algorithms();

	// Load all message digests
	OpenSSL_add_all_digests();

	// Get the message digest passed in by command line
	md = EVP_get_digestbyname(argv[1]);

	// Whoops
	if(!md) {
		printf("Unknown message digest %s\n", argv[1]);
		exit(1);
	}

	// Create the message digest context
	mdctx = EVP_MD_CTX_create();

	// Init
	EVP_DigestInit_ex(mdctx, md, NULL);

	// Load the message
	EVP_DigestUpdate(mdctx, mess, strlen(mess));

	// Load md_value with the result
	EVP_DigestFinal_ex(mdctx, md_value, &md_len);

	// Destroy the variable
	EVP_MD_CTX_destroy(mdctx);

	printf("Digest is: ");

	// Only print the first three bytes, per assignment specs 
	for (i = 0; i < 3; i++) {
		printf("%02x", md_value[i]);
	}
	printf("\n");

	printf("Starting Weak Collision Resistance Test:\n");

	unsigned char hash[EVP_MAX_MD_SIZE];
	long unsigned int count = 0;
	char test[100];

	while(memcmp(md_value, hash, 3)) {
		sprintf(test, "%lu", count);

		// Create the message digest context
		mdctx = EVP_MD_CTX_create();

		// Init
		EVP_DigestInit_ex(mdctx, md, NULL);

		// Load the message
		EVP_DigestUpdate(mdctx, test, strlen(test));

		// Load md_value with the result
		EVP_DigestFinal_ex(mdctx, hash, &md_len);

		// Destroy the variable
		EVP_MD_CTX_destroy(mdctx);
	
		count++;

	}	
	//printf("Original:");

	//for (int i = 0; i < 3; i++) {
	//	printf("%02x", md_value[i]);
	//}

	//printf(" Found:");
	
	//for (int i = 0; i < 3; i++) {
	//	printf("%02x", hash[i]);
	//}

	//printf(" Count: %lu\n", count);
	return 0;
}

void weak_collision(int argc, char *argv[], int attempt) {
	// EVP_MD_CTX: Message Digest Context
	EVP_MD_CTX *mdctx;

	// EVP_MD: Message Digest, the method we will use
	const EVP_MD *md;

	// Load the message, need to read in data from file
	char mess[100];

	for(int i = 0; i < strlen(mess); i++) {
			mess[i] = 'A' + rand() % 26;
	}

	// The character array the result will be loaded into
	unsigned char md_value[EVP_MAX_MD_SIZE];

	// Length of the hash?
	int md_len, i;

	// Usage
	if(!argv[1]) {
		printf("Usage: hash digestname\n");
		exit(1);
	}

	// Load all algorithms, probably don't need
	OpenSSL_add_all_algorithms();

	// Load all message digests
	OpenSSL_add_all_digests();

	// Get the message digest passed in by command line
	md = EVP_get_digestbyname(argv[1]);

	// Whoops
	if(!md) {
		printf("Unknown message digest %s\n", argv[1]);
		exit(1);
	}

	// Create the message digest context
	mdctx = EVP_MD_CTX_create();

	// Init
	EVP_DigestInit_ex(mdctx, md, NULL);

	// Load the message
	EVP_DigestUpdate(mdctx, mess, strlen(mess));

	// Load md_value with the result
	EVP_DigestFinal_ex(mdctx, md_value, &md_len);

	// Destroy the variable
	EVP_MD_CTX_destroy(mdctx);

	//printf("Digest is: ");

	// Only print the first three bytes, per assignment specs 
	//for (i = 0; i < 3; i++) {
	//	printf("%02x", md_value[i]);
	//}
	//printf("\n");

	//printf("Starting Weak Collision Resistance Test:\n");

	unsigned char hash[EVP_MAX_MD_SIZE];
	long unsigned int count = 0;
	char test[100];

	while(memcmp(md_value, hash, 3)) {
		sprintf(test, "%lu", count);

		// Create the message digest context
		mdctx = EVP_MD_CTX_create();

		// Init
		EVP_DigestInit_ex(mdctx, md, NULL);

		// Load the message
		EVP_DigestUpdate(mdctx, test, strlen(test));

		// Load md_value with the result
		EVP_DigestFinal_ex(mdctx, hash, &md_len);

		// Destroy the variable
		EVP_MD_CTX_destroy(mdctx);
	
		count++;

	}	
	printf("%d,%lu\n", attempt, count);
}
