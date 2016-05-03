/*
 * MinimaxPlayer.cpp
 *
 *  Created on: Apr 17, 2015
 *      Author: wong
 */
#include <iostream>
#include <assert.h>
#include "MinimaxPlayer.h"
#include <tuple>
#include <algorithm>
using std::vector;
using std::tuple;
using std::get;

typedef vector<tuple<int, int>> p_moves;

MinimaxPlayer::MinimaxPlayer(char symb) :
		Player(symb) {

}

MinimaxPlayer::~MinimaxPlayer() {

}

void print_vec(tuple<int,int> moves) {
	printf("(%i,%i)\n", std::get<0>(moves), std::get<1>(moves));
}

p_moves find_possible_moves(OthelloBoard* b, char symbol) {
	//p_moves *moves = (p_moves *)malloc(sizeof(p_moves));
    p_moves moves;
	// Collect possible moves
	int index = 0;
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			if(b->is_legal_move(i,j,symbol)) {
			    moves.push_back(tuple<int,int>(i,j));
			}
		}
	}
	std::cout << "wohoo" << std::endl;
	//debug
	std::for_each(moves.begin(), moves.end(), print_vec);
	return moves;
}

void MinimaxPlayer::get_move(OthelloBoard* b, int& col, int& row) {
		char symbol = b->get_p2_symbol();
		p_moves moves = find_possible_moves(b, symbol);

		// For each possible move, calculate the utility

		// Set the move value, no return
		tuple<int,int> selection = moves.front();
		col = get<0>(selection);
		row = get<1>(selection);
		//free(moves);
}

MinimaxPlayer* MinimaxPlayer::clone() {
	MinimaxPlayer* result = new MinimaxPlayer(symbol);
	return result;
}
