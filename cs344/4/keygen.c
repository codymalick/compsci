#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>

#define USAGE "keygen [size]"

void key_gen(int size);

int main(int argc, char* argv[]) {
	if(argv[1]) {
		int size = atoi(argv[1]);	
		key_gen(size);
	} else {
		printf("%s\n", USAGE);
	}
}


void key_gen(int size) {
	srand(time(NULL));
	char key[size];
	int i;
	for(i = 0; i < size; i++) {
		char c = 'A' + (rand() % 26);	//start with 'a', random number between the low and the high	
		key[i] = c;	
	}
	key[i] = '\0';	//Null terminate the string
	printf("%s\n", key);
}
