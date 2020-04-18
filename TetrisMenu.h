#ifndef TETRISMENU_H
#define TETRISMENU_H

#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <sstream>


#include "BoolEngine\BoolEngine.h"

const int SCOREBOARD_DIM = 9;
const std::string SCOREBOARD_NAME = "players.dat";
const int MAX_NAME_LENGHT = 15;

enum MenuState {OFF, ON };

class TetrisMenu {
public:
	//Executes the menu. Registers the player name
	void execute(BoolEngine &engine, std::vector<bool> &keys, bool refresh_key);
	
	//Loads the score board from the data file
	void loadScoreBoard();
	
	//Saves the updated score board to the data file
	void saveScoreBoard();
	
	//Updates the score board with the new pair p
	void updateScoreBoard(std::pair<std::string, std::string> const& p);

	//Setters
	void setState(MenuState newState) { state = newState; }
	void setIntroduceName(std::string newName) { introduce_name = newName; }

	//Getters
	MenuState getState() const { return state; }
	std::vector<std::pair<std::string, std::string>> getScoreBoard() { return score_board; }
	std::string getIntroduceName() const { return introduce_name; }

private: 
	std::vector<std::pair<std::string, std::string>> score_board; //The score board
	MenuState state; //The state of the menu
	std::string introduce_name; //The name being introduced
};


#endif // !TETRISMENU_H
