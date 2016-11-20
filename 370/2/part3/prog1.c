#include <stdio.h>
#include <openssl/evp.h>

//Required Random lib
#include <stdlib.h>
#include <time.h>

#include <string.h>

#define TRIALS 500

typedef int bool;
#define true 1
#define false 0

void weak_collision(int argc, char *argv[], int attempt);
void strong_collision(int argc, char *argv[], int attempt);

struct llist;
struct llist* ll_get_next(struct llist *l);
int llist_compare(struct llist *l, struct llist *m);
void print_arr(unsigned char *arr);

int main(int argc, char *argv[])
{
	// Seed random number generator
	srand(time(NULL));
	printf("Strong Hash Collision, trial, attempt\n");
	for(int i = 0; i < TRIALS; i++) {
		strong_collision(argc, argv, i);
	}


	printf("Weak Hash Collision, trial, attempt\n");
	for(int i = 0; i < TRIALS; i++) {	
		weak_collision(argc, argv, i);
	}
	return 0;
}

void print_arr(unsigned char *arr) {
	for (int i = 0; i < 3; i++) {
		printf("%02x", arr[i]);
	}
	printf("\n");

}
struct llist {
	struct llist *next;
	char mes[100];
	unsigned char digest[EVP_MAX_MD_SIZE];
};

struct llist* ll_get_next(struct llist *l) {
	return l->next;
}

int llist_compare(struct llist *l, struct llist *m) {
	struct llist *tmp = l;
	while(true) {
		if(!memcmp(tmp->digest, m->digest, 3)) {
			return 1;
		}
		if(tmp != NULL && tmp->next != NULL) {
			tmp = tmp->next;
		} else {
			break;
		}
	}
	tmp->next = m;

	return 0;
}

void strong_collision(int argc, char *argv[], int attempt) {
	
	// EVP_MD_CTX: Message Digest Context
	EVP_MD_CTX *mdctx;

	// EVP_MD: Message Digest, the method we will use
	const EVP_MD *md;

	// Load the message, need to read in data from file
	struct llist *list = (struct llist*)malloc(sizeof(struct llist));

	// First item in the linked list
	for(int i = 0; i < 100; i++) {
		list->mes[i] = 'A' + rand() % 26;
	}

	// The character array the result will be loaded into
	unsigned char md_value[EVP_MAX_MD_SIZE];

	// Length of the hash
	unsigned int md_len;

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
	EVP_DigestUpdate(mdctx, list->mes, strlen(list->mes));

	// Load md_value with the result
	EVP_DigestFinal_ex(mdctx, list->digest, &md_len);

	// Destroy the variable
	EVP_MD_CTX_destroy(mdctx);

	long unsigned int count = 0;
	int match = 0;
	
	while(match == 0) {
		// Generate next hash
		struct llist *next = (struct llist*)malloc(sizeof(struct llist));
		next->next = NULL;	
		for(int i = 0; i < 100; i++) {
			next->mes[i] = 'A' + rand() % 26;
		}


		// Create the message digest context
		mdctx = EVP_MD_CTX_create();

		// Init
		EVP_DigestInit_ex(mdctx, md, NULL);

		// Load the message
		EVP_DigestUpdate(mdctx, next->mes, strlen(next->mes));

		// Load md_value with the result
		EVP_DigestFinal_ex(mdctx, next->digest, &md_len);

		// Destroy the variable
		EVP_MD_CTX_destroy(mdctx);

		count++;
		// Check value against existing values
	
		//print_arr(next->digest);
		//print_arr(next->mes);

		match = llist_compare(list, next);
	}	
	printf(",%d,%lu\n", attempt, count);
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
	printf(",%d,%lu\n", attempt, count);
}
