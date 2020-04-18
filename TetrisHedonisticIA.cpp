#include "TetrisHedonisticIA.h"


TetrisHedonisticIA::TetrisHedonisticIA() {
	//Default Values (Internet based)
	HEIGHT_WEIGHT = 3.78f;
	CLEARS_WEIGHT = 1.6;
	HOLE_WEIGHT = 2.31;
	BLOCKADE_WEIGHT = 0.59;
	route_counter = 0;
}

TetrisHedonisticIA::TetrisHedonisticIA(float height_w, float clear_w, float hole_w, float blockade_w) {
	HEIGHT_WEIGHT = height_w;
	CLEARS_WEIGHT = clear_w;
	HOLE_WEIGHT = hole_w;
	BLOCKADE_WEIGHT = blockade_w;
	route_counter = 0;
}

void TetrisHedonisticIA::calculateDestination(Matriz<GameObject> const& table, GamePiece const& piece) {
	std::vector<GamePiece> possible_destinations = getPossibleDestinations(table, piece);
	orderPossibleDestinations(table, possible_destinations, piece);
	final_destination = chooseDestination(table, possible_destinations, piece);
	route_counter = 0;
}

std::vector<GamePiece> TetrisHedonisticIA::getPossibleDestinations(Matriz<GameObject> const& table, GamePiece const& piece){
	std::vector<GamePiece> possible_destinations;
	GamePiece destination;
	destination.type = piece.type;

	if (piece.type == I) {
		//No rotated
		destination.state = 0;
		for (int i = TABLE_HEIGHT - 1; i >= 3; i--) {
			for (int j = 0; j < TABLE_WIDTH - 3; j++) {
				if (i == TABLE_HEIGHT - 1 || table[i+1][j] != EMPTY || table[i+1][j+1] != EMPTY || table[i+1][j+2] != EMPTY || table[i+1][j+3] != EMPTY) { //If it is on the last row (floor) or has a block below
					if (table[i][j] == EMPTY && table[i][j + 1] == EMPTY && table[i][j + 2] == EMPTY && table[i][j + 3] == EMPTY) { //If there is space -> ADD
						destination.position.push_back({ i, j });
						destination.position.push_back({ i, j+1 });
						destination.position.push_back({ i, j+2 });
						destination.position.push_back({ i, j+3 });
						possible_destinations.push_back(destination);
						destination.position.clear();
					}
				}
			}
		}
		//Rotated once
		destination.state = 1;
		for (int i = TABLE_HEIGHT - 1; i >= 3; i--) {
			for (int j = 0; j < TABLE_WIDTH; j++) {
				if (i == TABLE_HEIGHT - 1 || (table[i + 1][j] != EMPTY)) {
					if (table[i][j] == EMPTY && table[i-1][j] == EMPTY && table[i-2][j] == EMPTY && table[i-3][j] == EMPTY) {
						destination.position.push_back({ i - 3, j });
						destination.position.push_back({ i - 2, j });
						destination.position.push_back({ i - 1, j });
						destination.position.push_back({ i, j});
						possible_destinations.push_back(destination);
						destination.position.clear();
					}
				}
			}
		}
	}
	else if (piece.type == O) {
		//No rotated
		destination.state = 0;
		for (int i = TABLE_HEIGHT - 1; i >= 3; i--) {
			for (int j = 0; j < TABLE_WIDTH - 1; j++) {
				if ((i == TABLE_HEIGHT - 1 || table[i + 1][j] != EMPTY || table[i+1][j+1])) { //If it is on the last row (floor) or has a block below
					if (table[i][j] == EMPTY && table[i][j + 1] == EMPTY && table[i-1][j] == EMPTY && table[i-1][j + 1] == EMPTY) { //If there is space -> ADD
						destination.position.push_back({ i-1, j });
						destination.position.push_back({ i-1, j + 1 });
						destination.position.push_back({ i, j });
						destination.position.push_back({ i, j + 1 });
						possible_destinations.push_back(destination);
						destination.position.clear();
					}
				}
			}
		}
	}
	else if (piece.type == S) {
		//No rotated
		destination.state = 0;
		for (int i = TABLE_HEIGHT - 1; i >= 3; i--) {
			for (int j = 0; j < TABLE_WIDTH-2; j++) {
				if (i == TABLE_HEIGHT - 1 || table[i + 1][j] != EMPTY || table[i+1][j+1] != EMPTY || table[i][j+2] != EMPTY) { //If it is on the last row (floor) or has a block below
					if (table[i][j] == EMPTY && table[i][j + 1] == EMPTY && table[i-1][j + 1] == EMPTY && table[i-1][j + 2] == EMPTY) { //If there is space -> ADD
				//		std::cout << "Posible destino en: " << i << " " << j << std::endl;
						destination.position.push_back({ i - 1, j + 1 });
						destination.position.push_back({ i - 1, j + 2 });
						destination.position.push_back({ i, j });
						destination.position.push_back({ i, j + 1 });
						possible_destinations.push_back(destination);
						destination.position.clear();
					}
				}
			}
		}
	//	std::cout << std::endl << std::endl;

		//Rotated once
		destination.state = 1;
		for (int i = TABLE_HEIGHT - 1; i >= 3; i--) {
			for (int j = 1; j < TABLE_WIDTH; j++) {
				if ((i == TABLE_HEIGHT - 1 || table[i + 1][j] != EMPTY || table[i][j-1] != EMPTY)) { //If it is on the last row (floor) or has a block below
					if (table[i][j] == EMPTY && table[i-1][j - 1] == EMPTY && table[i - 1][j] == EMPTY && table[i - 2][j - 1] == EMPTY) { //If there is space -> ADD
					//	std::cout << "Posible destino en: " << i << " " << j << std::endl;
						destination.position.push_back({ i - 2, j - 1 });
						destination.position.push_back({ i - 1, j - 1 });
						destination.position.push_back({ i - 1, j });
						destination.position.push_back({ i, j });
						possible_destinations.push_back(destination);
						destination.position.clear();
					}
				}
			}
		}
	}
	else if (piece.type == Z) {
		//No rotated
		destination.state = 0;
		for (int i = TABLE_HEIGHT - 1; i >= 3; i--) {
			for (int j = 1; j < TABLE_WIDTH - 1; j++) {
				if (i == TABLE_HEIGHT - 1 || table[i + 1][j] != EMPTY || table[i+1][j+1] != EMPTY || table[i][j-1] != EMPTY) { //If it is on the last row (floor) or has a block below
					if (table[i][j] == EMPTY && table[i][j + 1] == EMPTY && table[i - 1][j] == EMPTY && table[i - 1][j - 1] == EMPTY) { //If there is space -> ADD
						destination.position.push_back({ i - 1, j - 1 });
						destination.position.push_back({ i - 1, j });
						destination.position.push_back({ i, j });
						destination.position.push_back({ i, j + 1 });
						possible_destinations.push_back(destination);
						destination.position.clear();
					}
				}
			}
		}

		//Rotated once
		destination.state = 1;
		for (int i = TABLE_HEIGHT - 1; i >= 3; i--) {
			for (int j = 0; j < TABLE_WIDTH-1; j++) {
				if ((i == TABLE_HEIGHT - 1 || table[i + 1][j] != EMPTY || table[i][j+1] != EMPTY)) { //If it is on the last row (floor) or has a block below
					if (table[i][j] == EMPTY && table[i-1][j] == EMPTY && table[i - 1][j + 1] == EMPTY && table[i - 2][j + 1] == EMPTY) { //If there is space -> ADD
						destination.position.push_back({ i - 2, j + 1 });
						destination.position.push_back({ i - 1, j });
						destination.position.push_back({ i - 1, j + 1});
						destination.position.push_back({ i, j});
						possible_destinations.push_back(destination);
						destination.position.clear();
					}
				}
			}
		}
	}
	else if (piece.type == L) {
		//No rotated
		destination.state = 0;
		for (int i = TABLE_HEIGHT - 1; i >= 3; i--) {
			for (int j = 0; j < TABLE_WIDTH-2; j++) {
				if ((i == TABLE_HEIGHT - 1 || table[i + 1][j] != EMPTY || table[i][j+1] != EMPTY || table[i][j+2] != EMPTY)) { //If it is on the last row (floor) or has a block below
					if (table[i][j] == EMPTY && table[i-1][j] == EMPTY && table[i-1][j + 1] == EMPTY && table[i-1][j + 2] == EMPTY) { //If there is space -> ADD
						destination.position.push_back({ i-1, j });
						destination.position.push_back({ i-1, j + 1 });
						destination.position.push_back({ i-1, j + 2 });
						destination.position.push_back({ i, j});
						possible_destinations.push_back(destination);
						destination.position.clear();
					}
				}
			}
		}

		//Rotated once
		destination.state = 1;
		for (int i = TABLE_HEIGHT - 1; i >= 3; i--) {
			for (int j = 1; j < TABLE_WIDTH; j++) {
				if ((i == TABLE_HEIGHT - 1 || table[i + 1][j] != EMPTY || table[i-1][j-1] != EMPTY)) { //If it is on the last row (floor) or has a block below
					if (table[i][j] == EMPTY && table[i-1][j] == EMPTY && table[i-2][j] == EMPTY && table[i-2][j - 1] == EMPTY) { //If there is space -> ADD
						destination.position.push_back({ i-2, j-1 });
						destination.position.push_back({ i-2, j });
						destination.position.push_back({ i-1, j });
						destination.position.push_back({ i, j });
						possible_destinations.push_back(destination);
						destination.position.clear();
					}
				}
			}
		}

		//Rotated twice
		destination.state = 2;
		for (int i = TABLE_HEIGHT - 1; i >= 3; i--) {
			for (int j = 0; j < TABLE_WIDTH-2; j++) {
				if ((i == TABLE_HEIGHT - 1 || table[i + 1][j] != EMPTY || table[i+1][j+1] != EMPTY || table[i+1][j+2] != EMPTY)) { //If it is on the last row (floor) or has a block below
					if (table[i][j] == EMPTY && table[i][j + 1] == EMPTY && table[i][j + 2] == EMPTY && table[i-1][j + 2] == EMPTY) { //If there is space -> ADD
						destination.position.push_back({ i-1, j+2 });
						destination.position.push_back({ i, j });
						destination.position.push_back({ i, j + 1 });
						destination.position.push_back({ i, j + 2 });
						possible_destinations.push_back(destination);
						destination.position.clear();
					}
				}
			}
		}

		//Rotated thrice
		destination.state = 3;
		for (int i = TABLE_HEIGHT - 1; i >= 3; i--) {
			for (int j = 0; j < TABLE_WIDTH - 1; j++) {
				if ((i == TABLE_HEIGHT - 1 || table[i + 1][j] != EMPTY || table[i+1][j+1] != EMPTY)) { //If it is on the last row (floor) or has a block below
					if (table[i][j] == EMPTY && table[i][j + 1] == EMPTY && table[i-1][j] == EMPTY && table[i-2][j] == EMPTY) { //If there is space -> ADD
						destination.position.push_back({ i-2, j });
						destination.position.push_back({ i-1, j });
						destination.position.push_back({ i, j });
						destination.position.push_back({ i, j + 1 });
						possible_destinations.push_back(destination);
						destination.position.clear();
					}
				}
			}
		}
	}
	else if (piece.type == J) {
		//No rotated
		destination.state = 0;
		for (int i = TABLE_HEIGHT - 1; i >= 3; i--) {
			for (int j = 2; j < TABLE_WIDTH; j++) {
				if ((i == TABLE_HEIGHT - 1 || table[i + 1][j] != EMPTY || table[i][j-1] != EMPTY || table[i][j-2] != EMPTY)) { //If it is on the last row (floor) or has a block below
					if (table[i][j] == EMPTY && table[i-1][j] == EMPTY && table[i-1][j - 1] == EMPTY && table[i-1][j - 2] == EMPTY) { //If there is space -> ADD
						destination.position.push_back({ i-1, j - 2 });
						destination.position.push_back({ i-1, j - 1 });
						destination.position.push_back({ i-1, j });
						destination.position.push_back({ i, j });
						possible_destinations.push_back(destination);
						destination.position.clear();
					}
				}
			}
		}

		//Rotated once
		destination.state = 1;
		for (int i = TABLE_HEIGHT - 1; i >= 3; i--) {
			for (int j = 0; j < TABLE_WIDTH-1; j++) {
				if ((i == TABLE_HEIGHT - 1 || table[i + 1][j] != EMPTY || table[i-1][j+1] != EMPTY)) { //If it is on the last row (floor) or has a block below
					if (table[i][j] == EMPTY && table[i][j + 1] == EMPTY && table[i-1][j + 1] == EMPTY && table[i-2][j + 1] == EMPTY) { //If there is space -> ADD
						destination.position.push_back({ i-2, j+1 });
						destination.position.push_back({ i-1, j + 1 });
						destination.position.push_back({ i, j });
						destination.position.push_back({ i, j + 1 });
						possible_destinations.push_back(destination);
						destination.position.clear();
					}
				}
			}
		}

		//Rotated twice
		destination.state = 2;
		for (int i = TABLE_HEIGHT - 1; i >= 3; i--) {
			for (int j = 0; j < TABLE_WIDTH-2; j++) {
				if ((i == TABLE_HEIGHT - 1 || table[i + 1][j] != EMPTY || table[i+1][j+1] != EMPTY || table[i+1][j+2] != EMPTY)) { //If it is on the last row (floor) or has a block below
					if (table[i][j] == EMPTY && table[i][j + 1] == EMPTY && table[i][j + 2] == EMPTY && table[i-1][j] == EMPTY) { //If there is space -> ADD
						destination.position.push_back({ i-1, j });
						destination.position.push_back({ i, j });
						destination.position.push_back({ i, j + 1 });
						destination.position.push_back({ i, j + 2 });
						possible_destinations.push_back(destination);
						destination.position.clear();
					}
				}
			}
		}

		//Rotated thrice
		destination.state = 3;
		for (int i = TABLE_HEIGHT - 1; i >= 3; i--) {
			for (int j = 0; j < TABLE_WIDTH-1; j++) {
				if ((i == TABLE_HEIGHT - 1 || table[i + 1][j] != EMPTY || table[i+1][j+1] != EMPTY)) { //If it is on the last row (floor) or has a block below
					if (table[i][j] == EMPTY && table[i-1][j] == EMPTY && table[i-2][j] == EMPTY && table[i-2][j+1] == EMPTY) { //If there is space -> ADD
						destination.position.push_back({ i-2, j });
						destination.position.push_back({ i-2, j + 1 });
						destination.position.push_back({ i-1, j });
						destination.position.push_back({ i, j });
						possible_destinations.push_back(destination);
						destination.position.clear();
					}
				}
			}
		}
	}
	else if (piece.type == T) {
		//No rotated
		destination.state = 0;
		for (int i = TABLE_HEIGHT - 1; i >= 3; i--) {
			for (int j = 1; j < TABLE_WIDTH-1; j++) {
				if ((i == TABLE_HEIGHT - 1 || table[i + 1][j] != EMPTY || table[i][j-1] != EMPTY || table[i][j+1] != EMPTY)) { //If it is on the last row (floor) or has a block below
					if (table[i][j] == EMPTY && table[i-1][j - 1] == EMPTY && table[i-1][j] == EMPTY && table[i-1][j + 1] == EMPTY) { //If there is space -> ADD
						destination.position.push_back({ i-1, j-1 });
						destination.position.push_back({ i-1, j });
						destination.position.push_back({ i-1, j + 1 });
						destination.position.push_back({ i, j });
						possible_destinations.push_back(destination);
						destination.position.clear();
					}
				}
			}
		}

		//Rotated once
		destination.state = 1;
		for (int i = TABLE_HEIGHT - 1; i >= 3; i--) {
			for (int j = 1; j < TABLE_WIDTH; j++) {
				if ((i == TABLE_HEIGHT - 1 || table[i + 1][j] != EMPTY || table[i][j-1] != EMPTY)) { //If it is on the last row (floor) or has a block below
					if (table[i][j] == EMPTY && table[i-1][j] == EMPTY && table[i-1][j - 1] == EMPTY && table[i-2][j] == EMPTY) { //If there is space -> ADD
						destination.position.push_back({ i-2, j });
						destination.position.push_back({ i-1, j - 1 });
						destination.position.push_back({ i-1, j });
						destination.position.push_back({ i, j });
						possible_destinations.push_back(destination);
						destination.position.clear();
					}
				}
			}
		}

		//Rotated twice
		destination.state = 2;
		for (int i = TABLE_HEIGHT - 1; i >= 3; i--) {
			for (int j = 0; j < TABLE_WIDTH-2; j++) {
				if ((i == TABLE_HEIGHT - 1 || table[i + 1][j] != EMPTY || table[i+1][j+1] != EMPTY || table[i+1][j+2] != EMPTY)) { //If it is on the last row (floor) or has a block below
					if (table[i][j] == EMPTY && table[i][j+1] == EMPTY && table[i][j + 2] == EMPTY && table[i - 1][j+1] == EMPTY) { //If there is space -> ADD
						destination.position.push_back({ i - 1, j+1 });
						destination.position.push_back({ i, j });
						destination.position.push_back({ i, j+1 });
						destination.position.push_back({ i, j+2 });
						possible_destinations.push_back(destination);
						destination.position.clear();
					}
				}
			}
		}

		//Rotated thrice
		destination.state = 3;
		for (int i = TABLE_HEIGHT - 1; i >= 3; i--) {
			for (int j = 0; j < TABLE_WIDTH-1; j++) {
				if ((i == TABLE_HEIGHT - 1 || table[i + 1][j] != EMPTY || table[i][j+1] != EMPTY)) { //If it is on the last row (floor) or has a block below
					if (table[i][j] == EMPTY && table[i - 1][j] == EMPTY && table[i - 1][j +1] == EMPTY && table[i - 2][j] == EMPTY) { //If there is space -> ADD
						destination.position.push_back({ i - 2, j });
						destination.position.push_back({ i - 1, j });
						destination.position.push_back({ i - 1, j + 1 });
						destination.position.push_back({ i, j });
						possible_destinations.push_back(destination);
						destination.position.clear();
					}
				}
			}
		}
	}

	//std::cout << std::endl << std::endl << "ADIOS" << std::endl << std::endl;
	return possible_destinations;
}

void TetrisHedonisticIA::calculateFactors(Matriz<GameObject> const& table, float &heights, float &clears, float &holes, float &blockades) {
	bool is_clear = false, new_height = false, check_height = true;
	std::vector<std::pair<int, int>> holes_position;

	for (int i = TABLE_HEIGHT - 1; i >= 0; i--) {
		is_clear = true;
		new_height = false;
		for (int j = 0; j < TABLE_WIDTH; j++){
			//Check if new height
			if (check_height && table[i][j] != EMPTY) new_height = true;
			
			//Check if the row is a clear
			if (table[i][j] == EMPTY) is_clear = false;
			
			//Check if (i,j) is a hole: it is a hole if is empty and there are any block above
			if (i > 0 && table[i][j] == EMPTY && table[i - 1][j] != EMPTY) {
				holes_position.push_back({i, j});
				holes++;
			}

			//Check if (i,j) is a blockade: it is a blockade if is not empty and there is a hole below
			if (table[i][j] != EMPTY){
				for (int t = 0; t < holes_position.size(); t++) {
					if (i < holes_position[t].first && j == holes_position[t].second) blockades++;
				}
			}
		}
		if (is_clear) clears++;
		if (new_height) heights++;
		else check_height = false;
	}
}

bool orderComparator(GamePiece const& p1, GamePiece const& p2) {
	return p1.score > p2.score;
}

void TetrisHedonisticIA::orderPossibleDestinations(Matriz<GameObject> const& table, std::vector<GamePiece> & possible_destinations, GamePiece const& piece) {
	
	//Calculate score of each possible movement
	for (int i = 0; i < possible_destinations.size(); i++){
		//Get the table with the piece moved (first delete it from the original place and then move it with the possible destination (inside the for))
		Matriz<GameObject> aux_table = table;
		for (int j = 0; j < piece.position.size(); j++) aux_table[piece.position[j].first][piece.position[j].second] = EMPTY;
		for (int j = 0; j < possible_destinations[i].position.size(); j++) aux_table[possible_destinations[i].position[j].first][possible_destinations[i].position[j].second] = possible_destinations[i].type;
		
		//Calculate factors of the score
		float heights = 0, clears = 0, holes = 0, blockades = 0;
		calculateFactors(aux_table, heights, clears, holes, blockades);

		//Calculate score of a single movement
		possible_destinations[i].score = clears * CLEARS_WEIGHT - heights * HEIGHT_WEIGHT - holes * HOLE_WEIGHT - blockades * BLOCKADE_WEIGHT;
		possible_destinations[i].height = heights;
	}
	std::sort(possible_destinations.begin(), possible_destinations.end(), orderComparator);
 }

std::vector<GamePiece> TetrisHedonisticIA::nodeNeighbours(Matriz<GameObject> const& table, GamePiece const& node) {
	std::vector<GamePiece> neighbours;
	TetrisGame aux_game;
	Matriz<GameObject> aux_table = table;
	GamePiece aux_node = node;

	//Down
	if (!aux_game.isCollision(DOWN, aux_node, aux_table)) {
		aux_game.movePiece(DOWN, aux_node, aux_table);
		for (int i = 0; i < aux_node.position.size(); i++) aux_table[aux_node.position[i].first][aux_node.position[i].second] = EMPTY;
		aux_node.previous_moves.push_back(DOWN);
		aux_node.depth++;
		neighbours.push_back(aux_node);
	}

	//Up
	aux_node = node;
	if (!aux_game.isCollision(UP, aux_node, aux_table)) {
		aux_game.movePiece(UP, aux_node, aux_table);
		for (int i = 0; i < aux_node.position.size(); i++) aux_table[aux_node.position[i].first][aux_node.position[i].second] = EMPTY;
		aux_node.previous_moves.push_back(UP);
		aux_node.depth++;
		neighbours.push_back(aux_node);
	}

	//Right
	aux_node = node;
	if (!aux_game.isCollision(RIGHT, aux_node, aux_table)) {
		aux_game.movePiece(RIGHT, aux_node, aux_table);
		for (int i = 0; i < aux_node.position.size(); i++) aux_table[aux_node.position[i].first][aux_node.position[i].second] = EMPTY;
		aux_node.previous_moves.push_back(RIGHT);
		aux_node.depth++;
		neighbours.push_back(aux_node);
	}

	//Left
	aux_node = node;
	if (!aux_game.isCollision(LEFT, aux_node, aux_table)) {
		aux_game.movePiece(LEFT, aux_node, aux_table);
		for (int i = 0; i < aux_node.position.size(); i++) aux_table[aux_node.position[i].first][aux_node.position[i].second] = EMPTY;
		aux_node.previous_moves.push_back(LEFT);
		aux_node.depth++;
		neighbours.push_back(aux_node);
	}

	return neighbours;
}

struct pqCompare {
	bool operator() (GamePiece & p1, GamePiece & p2) {
		return p1.cost > p2.cost;
	}
};

template<class T, class C = vector<T>, class P = less<typename C::value_type> >
struct MyPriorityQueue : std::priority_queue<T, C, P> {
	typename C::iterator begin() { return std::priority_queue<T, C, P>::c.begin(); }
	typename C::iterator end() { return std::priority_queue<T, C, P>::c.end(); }
};

int TetrisHedonisticIA::calculateCost(GamePiece const& node, GamePiece const& destination) {
	int min = 1000, aux_hor = 1000, aux_vert = 1000;
	for (int i = 0; i < node.position.size(); i++) {
		if (abs(node.position[i].first - destination.position[i].first) < aux_hor) aux_hor = abs(node.position[i].first - destination.position[i].first);
		if (abs(node.position[i].second - destination.position[i].second) < aux_vert) aux_vert = abs(node.position[i].second - destination.position[i].second);
	}
	min = aux_hor + aux_vert;
	return min;
}

bool TetrisHedonisticIA::isReachableBFS(Matriz<GameObject> const& table, GamePiece const& destination, GamePiece const& piece) {
	//return true;
	bool reached = false;
	
	//Perform a breadth first search (Finds a solution for sure)
	MyPriorityQueue<GamePiece, std::vector<GamePiece>, pqCompare> frontier; //Open list
	std::set<GamePiece> explored; //Closed list
	TetrisGame aux_game; //Game Aux for the collision and movement operations
	Matriz<GameObject> aux_table = table; //Table aux for the collision and movement operations
	GamePiece node = piece; //Node
	node.height = node.position[node.position.size() - 1].first;
	int counter = 0; //To check how many nodes explored

	for (int i = 0; i < piece.position.size(); i++) aux_table[piece.position[i].first][piece.position[i].second] = EMPTY; //To avoid anoying errors

	//Spped up by going down before searching;
	while (node.height < TABLE_HEIGHT - destination.height - 3) {
		for (int i = 0; i < node.position.size(); i++) {
			node.position[i].first++;
		}
		node.previous_moves.push_back(DOWN);
		node.height = node.position[node.position.size() - 1].first;
	}

	frontier.push(node);
	while (!frontier.empty()) {
		node = frontier.top(); //Get next Node
		frontier.pop();
		
		//Check if goal
		if (node == destination) {
			//std::cout << "Destino Reached tras " << counter << " iteraciones." << std::endl;
			route_to_destination = node.previous_moves;
			return true; 
		}

		explored.insert(node);
		
		//If not goal, insert possible moves
		for each (GamePiece neighbour in nodeNeighbours(aux_table, node)) {
			if (std::find(explored.begin(), explored.end(), neighbour) == explored.end() && std::find(frontier.begin(), frontier.end(), neighbour) == frontier.end()) {
				neighbour.cost = calculateCost(neighbour, destination);
				frontier.push(neighbour);
			}
		}
		counter++;
	}
	//std::cout << "Destino Fallido tras " << counter << " iteraciones." << std::endl;
	return false;
}

/*

bool TetrisHedonisticIA::isReachableAstar(Matriz<GameObject> const& table, GamePiece const& destination, GamePiece const& piece) {
	bool reached = false;

	//Perform a astar search
	MyPriorityQueue<GamePiece, pqCompare> frontier; //For the search
	std::set<GamePiece> explored;
	int counter = 0; //To check how many nodes checked

	frontier.push(piece);
	while (!frontier.empty()) {
		GamePiece node = frontier.top(); //Get next Node
		frontier.pop();

		//If we have reached goal, finish
		if (node == destination) {
			std::cout << "Destino Reached tras " << counter << " iteraciones." << std::endl;
			return true; //Check if goal
		}

		//If not, insert node in explored and insert neighbours in pq if needed
		explored.insert(node);

		for each (GamePiece neighbour in nodeNeighbours(table, node)){
			if (std::find(explored.begin(), explored.end(), neighbour) == explored.end()) {

			}
		}
		counter++;
	}

	return false;
}
*/

GamePiece TetrisHedonisticIA::chooseDestination(Matriz<GameObject> const& table, std::vector<GamePiece> const& possible_destinations, GamePiece const& piece) {
	for (int i = 0; i < possible_destinations.size(); i++) {
		if (isReachableBFS(table, possible_destinations[i], piece)) {
			if (i > 0) {
				//std::cout << "RECTIFICACION TOMADA! -> ";
			}
			//std::cout << "ELEGIDO POSIBLE DESTINO: " << i << std::endl;
			return possible_destinations[i];
		}
	}
	//None reachable destinations, return a random piece;
	if (possible_destinations.size() > 0) return possible_destinations[0];
	else return GamePiece();
}

GameDirection TetrisHedonisticIA::getNextMovement(Matriz<GameObject> const& table, GamePiece const& piece) {
	GameDirection next_move;

	/*
	int left_piece = TABLE_WIDTH;
	int left_destination = TABLE_WIDTH;

	//(This algorithm is not always right, maybe we want to rotate/move it later so if fits, but if we avoid undesired walls and holes it should work nice... LATER IMPROVEMENT NEEDED!)
	//If the piece isnt rotated, rotate it first! 
	
	if (quick_down < 2) { //To avoid delay because of rotations
		next_move = DOWN;
		quick_down++;
	}
	else {
		if (piece.state != final_destination.state) next_move = UP;
		else {
			//Get the leftest coordinate from the piece and the destination
			for (int i = 0; i < piece.position.size(); i++) {
				left_piece = std::min(piece.position[i].second, left_piece);
				left_destination = std::min(final_destination.position[i].second, left_destination);
			}

			if (left_piece < left_destination) next_move = RIGHT;
			else if (left_piece > left_destination) next_move = LEFT;
			else next_move = DOWN;
		}
	}
	*/

	next_move = route_to_destination[route_counter];
	route_counter++;
	if (route_counter == route_to_destination.size()) route_counter = 0;

	return next_move;
}