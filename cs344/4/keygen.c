/*********************************
 * File Name: Keygen.c
 * Function: Generates a key based
 * on input parameter
**********************************/

#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>

#define USAGE "keygen [size]"

void key_gen(int size);

int main(int argc, char* argv[]) {
	//checks for correct input
	if(argv[1]) {
		int size = atoi(argv[1]);	
		key_gen(size);
	} else {
		fprintf(stderr, "USAGE: %s\n", USAGE);
	}
	return 0;
}

//function generates the actual key
void key_gen(int size) {
	//seed random
	srand(time(NULL));
	char key[size];
	int i;
	for(i = 0; i < size; i++) {
		char c = 'A' + (rand() % 27);	//start with 'a', random number between the low and the high
		if(c == '[') {
			c = ' ';
		}
		key[i] = c;	
	}
	key[i] = '\0';	//Null terminate the string
	printf("%s\n", key);
}
