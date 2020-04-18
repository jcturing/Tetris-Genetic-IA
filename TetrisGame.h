#ifndef TETRISGAME_H
#define TETRISGAME_H

#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

#include <vector>

#include "Matriz.h"
#include "BoolEngine\BoolEngine.h"


const float INCREMENT_SPEED = 2;
const float INITIAL_SPEED = 1;
const int INCREMENT_LEVEL = 1;
const int LINE_SCORE = 10;
const int TABLE_WIDTH = 10;
const int TABLE_HEIGHT = 18;

enum GameState {READY, RUNNING, PAUSE, RESET, GAME_OVER };
enum GameObject { EMPTY, O, I, S, Z, L, J, T };
enum GameDirection { UP, LEFT, DOWN, RIGHT, BOTTOM};

struct GamePiece {
	std::vector<std::pair<int, int>> position; //The position of each block of the piece: (row, column). The position vector will always be ordered : from up to down and then from left to right
	GameObject type; //The type of the piece
	bool placed = false; //True if the piece has been placed
	int state; //The rotation: 0 if the piece isnt rotated, 1 rotated once, 2 rotated twice, 3...
	/* Here we describe the state 0 for each piece, so you can figure out what each rotation is. Bare in mind that rotations are clockwise!
	O -> No rotations
	I -> Horizontal _
	S -> No rotations
	Z -> No rotations
	L -> i-
	J -> ¬
	T -> No rotations
	*/

	//Used for the IA
	float score = 0;
	int height = 0;
	int cost = 0;
	int depth = 0;
	std::vector<GameDirection> previous_moves;

	bool operator< (GamePiece const& p2) const{
		return position < p2.position;
	}

	bool operator== (GamePiece const& p2) const {
		return position == p2.position;
	}

};

class TetrisGame {

public:
	TetrisGame();
	//Executes the tetris game. If the game is running, move piece and if has been placed: check lines and check game over and generate new piece.
	//If the game is reset, restart all the atributes to their standards
	void execute(std::vector<bool> &keys, bool refresh_key, bool bot);
	
	//Generate randomly a new piece on the top of the board
	void generateNewPiece();

	//Move if possible the piece to the direction specified
	void movePiece(GameDirection direction, GamePiece &piece_to_move, Matriz<GameObject> &table_to_move);

	//Check if there is a collition in order to move correctly
	bool isCollision(GameDirection direction, GamePiece const& piece_to_move, Matriz<GameObject> const& table_to_check);

	//Setters
	void setState(GameState newState) { state = newState; }
	void setNewTimer(bool option) { newTimer = option; }
	void setPlayerName(std::string newName);
	void setPiecePlaced(bool new_piece_placed) { piece_placed = new_piece_placed; }

	//Getters
	GameState getState() const { return state; }
	std::string getPlayerName() const { return player_name; }
	Matriz<GameObject> getTable() { return table; }
	GameObject getTablePos(int i, int j) { return table[i][j]; }
	float getSpeed() const { return speed; }
	int getLevel() const { return level; }
	int getLines() const { return lines; }
	long long int getScore() const { return score; }
	bool getNewTimer() const { return newTimer; }
	GamePiece getPiece() const { return piece; }
	bool getPiecePlaced() const { return piece_placed; }

private:

	//Rotate if possible the piece
	void rotatePiece(GamePiece &piece_to_move, Matriz<GameObject> &table_to_move);
	
	//Check if some line has been fulled and updates scores, lines and level if necessary
	void checkLine();
	
	//Check if the player has lost
	void checkGameOver();
	
	//Scroll the board down when one line has been fulled
	void moveTableDown(int t);
	
	long long int score; //The game score
	std::string player_name; //The player name
	GameState state; //The state of the game
	Matriz<GameObject> table; //The board with all the pieces inside
	GamePiece piece; //The piece the player is controlling
	int level, lines; //Level and lines of the game
	float speed; //Speed of the game
	bool newTimer; //Tells the UI to speed up
	bool piece_placed; //Tells whether the piece of the player has been placed and game continues: Used for the IA
};


#endif // !TETRISGAME_H
