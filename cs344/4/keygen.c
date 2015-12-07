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
		printf("USAGE: %s\n", USAGE);
	}
	return 0;
}


void key_gen(int size) {
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
