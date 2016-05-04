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
#include <queue>
using std::vector;
using std::tuple;
using std::get;
using std::queue;

typedef vector<tuple<int, int>> p_moves;
struct node {
	node *parent;
	tuple<int, int> origin;
	OthelloBoard* b;
	vector<node*> children;
	p_moves moves;
	bool minmax;
	int score;
};

typedef queue <struct node*> tree_queue;

MinimaxPlayer::MinimaxPlayer(char symb) :
		Player(symb) {

}

MinimaxPlayer::~MinimaxPlayer() {

}



void print_vec(tuple<int,int> moves) {
	printf("(%i,%i)\n", std::get<0>(moves), std::get<1>(moves));
}

p_moves find_possible_moves(OthelloBoard* b, char symbol) {
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
	//std::cout << "wohoo" << std::endl;
	//debug
	//std::for_each(moves.begin(), moves.end(), print_vec);
	return moves;
}

node* create_node(OthelloBoard *b, node *parent, char symbol) {
	struct node *result = new node;
	if(parent == NULL) {
		result->parent = NULL;
		result->minmax = true;
	} else {
		result->parent = parent;
		result->minmax = !parent->minmax;
	}
	result->moves = find_possible_moves(b, symbol);
	return result;
}

void generate_successors(tree_queue *tree, node *next, char symbol) {
		node *successor;
		for(int i = 0; i < next->moves.size(); i++) {
			// Create a copy of the game board
			OthelloBoard *nb = new OthelloBoard(*next->b);
			//Play the new move
			nb->play_move(get<0>(next->moves[i]), get<1>(next->moves[i]), symbol);
			// Create sucessor with new board state
			successor = create_node(nb, next, symbol);
			// Set move that created this state
			successor->origin = tuple<int,int>(get<0>(next->moves[i]), get<1>(next->moves[i]));
			// Set successor as child node of parent
			successor->parent->children.push_back(successor);
			successor->b = nb;
			tree->push(successor);
		}
}

vector<node*>* evaluate_scores(vector<node*> start_set) {
	vector<node*> *next_set;
	node *eval = new node;

	for(int i = 0; i < start_set.size(); i++) {
		eval = start_set[i]->parent;
		// check behavior of player
		if(eval->minmax) {
			// Grab the best outcome for the max player
			for(int j = 0; j < eval->children.size(); j++) {
				if(eval->children[j]->score > eval->score) {
					eval->score = eval->children[j]->score;
				}
			}
		} else {
			// Grab the best outcome for the min player
			for(int j = 0; j < eval->children.size(); j++) {
				if(eval->children[j]->score < eval->score) {
					eval->score = eval->children[j]->score;
				}
			}
		}
		// add this node to the next set to be evaluated
		next_set->push_back(eval);
	}
	return next_set;
}

node* minimax_func(vector<struct node*> leaf_nodes, node root) {
		// For each node in the leaf_nodes vector
		vector<node*> next_set0 = leaf_nodes;
		vector<node*> next_set1;
		vector<node*> root_children;

		while(true) {
				// For each round of parents, set scores from children
				next_set1 = evaluate_scores(next_set0);
				for(int i = 0; i < next_set1.size(); i++) {
					if(next_set1[i]->parent == root) {
						root_children.push_back(next_set1[i]);
						next_set1.erase(i);
					}
				}
				// If we only have root nodes left, break
				if(next_set1.size() == 0) {
					break;
				}

				// reset for next loop
				next_set0 = next_set1;

				// Set next_set1 to empty
				for(int i = 0; i < next_set1.size(); i++) {
					next_set1.erase(i);
				}
		}

		node* return_node = new node;
		return_node->score = -500;

		for(int i = 0; i < root_children.size(); i++) {
			if(root_children[i]->score > return_node->score) {
				return_node = root_children[i];
			}
		}
		return return_node;
}
void MinimaxPlayer::get_move(OthelloBoard* b, int& col, int& row) {
		// Create tree queue
		tree_queue *tree = new tree_queue;

		// Create leaf vector
		vector<struct node*> leaf_nodes;

		// Get player 2 symbol
		char symbol = b->get_p2_symbol();

		// Create root node
		struct node *root = create_node(b, NULL, symbol);
		// Set board condition for this node
		root->b = b;
		// Set minmax status
		root->minmax = true;

		//Add root node to queue
		tree->push(root);

		struct node *next, *successor;
		// For each possible move, search for leaf nodes
		while(true) {
			next = tree->front();
			// If the tree is empty, exit the loop
			if(!next) {
				break;
			}

			if(next->minmax) {
				if(!next->b->has_legal_moves_remaining(symbol)) {
					// Calculate score, set value of leaf as an absolute value from X->score
					next->score = next->b->count_score(symbol);
					next->score = next->score - next->b->count_score('X');
					leaf_nodes.push_back(next);
				} else {
					// Generate Successors and add them to the queue
					generate_successors(tree, next, symbol);
				}


			} else {
				if(!next->b->has_legal_moves_remaining('X')) {
					// Calculate score, set value of leaf as an absolute value from Y->score
					// multiplied by -1 to get the algorithm to avoid X winning
					next->score = next->b->count_score('X');
					next->score = next->score - next->b->count_score(symbol) * -1;
					leaf_nodes.push_back(next);
				} else {
					// Generate Successors and add them to the queue
					generate_successors(tree, next, 'X');
				}
			}

			//remove current node
		  tree->pop();
		}

		// for(int i = 0; i < leaf_nodes.size(); i++) {
		// 		for_each(leaf_nodes[i]->moves.begin(), leaf_nodes[i]->moves.end(), print_vec);
		// }

		node *best_node = new node;
		best_node->score = -500;

		//return up the tree
		best_node = minimax_func(leaf_nodes, root);

		// for(int i = 0; i < leaf_nodes.size(); i++) {
		// 	if(leaf_nodes[i]->score >= best_node->score) {
		// 		best_node = leaf_nodes[i];
		// 	}
		// }

		while(best_node->parent != root && best_node->parent != NULL) {
			best_node = best_node->parent;
		}

		// Set the move as the origin, the move that created the node.
		tuple<int,int> selection = best_node->origin;
		printf("(%i,%i)\n", get<0>(selection), get<1>(selection));

		// Set the move value, no return
		col = get<0>(selection);
		row = get<1>(selection);
		//free(moves);
}

MinimaxPlayer* MinimaxPlayer::clone() {
	MinimaxPlayer* result = new MinimaxPlayer(symbol);
	return result;
}
