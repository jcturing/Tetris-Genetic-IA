#ifndef TETRISHEDONISTIC_H
#define TETRISHEDONISTIC_H

/* IA for the Tetris Game
-> 1º Version: Hedonistic with genetic algorithm.

*/

#include <vector>
#include <algorithm>
#include <deque>
#include <set>
#include <queue>
#include "TetrisGame.h"

class TetrisHedonisticIA {

public:
	TetrisHedonisticIA();
	TetrisHedonisticIA(float height_w, float clear_w, float hole_w, float blockade_w);

	void calculateDestination(Matriz<GameObject> const& table, GamePiece const& piece);
	GameDirection getNextMovement(Matriz<GameObject> const& table, GamePiece const& piece);

	//Setters
	void setHeightWeight(float new_height_weight) { HEIGHT_WEIGHT = new_height_weight; }
	void setHoleWeight(float new_hole_weight) { HOLE_WEIGHT = new_hole_weight; }
	void setBlockadeWeight(float new_blockade_weight) { BLOCKADE_WEIGHT = new_blockade_weight; }
	void setClearsWeight(float new_clears_weight) { CLEARS_WEIGHT = new_clears_weight; }

	//Getters
	float getHeightWeight() const { return HEIGHT_WEIGHT; }
	float getHoleWeight() const { return HOLE_WEIGHT; }
	float getBlockadeWeight() const { return BLOCKADE_WEIGHT; }
	float getClearsWeight() const { return CLEARS_WEIGHT; }

private:

	//DESTINATION PRIVATE METHODS
	
	//Given a table and the piece to be moved, calculates all the possible places that the piece can be moved to
	std::vector<GamePiece> getPossibleDestinations(Matriz<GameObject> const& table, GamePiece const& piece);
	
	//Calculate all the factor that determines which destination will be chosen
	void calculateFactors(Matriz<GameObject> const& table, float &heights, float &clears, float &holes, float &blockades);
	
	//Order the destination vector, giving the first place for the destination with highest score
	void orderPossibleDestinations(Matriz<GameObject> const& table, std::vector<GamePiece> & possible_destinations, GamePiece const& piece);

	//Given a node, returns reachable neighbours
	std::vector<GamePiece> nodeNeighbours(Matriz<GameObject> const& table, GamePiece const& node);

	//Calculate de real cost between the node passed and the destination
	int calculateCost(GamePiece const& node, GamePiece const& destination);

	//Given a destination, makes sure if it is reachable through breadth first search -> Complete (Always finds a solution)
	bool isReachableBFS(Matriz<GameObject> const& table, GamePiece const& destination, GamePiece const& piece);

	//Given a destination, makes sure if it is reachable through astar search
	//bool isReachableAstar(Matriz<GameObject> const& table, GamePiece const& destination, GamePiece const& piece);

	//Among all destinations, choose the one with the highest score THAT IS REACHABLE
	GamePiece chooseDestination(Matriz<GameObject> const& table, std::vector<GamePiece> const& possible_destinations, GamePiece const& piece);

	//Weights -> should be constants. These constants determine the "brain" of the AI. The genetic algorithm will play with these.
	float HEIGHT_WEIGHT, CLEARS_WEIGHT, HOLE_WEIGHT, BLOCKADE_WEIGHT;
	
	//Vector of directions to move the piece
	GamePiece final_destination;
	std::vector<GameDirection> route_to_destination;
	int route_counter;

};

#endif