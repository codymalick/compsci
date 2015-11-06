/*******************************************************
 * Author: Cody Malick
 * Program: Adventure program that the user has fun with!
 *******************************************************/

#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<time.h>


//Function declarations
void gen_files(char* dir);
void assign_names();
void generate_connections();
void assign_types();
void read_rooms(char* dir);
void game_loop();
int get_start_room();
int find_room(char* name);

//Struct/Variable Declarations
struct room {
	char name[30];
	char connections[6][30];	
	char type[30];
	int count;
};

struct room rooms[6];
struct room rrooms[6];

char names[10][10] = {{"one"}, {"two"},{"three"}, {"four"}, {"five"}, {"six"}, {"seven"}, {"eight"}, {"nine"}, {"ten"}};

int main() 
{
	//seed random number
	srand(time(NULL));

	char room_path[20];		//Room path
	sprintf(room_path, "malickc.rooms.%i", getpid());	//Create room directory path
	mkdir(room_path, 0777);		//Create directory

	assign_names();		//Assign names from "Names array"

	generate_connections();		//Generate random connections between rooms
	
	assign_types();		//Assign room types

	gen_files(room_path);	//generate files from room structs array "rooms"

	read_rooms(room_path);		//read files into new structs array "rrooms"

	game_loop();	//game loop
	
	return 0;	//exit 0
}

/**************************************************
 *	Function: get_start_rooms()
 *	Purpose: finds the room with type "START_ROOM"
 *	Return: room index
 **************************************************/
int get_start_room() {
	int i = 0;
	for(i = 0; i < 7; i++) {
		if(!strcmp(rrooms[i].type, "START_ROOM")){
			return i;	
		}
	}
	exit(1);
}


/**************************************************
 *	Function: find_room()
 *	Purpose: finds the room from given name,
 *	Return: room index
 **************************************************/
int find_room(char* name) {
	int i = 0;
	for(i = 0; i < 7; i++) {
		if(!strcmp(rrooms[i].name, name)) {
			return i;
		}
	}
	return 100;
}

/**************************************************
 *	Function: game_loop()
 *	Purpose: runs the game loop after components
 *		have been built
 *	Return: --
 **************************************************/
void game_loop() {
	printf("Welcome to the game! \n");
	int game = 0;
	int c_room = get_start_room();
	char input[100];
	int steps = 0;
	char vic_path[1000]="";
	
	//start game
	while(game == 0) {
		printf("CURRENT LOCATION: %s\n", rrooms[c_room].name);
		printf("POSSIBLE LOCATIONS: ");
		
		int i = 0;
		for(i = 0; i < 6; i++) {
			if(rrooms[c_room].connections[i][0]) {
				printf("%s ", rrooms[c_room].connections[i]);
			}
		}
		printf("\n");
				
		int move = 0;
		
		//move loop
		while(move == 0) {
			printf("WHERE TO? >");

			gets(input);

			printf("\n");

			int room = 10;

			for(i = 0; i < 6; i++) {
				if(!strcmp(input, rrooms[c_room].connections[i])) {
					room = find_room(&input);
					break;
				}
			}
			if(room < 8 && room >= 0) {
				c_room = room;
				move = 1;
			} else {
				if(room > 6) {
					printf("HUH? I DON'T UNDERSTAND THAT ROOM. TRY AGAIN\n\n");
				}
			}
		}
		
		steps++;
		strcat(vic_path, rrooms[c_room].name);
		strcat(vic_path, "\n");

		//check for end of game
		if(!strcmp(rrooms[c_room].type, "END_ROOM")) {
			printf("YOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\n");
			printf("YOU TOOK %i STEPS. YOUR PATH TO VICTORY WAS:\n", steps);
			printf("%s\n", vic_path);
			game = 1;
		}
	}
}

/**************************************************
 *	Function: assign_names()
 *	Purpose: assigns seven random names from the list
 *		of ten
 *	Return: --
 **************************************************/
void assign_names() {
	int i, j, k, match;
	int random = 0;
	int used_names[10];

	char temp[20];

	//output some file names
	for( i = 0; i < 7; i++ )
	{
		//Random number for random name
		while(random < 10) 
		{
			match = 0;
			random = rand() % (sizeof(names[0]) / sizeof(names[0][0]));
			for( k = 0; k < (sizeof(used_names))/sizeof(used_names[0]); k++) 
			{
				if(used_names[k] == random) {
					match = 1;	
				}
			}
			if ( match == 0 ) {
				used_names[i] = random;
				break;
			}
		}
		strcpy(rooms[i].name, names[random]);
	}
}

/**************************************************
 *	Function: generate_connections()
 *	Purpose: uses an adjecency matrix to mark connections
 *		and build the connections for each room based
 *		off the matrix:
 *	Return: --
 **************************************************/
void generate_connections() {

	//adjacency matrix
	int c_array[7][7] = {0};
	
	int i, j;
	int connections = 0;
	int con = 0;

	//for each room
	for(i = 0; i < 7; i++) {
		//seed random
		srand(time(NULL)+i);
		//roll number of connections
		connections = rand() % 4 + 3;
		//check for existing connections, if any, decrement
		for(j = 0; j < 7; j++) {
			if(c_array[i][j]) {
				connections--;
			}
		}

		//for each connection
		for(j = 0; j < connections; j++) {
			//generate connection
			con = rand() % 7;
			//check to make sure it isn't connecting to itself and a connection
			//doesn't already exist, if it does, re-roll
			while(i == con && !c_array[i][j]) {
				con = rand() % 7;
			}	
			
			//create connection in room and room connecting to
			c_array[i][con] = 1;
			c_array[con][i] = 1;
		}
	}

	//Actuall write connections to rooms struct array
	for(i = 0; i < 7; i++) {
		for(j = 0; j < 7; j++) {
			if(c_array[i][j]) {
				char con_string[100];
				strcpy(con_string, "CONNECTION ");
				char tmp[10];
				sprintf(tmp,"%i", j);
				strcat(con_string, tmp);
				strcat(con_string, ": ");
				strcat(con_string, rooms[j].name);
				strcpy(rooms[i].connections[rooms[i].count], con_string);
				rooms[i].count++;

			}	
		}
	}
}

/**************************************************
 *	Function: assign_types
 *	Purpose: Assigns rooms types, randomly for Start
 *		and End rooms, then fills with Middle rooms
 *	Return: --
 **************************************************/
void assign_types() {

	int i = 0;
	int rand = random() % 7;
	int rand2 = 0;

	strcpy(rooms[rand].type, "ROOM TYPE: START_ROOM");

	while(rand == rand2) {
		rand2 = random() % 7;
	}

	strcpy(rooms[rand2].type, "ROOM TYPE: END_ROOM");

	for(i = 0; i < 7; i++) {
		if(i != rand && i != rand2) {
			strcpy(rooms[i].type, "ROOM TYPE: MID_ROOM");
		}	
	}
}

/**************************************************
 *	Function: gen_files(char* dir)
 *	Purpose: generates files for game after the room
 *		structs have been completely filled out 
 *	Return: --
 **************************************************/
void gen_files(char* dir) {
	
	int r_i = 0;
	strcat(dir, "/");

	char file_path[30];
	strcpy(file_path, dir);

	for(r_i = 0; r_i < 7; r_i++) {

		int i = 0;
		strcpy(file_path, dir);
		strcat(file_path, rooms[r_i].name);

		FILE *f = fopen( file_path, "w");

		fputs("ROOM NAME: ", f);
		fputs(rooms[r_i].name, f);
		fputs("\n", f);

		for(i = 0; i < 6; i++) {
			if(rooms[r_i].connections[i][0]) {
				fputs(rooms[r_i].connections[i], f);
				fputs("\n", f);

			}
		}

		fputs(rooms[r_i].type, f);
		fclose(f);
		
	}

}

/**************************************************
 *	Function: read_rooms(char* dir) 
 *	Purpose: Reads room files and fills "rrooms" struct
 *		array with data
 *	Return: --
 **************************************************/

void read_rooms(char* dir) {
	int i = 0;

	for(i = 0; i < 7; i++) {
		FILE* in;
		char path[30];
		strcpy(path, dir);
		strcat(path, rooms[i].name);

		char line[30];

		in = fopen(path, "r");
		if(in == NULL) {
			exit(1);
		}
	
		char command[70] = "grep ";
		strcat(command, "NAME ");
		strcat(command, path);
		strcat(command, " | cut -f3 -d' ' | tr -d '\n'");
		in = popen(command, "r");

		while(fgets(line, sizeof(line), in)) {
			strcpy(rrooms[i].name, line);
		}
		
		fclose(in);

		int j = 0;
		char tmp[10];
		for(j = 0; j < 6; j++) {
			
			memset(command, 0, sizeof(command));
			sprintf(tmp,"%i", j);

			strcpy(command, "grep ");
			strcat(command, "'CONNECTION ");
			strcat(command, tmp);
			
			strcat(command, "' ");
			strcat(command, path);
			strcat(command, " | cut -f3 -d' ' | tr -d '\n'");
		
			in = popen(command, "r");

			while(fgets(line, sizeof(line), in)) {
				strcpy(rrooms[i].connections[j], line);
			}
			fclose(in);
		}

		memset(command, 0, sizeof(command));
		
		strcpy(command, "grep ");
		strcat(command, "TYPE ");
		strcat(command, path);
		strcat(command, " | cut -f3 -d' ' | tr -d '\n'");

		in = popen(command, "r");

		while(fgets(line, sizeof(line), in)) {
			strcpy(rrooms[i].type, line);
		}
		fclose(in);
	}
}

