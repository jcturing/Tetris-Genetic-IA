#include "TetrisGame.h"

TetrisGame::TetrisGame() {
	table = Matriz<GameObject>(TABLE_HEIGHT, TABLE_WIDTH, EMPTY);
	piece = GamePiece();
	level = 1;
	state = RUNNING;
	player_name = " ";
	score = 0;
	speed = INITIAL_SPEED;
	lines = 0;
	newTimer = false;
}

void TetrisGame::execute(std::vector<bool> &keys, bool refresh_key, bool IA_ENABLE) {
	if (state == RUNNING) {
		for (int i = 0; i < keys.size(); i++) {

			if (!IA_ENABLE) { //Si la IA no esta encendida:
				//La primera condicion es para las teclas que se quieren hacer una sola vez, es decir no cuentan que se tengan presionadas: girar y el timer down
				//La segunda condicion es para las teclas que si cuentan tenerlas presionadas: moverse lateralmente y abajo
				if (((i == 0 || i == KEY_UP) && keys[i]) || (keys[i]) && refresh_key) {
					switch (i) {
					case 0:
						keys[i] = false;
						movePiece(DOWN, piece, table); //Timer down
						break;
					case KEY_D:
					case KEY_RIGHT: movePiece(RIGHT, piece, table); break;
					case KEY_A:
					case KEY_LEFT: movePiece(LEFT, piece, table); break;
					case KEY_W:
					case KEY_UP:
						keys[i] = false;
						movePiece(UP, piece, table);
						break;
					case KEY_S:
					case KEY_DOWN: movePiece(DOWN, piece, table); break;
					case KEY_SPACE:
						keys[i] = false;
						movePiece(BOTTOM, piece, table);
						break;
					}
				}
			}
			else { //Si la IA esta encendida: todas las teclas se ejecutan solo una vez, no hay arrastre.
				if (keys[i]) {
					switch (i) {
						case 0:
							movePiece(DOWN, piece, table); //Timer down
							break;
						case KEY_D:
						case KEY_RIGHT: movePiece(RIGHT, piece, table); break;
						case KEY_A:
						case KEY_LEFT: movePiece(LEFT, piece, table); break;
						case KEY_W:
						case KEY_UP:
							movePiece(UP, piece, table);
							break;
						case KEY_S:
						case KEY_DOWN: movePiece(DOWN, piece, table); break;
						case KEY_SPACE:
							movePiece(BOTTOM, piece, table);
							break;
					}
					keys[i] = false;
				}
			}

			if (piece.placed) {
				score++;
				checkLine();
				checkGameOver();
				if (state != GAME_OVER) {
					generateNewPiece();
					piece_placed = true;
				}
			}	
		}
	}
	else if (state == RESET) {
		table = Matriz<GameObject>(TABLE_HEIGHT, TABLE_WIDTH, EMPTY);
		piece = GamePiece();
		level = 1;
		player_name = " ";
		score = 0;
		speed = INITIAL_SPEED;
		lines = 0;
		newTimer = false;
	}
}

void TetrisGame::movePiece(GameDirection direction, GamePiece &piece_to_move, Matriz<GameObject> &table_to_move) {
	bool possible = true;
	
	if (direction == UP) {
		if(!isCollision(UP, piece_to_move, table_to_move)) rotatePiece(piece_to_move, table_to_move);
	}
	else if (direction == LEFT) {
		if (!isCollision(LEFT, piece_to_move, table_to_move)) {
			for (int i = 0; i < piece_to_move.position.size(); i++) {
				table_to_move[piece_to_move.position[i].first][piece_to_move.position[i].second] = EMPTY;
				piece_to_move.position[i].second--;
			}
		}
	}
	else if (direction == RIGHT) {
		if (!isCollision(RIGHT, piece_to_move, table_to_move)) {
			for (int i = 0; i < piece_to_move.position.size(); i++) {
				table_to_move[piece_to_move.position[i].first][piece_to_move.position[i].second] = EMPTY;
				piece_to_move.position[i].second++;
			}
		}
	}
	else if (direction == DOWN) {
		if (!isCollision(DOWN, piece_to_move, table_to_move)) {
			for (int i = 0; i < piece_to_move.position.size(); i++) {
				table_to_move[piece_to_move.position[i].first][piece_to_move.position[i].second] = EMPTY;
				piece_to_move.position[i].first++;
			}			
		}
		else {
			piece_to_move.placed = true;
		}
	}
	else if (direction == BOTTOM) {
		while (!isCollision(DOWN, piece_to_move, table_to_move)) {
			for (int i = 0; i < piece_to_move.position.size(); i++) {
				table_to_move[piece_to_move.position[i].first][piece_to_move.position[i].second] = EMPTY;
				piece_to_move.position[i].first++;
			}
		}
		piece_to_move.placed = true;
	}
	for (int i = 0; i < piece_to_move.position.size(); i++) table_to_move[piece_to_move.position[i].first][piece_to_move.position[i].second] = piece_to_move.type;
}

bool TetrisGame::isCollision(GameDirection direction, GamePiece const& piece_to_check, Matriz<GameObject> const& table_to_check) {
	bool collision = false;
	std::vector<std::pair<int, int>> v = piece_to_check.position;

	if (direction == UP) {
		//Inverse logic in this one... Just to lazy to check every condition
		collision = true;
		if (piece_to_check.type == I) {
			if (piece_to_check.state == 0) { //I is horizontal -> --
				if (piece_to_check.position[0].first - 1 >= 0 && table_to_check[piece_to_check.position[0].first - 1][piece_to_check.position[0].second + 1] == EMPTY && piece_to_check.position[2].first + 1 < TABLE_HEIGHT && table_to_check[piece_to_check.position[2].first + 1][piece_to_check.position[2].second - 1] == EMPTY
					&& piece_to_check.position[3].first + 2 < TABLE_HEIGHT && table_to_check[piece_to_check.position[3].first + 2][piece_to_check.position[3].second - 2] == EMPTY) {
					collision = false;
				}
			}
			else { //La I esta de pie
				if (piece_to_check.position[0].second - 1 >= 0 && table_to_check[piece_to_check.position[0].first + 1][piece_to_check.position[0].second - 1] == EMPTY && piece_to_check.position[2].second + 1 < TABLE_WIDTH && table_to_check[piece_to_check.position[2].first - 1][piece_to_check.position[2].second + 1] == EMPTY
					&& piece_to_check.position[3].second + 2 < TABLE_WIDTH && table_to_check[piece_to_check.position[3].first - 2][piece_to_check.position[3].second + 2] == EMPTY) {
					collision = false;
				}
			}
		}
		else if (piece_to_check.type == S) {
			if (piece_to_check.state == 0) {
				if (piece_to_check.position[0].first - 1 >= 0 && table_to_check[piece_to_check.position[0].first - 1][piece_to_check.position[0].second] == EMPTY && table_to_check[piece_to_check.position[3].first][piece_to_check.position[3].second + 1] == EMPTY) {
					collision = false;
				}
			}
			else {
				if (piece_to_check.position[0].second - 1 >= 0 && table_to_check[piece_to_check.position[2].first + 1][piece_to_check.position[2].second - 2] == EMPTY && table_to_check[piece_to_check.position[3].first][piece_to_check.position[3].second - 1] == EMPTY) {
					collision = false;
				}
			}
		}
		else if (piece_to_check.type == Z) {
			if (piece_to_check.state == 0) {
				if (piece_to_check.position[0].first - 1 >= 0 && table_to_check[piece_to_check.position[0].first - 1][piece_to_check.position[0].second + 2] == EMPTY && table_to_check[piece_to_check.position[2].first - 1][piece_to_check.position[2].second + 1] == EMPTY) {
					collision = false;
				}
			}
			else {
				if (piece_to_check.position[1].second - 1 >= 0 && table_to_check[piece_to_check.position[0].first + 1][piece_to_check.position[0].second - 2] == EMPTY && table_to_check[piece_to_check.position[3].first][piece_to_check.position[3].second + 1] == EMPTY) {
					collision = false;
				}
			}
		}
		else if (piece_to_check.type == L) {
			if (piece_to_check.state == 0) {
				if (piece_to_check.position[0].first - 1 >= 0 && table_to_check[piece_to_check.position[0].first - 1][piece_to_check.position[0].second] == EMPTY && table_to_check[piece_to_check.position[1].first - 1][piece_to_check.position[3].second] == EMPTY
					&& table_to_check[piece_to_check.position[3].first][piece_to_check.position[3].second + 1] == EMPTY) {
					collision = false;
				}
			}
			else if (piece_to_check.state == 1) {
				if (piece_to_check.position[1].second + 1 < TABLE_WIDTH && table_to_check[piece_to_check.position[0].first][piece_to_check.position[0].second + 2] == EMPTY && table_to_check[piece_to_check.position[1].first + 1][piece_to_check.position[1].second - 1] == EMPTY
					&& table_to_check[piece_to_check.position[3].first - 1][piece_to_check.position[3].second + 1] == EMPTY) {
					collision = false;
				}
			}
			else if (piece_to_check.state == 2) {
				if (piece_to_check.position[1].first + 1 < TABLE_HEIGHT && table_to_check[piece_to_check.position[0].first][piece_to_check.position[0].second - 1] == EMPTY && table_to_check[piece_to_check.position[2].first + 1][piece_to_check.position[2].second] == EMPTY
					&& table_to_check[piece_to_check.position[3].first + 1][piece_to_check.position[3].second] == EMPTY) {
					collision = false;
				}
			}
			else if (piece_to_check.state == 3) {
				if (piece_to_check.position[0].second - 1 >= 0 && table_to_check[piece_to_check.position[0].first + 1][piece_to_check.position[0].second - 1] == EMPTY && table_to_check[piece_to_check.position[2].first - 1][piece_to_check.position[2].second + 1] == EMPTY
					&& table_to_check[piece_to_check.position[3].first][piece_to_check.position[3].second - 2] == EMPTY) {
					collision = false;
				}
			}
		}
		else if (piece_to_check.type == J) {
			if (piece_to_check.state == 0) {
				if (piece_to_check.position[0].first - 1 >= 0 && table_to_check[piece_to_check.position[0].first - 1][piece_to_check.position[0].second + 1] == EMPTY && table_to_check[piece_to_check.position[2].first + 1][piece_to_check.position[2].second - 2] == EMPTY
					&& table_to_check[piece_to_check.position[3].first][piece_to_check.position[3].second - 1] == EMPTY) {
					collision = false;
				}
			}
			else if (piece_to_check.state == 1) {
				if (piece_to_check.position[0].second + 1 < TABLE_WIDTH && table_to_check[piece_to_check.position[0].first][piece_to_check.position[0].second - 1] == EMPTY && table_to_check[piece_to_check.position[1].first][piece_to_check.position[1].second - 1] == EMPTY
					&& table_to_check[piece_to_check.position[3].first - 1][piece_to_check.position[3].second + 1] == EMPTY) {
					collision = false;
				}
			}
			else if (piece_to_check.state == 2) {
				if (piece_to_check.position[1].first + 1 < TABLE_HEIGHT && table_to_check[piece_to_check.position[0].first][piece_to_check.position[0].second + 1] == EMPTY && table_to_check[piece_to_check.position[1].first - 1][piece_to_check.position[1].second + 2] == EMPTY
					&& table_to_check[piece_to_check.position[3].first + 1][piece_to_check.position[3].second - 1] == EMPTY) {
					collision = false;
				}
			}
			else if (piece_to_check.state == 3) {
				if (piece_to_check.position[0].second - 1 >= 0 && table_to_check[piece_to_check.position[0].first + 1][piece_to_check.position[0].second - 1] == EMPTY && table_to_check[piece_to_check.position[2].first][piece_to_check.position[2].second + 1] == EMPTY
					&& table_to_check[piece_to_check.position[3].first][piece_to_check.position[3].second + 1] == EMPTY) {
					collision = false;
				}
			}
		}
		else if (piece_to_check.type == T) {
			if (piece_to_check.state == 0) {
				if (piece_to_check.position[0].first - 1 >= 0 && table_to_check[piece_to_check.position[0].first - 1][piece_to_check.position[0].second + 1] == EMPTY) {
					collision = false;
				}
			}
			else if (piece_to_check.state == 1) {
				if (piece_to_check.position[0].second + 1 < TABLE_WIDTH && table_to_check[piece_to_check.position[3].first - 1][piece_to_check.position[0].second + 1] == EMPTY) {
					collision = false;
				}
			}
			else if (piece_to_check.state == 2) {
				if (piece_to_check.position[1].first + 1 < TABLE_HEIGHT && table_to_check[piece_to_check.position[3].first + 1][piece_to_check.position[3].second - 1] == EMPTY) {
					collision = false;
				}
			}
			else if (piece_to_check.state == 3) {
				if (piece_to_check.position[0].second - 1 >= 0 && table_to_check[piece_to_check.position[0].first + 1][piece_to_check.position[0].second - 1] == EMPTY) {
					collision = false;
				}
			}
		}
	}
	else if (direction == LEFT) {
		switch (piece_to_check.type) {
			case O:
				if (v[0].second == 0 || table_to_check[v[0].first][v[0].second - 1] != EMPTY || table_to_check[v[2].first][v[2].second - 1] != EMPTY) collision = true;
				break;
			case I:
				if (piece_to_check.state == 0) {
					if (v[0].second == 0 || table_to_check[v[0].first][v[0].second - 1] != EMPTY) collision = true;
				}
				else {
					if (v[0].second == 0 || table_to_check[v[0].first][v[0].second - 1] != EMPTY || table_to_check[v[1].first][v[1].second - 1] != EMPTY || table_to_check[v[2].first][v[2].second - 1] != EMPTY
						|| table_to_check[v[3].first][v[3].second - 1] != EMPTY) collision = true;
				}
				break;
			case S:
				if (piece_to_check.state == 0) {
					if (v[2].second == 0 || table_to_check[v[0].first][v[0].second - 1] != EMPTY || table_to_check[v[2].first][v[2].second - 1] != EMPTY) collision = true; 
				}
				else {
					if (v[0].second == 0 || table_to_check[v[0].first][v[0].second - 1] != EMPTY || table_to_check[v[1].first][v[1].second - 1] != EMPTY
						|| table_to_check[v[3].first][v[3].second - 1] != EMPTY) collision = true;
				}
				break;
			case Z:
				if (piece_to_check.state == 0) {
					if (v[0].second == 0 || table_to_check[v[0].first][v[0].second - 1] != EMPTY || table_to_check[v[2].first][v[2].second - 1] != EMPTY) collision = true;
				}
				else {
					if (v[1].second == 0 || table_to_check[v[0].first][v[0].second-1] != EMPTY || table_to_check[v[1].first][v[1].second-1] != EMPTY
						|| table_to_check[v[3].first][v[3].second - 1] != EMPTY) collision = true;
				}
				break;
			case L:
				if (piece_to_check.state == 0) {
					if (v[0].second == 0 || table_to_check[v[0].first][v[0].second - 1] != EMPTY || table_to_check[v[3].first][v[3].second - 1] != EMPTY) collision = true;
				}
				else if (piece_to_check.state == 1) {
					if (v[0].second == 0 || table_to_check[v[0].first][v[0].second - 1] != EMPTY || table_to_check[v[2].first][v[2].second - 1] != EMPTY
						|| table_to_check[v[3].first][v[3].second - 1] != EMPTY) collision = true;
				}
				else if (piece_to_check.state == 2) {
					if (v[1].second == 0 || table_to_check[v[0].first][v[0].second - 1] != EMPTY || table_to_check[v[1].first][v[1].second - 1] != EMPTY) collision = true;
				}
				else if (piece_to_check.state == 3) {
					if (v[0].second == 0 || table_to_check[v[0].first][v[0].second - 1] != EMPTY || table_to_check[v[1].first][v[1].second - 1] != EMPTY
						|| table_to_check[v[2].first][v[2].second - 1] != EMPTY) collision = true;
				}
				break;
			case J:
				if (piece_to_check.state == 0) {
					if (v[0].second == 0 || table_to_check[v[0].first][v[0].second - 1] != EMPTY || table_to_check[v[3].first][v[3].second - 1] != EMPTY) collision = true;
				}
				else if (piece_to_check.state == 1) {
					if (v[2].second == 0 || table_to_check[v[0].first][v[0].second - 1] != EMPTY || table_to_check[v[1].first][v[1].second - 1] != EMPTY
						|| table_to_check[v[2].first][v[2].second - 1] != EMPTY) collision = true;
				}
				else if (piece_to_check.state == 2) {
					if (v[0].second == 0 || table_to_check[v[0].first][v[0].second - 1] != EMPTY || table_to_check[v[1].first][v[1].second - 1] != EMPTY) collision = true;
				}
				else if (piece_to_check.state == 3) {
					if (v[0].second == 0 || table_to_check[v[0].first][v[0].second - 1] != EMPTY || table_to_check[v[2].first][v[2].second - 1] != EMPTY
						|| table_to_check[v[3].first][v[3].second - 1] != EMPTY) collision = true;
				}
				break;
			case T:
				if (piece_to_check.state == 0) {
					if (v[0].second == 0 || table_to_check[v[0].first][v[0].second - 1] != EMPTY || table_to_check[v[3].first][v[3].second - 1] != EMPTY) collision = true;
				}
				else if (piece_to_check.state == 1) {
					if (v[1].second == 0 || table_to_check[v[0].first][v[0].second - 1] != EMPTY || table_to_check[v[1].first][v[1].second - 1] != EMPTY
						|| table_to_check[v[3].first][v[3].second - 1] != EMPTY) collision = true;
				}
				else if (piece_to_check.state == 2) {
					if (v[1].second == 0 || table_to_check[v[0].first][v[0].second - 1] != EMPTY || table_to_check[v[1].first][v[1].second - 1] != EMPTY) collision = true;
				}
				else if (piece_to_check.state == 3) {
					if (v[0].second == 0 || table_to_check[v[0].first][v[0].second - 1] != EMPTY || table_to_check[v[1].first][v[1].second - 1] != EMPTY
						|| table_to_check[v[3].first][v[3].second - 1] != EMPTY) collision = true;
				}
				break;
		}
	}
	else if (direction == DOWN) {
		switch (piece_to_check.type) {
		case O:
			if (v[2].first == TABLE_HEIGHT-1 || table_to_check[v[2].first + 1][v[2].second] != EMPTY || table_to_check[v[3].first + 1][v[3].second] != EMPTY) collision = true;
			break;
		case I:
			if (piece_to_check.state == 0) {
				if (v[0].first == TABLE_HEIGHT - 1 || table_to_check[v[0].first + 1][v[0].second] != EMPTY || table_to_check[v[1].first + 1][v[1].second] != EMPTY
					|| table_to_check[v[2].first + 1][v[2].second] != EMPTY || table_to_check[v[3].first + 1][v[3].second] != EMPTY) collision = true;
			}
			else {
				if (v[3].first == TABLE_HEIGHT - 1 || table_to_check[v[3].first + 1][v[3].second] != EMPTY) collision = true;
			}
			break;
		case S:
			if (piece_to_check.state == 0) {
				if (v[2].first == TABLE_HEIGHT - 1 || table_to_check[v[1].first + 1][v[1].second] != EMPTY || table_to_check[v[2].first + 1][v[2].second] != EMPTY
					|| table_to_check[v[3].first + 1][v[3].second] != EMPTY) collision = true;
			}
			else {
				if (v[3].first == TABLE_HEIGHT - 1 || table_to_check[v[1].first + 1][v[1].second] != EMPTY || table_to_check[v[3].first + 1][v[3].second] != EMPTY) collision = true;
			}
			break;
		case Z:
			if (piece_to_check.state == 0) {
				if (v[2].first == TABLE_HEIGHT - 1 || table_to_check[v[0].first + 1][v[0].second] != EMPTY || table_to_check[v[2].first + 1][v[2].second] != EMPTY
					|| table_to_check[v[3].first + 1][v[3].second] != EMPTY) collision = true;
			}
			else {
				if (v[3].first == TABLE_HEIGHT - 1 || table_to_check[v[2].first + 1][v[2].second] != EMPTY || table_to_check[v[3].first + 1][v[3].second] != EMPTY) collision = true;
			}
			break;
		case L:
			if (piece_to_check.state == 0) {
				if (v[3].first == TABLE_HEIGHT - 1 || table_to_check[v[1].first+1][v[1].second] != EMPTY || table_to_check[v[2].first+1][v[2].second] != EMPTY
					|| table_to_check[v[3].first + 1][v[3].second] != EMPTY) collision = true;
			}
			else if (piece_to_check.state == 1) {
				if (v[3].first == TABLE_HEIGHT - 1 || table_to_check[v[0].first+1][v[0].second] != EMPTY || table_to_check[v[3].first+1][v[3].second] != EMPTY) collision = true;
			}
			else if (piece_to_check.state == 2) {
				if (v[1].first == TABLE_HEIGHT - 1 || table_to_check[v[1].first+1][v[1].second] != EMPTY || table_to_check[v[2].first+1][v[2].second] != EMPTY
					|| table_to_check[v[3].first + 1][v[3].second] != EMPTY) collision = true;
			}
			else if (piece_to_check.state == 3) {
				if (v[2].first == TABLE_HEIGHT - 1 || table_to_check[v[2].first+1][v[2].second] != EMPTY || table_to_check[v[3].first+1][v[3].second] != EMPTY) collision = true;
			}
			break;
		case J:
			if (piece_to_check.state == 0) {
				if (v[3].first == TABLE_HEIGHT - 1 || table_to_check[v[0].first + 1][v[0].second] != EMPTY || table_to_check[v[1].first + 1][v[1].second] != EMPTY
					|| table_to_check[v[3].first + 1][v[3].second] != EMPTY) collision = true;
			}
			else if (piece_to_check.state == 1) {
				if (v[2].first == TABLE_HEIGHT - 1 || table_to_check[v[2].first + 1][v[2].second] != EMPTY || table_to_check[v[3].first + 1][v[3].second] != EMPTY) collision = true;
			}
			else if (piece_to_check.state == 2) {
				if (v[1].first == TABLE_HEIGHT - 1 || table_to_check[v[1].first + 1][v[1].second] != EMPTY || table_to_check[v[2].first + 1][v[2].second] != EMPTY
					|| table_to_check[v[3].first + 1][v[3].second] != EMPTY) collision = true;
			}
			else if (piece_to_check.state == 3) {
				if (v[3].first == TABLE_HEIGHT - 1 || table_to_check[v[1].first + 1][v[1].second] != EMPTY || table_to_check[v[3].first + 1][v[3].second] != EMPTY) collision = true;
			}
			break;
		case T:
			if (piece_to_check.state == 0) {
				if (v[3].first == TABLE_HEIGHT - 1 || table_to_check[v[0].first + 1][v[0].second] != EMPTY || table_to_check[v[2].first + 1][v[2].second] != EMPTY
					|| table_to_check[v[3].first + 1][v[3].second] != EMPTY) collision = true;
			}
			else if (piece_to_check.state == 1) {
				if (v[3].first == TABLE_HEIGHT - 1 || table_to_check[v[1].first + 1][v[1].second] != EMPTY || table_to_check[v[3].first + 1][v[3].second] != EMPTY) collision = true;
			}
			else if (piece_to_check.state == 2) {
				if (v[1].first == TABLE_HEIGHT - 1 || table_to_check[v[1].first + 1][v[1].second] != EMPTY || table_to_check[v[2].first + 1][v[2].second] != EMPTY
					|| table_to_check[v[3].first + 1][v[3].second] != EMPTY) collision = true;
			}
			else if (piece_to_check.state == 3) {
				if (v[3].first == TABLE_HEIGHT - 1 || table_to_check[v[2].first + 1][v[2].second] != EMPTY || table_to_check[v[3].first + 1][v[3].second] != EMPTY) collision = true;
			}
			break;
		}
	}
	else if (direction == RIGHT) {
		switch (piece_to_check.type) {
		case O:
			if (v[1].second == TABLE_WIDTH-1 || table_to_check[v[1].first][v[1].second + 1] != EMPTY || table_to_check[v[3].first][v[3].second + 1] != EMPTY) collision = true;
			break;
		case I:
			if (piece_to_check.state == 0) {
				if (v[3].second == TABLE_WIDTH - 1 || table_to_check[v[3].first][v[3].second + 1] != EMPTY) collision = true;
			}
			else {
				if (v[0].second == TABLE_WIDTH - 1 || table_to_check[v[0].first][v[0].second + 1] != EMPTY || table_to_check[v[1].first][v[1].second + 1] != EMPTY || table_to_check[v[2].first][v[2].second + 1] != EMPTY
					|| table_to_check[v[3].first][v[3].second + 1] != EMPTY) collision = true;
			}
			break;
		case S:
			if (piece_to_check.state == 0) {
				if (v[1].second == TABLE_WIDTH-1 || table_to_check[v[1].first][v[1].second + 1] != EMPTY || table_to_check[v[3].first][v[3].second + 1] != EMPTY) collision = true;
			}
			else {
				if (v[2].second == TABLE_WIDTH - 1 || table_to_check[v[0].first][v[0].second + 1] != EMPTY || table_to_check[v[2].first][v[2].second + 1] != EMPTY
					|| table_to_check[v[3].first][v[3].second + 1] != EMPTY) collision = true;
			}
			break;
		case Z:
			if (piece_to_check.state == 0) {
				if (v[3].second == TABLE_WIDTH-1 || table_to_check[v[1].first][v[1].second + 1] != EMPTY || table_to_check[v[3].first][v[3].second + 1] != EMPTY) collision = true;
			}
			else {
				if (v[0].second == TABLE_WIDTH-1 || table_to_check[v[0].first][v[0].second + 1] != EMPTY || table_to_check[v[2].first][v[2].second + 1] != EMPTY
					|| table_to_check[v[3].first][v[3].second + 1] != EMPTY) collision = true;
			}
			break;
		case L:
			if (piece_to_check.state == 0) {
				if (v[2].second == TABLE_WIDTH - 1 || table_to_check[v[2].first][v[2].second + 1] != EMPTY || table_to_check[v[3].first][v[3].second + 1] != EMPTY) collision = true;
			}
			else if (piece_to_check.state == 1) {
				if (v[1].second == TABLE_WIDTH - 1 || table_to_check[v[1].first][v[1].second + 1] != EMPTY || table_to_check[v[2].first][v[2].second + 1] != EMPTY
					|| table_to_check[v[3].first][v[3].second + 1] != EMPTY) collision = true;
			}
			else if (piece_to_check.state == 2) {
				if (v[0].second == TABLE_WIDTH - 1 || table_to_check[v[0].first][v[0].second + 1] != EMPTY || table_to_check[v[3].first][v[3].second + 1] != EMPTY) collision = true;
			}
			else if (piece_to_check.state == 3) {
				if (v[3].second == TABLE_WIDTH - 1 || table_to_check[v[0].first][v[0].second + 1] != EMPTY || table_to_check[v[1].first][v[1].second + 1] != EMPTY
					|| table_to_check[v[3].first][v[3].second + 1] != EMPTY) collision = true;
			}
			break;
		case J:
			if (piece_to_check.state == 0) {
				if (v[2].second == TABLE_WIDTH - 1 || table_to_check[v[2].first][v[2].second + 1] != EMPTY || table_to_check[v[3].first][v[3].second + 1] != EMPTY) collision = true;
			}
			else if (piece_to_check.state == 1) {
				if (v[0].second == TABLE_WIDTH - 1 || table_to_check[v[0].first][v[0].second + 1] != EMPTY || table_to_check[v[1].first][v[1].second + 1] != EMPTY
					|| table_to_check[v[3].first][v[3].second + 1] != EMPTY) collision = true;
			}
			else if (piece_to_check.state == 2) {
				if (v[3].second == TABLE_WIDTH - 1 || table_to_check[v[0].first][v[0].second + 1] != EMPTY || table_to_check[v[3].first][v[3].second + 1] != EMPTY) collision = true;
			}
			else if (piece_to_check.state == 3) {
				if (v[1].second == TABLE_WIDTH - 1 || table_to_check[v[1].first][v[1].second + 1] != EMPTY || table_to_check[v[2].first][v[2].second + 1] != EMPTY
					|| table_to_check[v[3].first][v[3].second + 1] != EMPTY) collision = true;
			}
			break;
		case T:
			if (piece_to_check.state == 0) {
				if (v[2].second == TABLE_WIDTH - 1 || table_to_check[v[2].first][v[2].second + 1] != EMPTY || table_to_check[v[3].first][v[3].second + 1] != EMPTY) collision = true;
			}
			else if (piece_to_check.state == 1) {
				if (v[0].second == TABLE_WIDTH - 1 || table_to_check[v[0].first][v[0].second + 1] != EMPTY || table_to_check[v[2].first][v[2].second + 1] != EMPTY
					|| table_to_check[v[3].first][v[3].second + 1] != EMPTY) collision = true;
			}
			else if (piece_to_check.state == 2) {
				if (v[3].second == TABLE_WIDTH - 1 || table_to_check[v[0].first][v[0].second + 1] != EMPTY || table_to_check[v[3].first][v[3].second + 1] != EMPTY) collision = true;
			}
			else if (piece_to_check.state == 3) {
				if (v[2].second == TABLE_WIDTH - 1 || table_to_check[v[0].first][v[0].second + 1] != EMPTY || table_to_check[v[2].first][v[2].second + 1] != EMPTY
					|| table_to_check[v[3].first][v[3].second + 1] != EMPTY) collision = true;
			}
			break;
		}
	}
	return collision;
}

void TetrisGame::rotatePiece(GamePiece &piece_to_move, Matriz<GameObject> &table_to_move){
	if (piece_to_move.type == I) {
		if (piece_to_move.state == 0) { //La I esta de lado
				for (int i = 0; i <piece_to_move.position.size(); i++) table_to_move[piece_to_move.position[i].first][piece_to_move.position[i].second] = EMPTY;
				piece_to_move.position[0].first--;
				piece_to_move.position[0].second++;
				piece_to_move.position[2].first++;
				piece_to_move.position[2].second--;
				piece_to_move.position[3].first += 2;
				piece_to_move.position[3].second += -2;
				piece_to_move.state = 1;
		}
		else { //La I esta de pie
				for (int i = 0; i < piece_to_move.position.size(); i++) table_to_move[piece_to_move.position[i].first][piece_to_move.position[i].second] = EMPTY;
				piece_to_move.position[0].first++;
				piece_to_move.position[0].second--;
				piece_to_move.position[2].first--;
				piece_to_move.position[2].second++;
				piece_to_move.position[3].first += -2;
				piece_to_move.position[3].second += 2;
				piece_to_move.state = 0;
		}
	}
	else if(piece_to_move.type == S) {
		if (piece_to_move.state == 0) {
				for (int i = 0; i < piece_to_move.position.size(); i++) table_to_move[piece_to_move.position[i].first][piece_to_move.position[i].second] = EMPTY;
				piece_to_move.position[0].first--;
				piece_to_move.position[1].second--;
				piece_to_move.position[2].first--;
				piece_to_move.position[2].second += 2;
				piece_to_move.position[3].second++;
				piece_to_move.state = 1;
		}
		else {
				for (int i = 0; i < piece_to_move.position.size(); i++) table_to_move[piece_to_move.position[i].first][piece_to_move.position[i].second] = EMPTY;
				piece_to_move.position[0].first++;
				piece_to_move.position[1].second++;
				piece_to_move.position[2].first++;
				piece_to_move.position[2].second += -2;
				piece_to_move.position[3].second--;
				piece_to_move.state = 0;
		}
	}
	else if (piece_to_move.type == Z) {
 		if (piece_to_move.state == 0) {
				for (int i = 0; i < piece_to_move.position.size(); i++) table_to_move[piece_to_move.position[i].first][piece_to_move.position[i].second] = EMPTY;
				piece_to_move.position[0].first--;
				piece_to_move.position[0].second += 2;
				piece_to_move.position[2].first--;
				piece_to_move.position[2].second++;
				piece_to_move.position[3].second--;
				piece_to_move.state = 1;
		}
		else {
				for (int i = 0; i < piece_to_move.position.size(); i++) table_to_move[piece_to_move.position[i].first][piece_to_move.position[i].second] = EMPTY;
				piece_to_move.position[0].first++;
				piece_to_move.position[0].second += -2;
				piece_to_move.position[2].first++;
				piece_to_move.position[2].second--;
				piece_to_move.position[3].second++;
				piece_to_move.state = 0;
		}
	}
	else if (piece_to_move.type == L) {
		if (piece_to_move.state == 0) {
				for (int i = 0; i < piece_to_move.position.size(); i++) table_to_move[piece_to_move.position[i].first][piece_to_move.position[i].second] = EMPTY;
				piece_to_move.position[0].first--;
				piece_to_move.position[1].first--;
				piece_to_move.position[2].second--;
				piece_to_move.position[3].second++;
				piece_to_move.state = 1;
		}
		else if (piece_to_move.state == 1) {
				for (int i = 0; i < piece_to_move.position.size(); i++) table_to_move[piece_to_move.position[i].first][piece_to_move.position[i].second] = EMPTY;
				piece_to_move.position[0].second += 2;
				piece_to_move.position[1].first++;
				piece_to_move.position[1].second--;
				piece_to_move.position[3].first--;
				piece_to_move.position[3].second++;
				piece_to_move.state = 2;
		}
		else if (piece_to_move.state == 2) {
				for (int i = 0; i < piece_to_move.position.size(); i++) table_to_move[piece_to_move.position[i].first][piece_to_move.position[i].second] = EMPTY;
				piece_to_move.position[0].second--;
				piece_to_move.position[1].second++;
				piece_to_move.position[2].first++;
				piece_to_move.position[3].first++;
				piece_to_move.state = 3;
		}
		else if (piece_to_move.state == 3) {
				for (int i = 0; i < piece_to_move.position.size(); i++) table_to_move[piece_to_move.position[i].first][piece_to_move.position[i].second] = EMPTY;
				piece_to_move.position[0].first++;
				piece_to_move.position[0].second--;
				piece_to_move.position[2].first--;
				piece_to_move.position[2].second++;
				piece_to_move.position[3].second += -2;
				piece_to_move.state = 0;
		}
	}
	else if (piece_to_move.type == J) {
		if (piece_to_move.state == 0) {
				for (int i = 0; i < piece_to_move.position.size(); i++) table_to_move[piece_to_move.position[i].first][piece_to_move.position[i].second] = EMPTY;
				piece_to_move.position[0].first--;
				piece_to_move.position[0].second++;
				piece_to_move.position[2].first++;
				piece_to_move.position[2].second += -2;
				piece_to_move.position[3].second--;
				piece_to_move.state = 1;
		}
		else if (piece_to_move.state == 1) {
				for (int i = 0; i < piece_to_move.position.size(); i++) table_to_move[piece_to_move.position[i].first][piece_to_move.position[i].second] = EMPTY;
				piece_to_move.position[0].second--;
				piece_to_move.position[1].second--;
				piece_to_move.position[2].first--;
				piece_to_move.position[2].second++;
				piece_to_move.position[3].first--;
				piece_to_move.position[3].second++;
				piece_to_move.state = 2;
		}
		else if (piece_to_move.state == 2) {
				for (int i = 0; i < piece_to_move.position.size(); i++) table_to_move[piece_to_move.position[i].first][piece_to_move.position[i].second] = EMPTY;
				piece_to_move.position[0].second++;
				piece_to_move.position[1].first--;
				piece_to_move.position[1].second += 2;
				piece_to_move.position[3].first++;
				piece_to_move.position[3].second--;
				piece_to_move.state = 3;
		}
		else if (piece_to_move.state == 3) {
				for (int i = 0; i < piece_to_move.position.size(); i++) table_to_move[piece_to_move.position[i].first][piece_to_move.position[i].second] = EMPTY;
				piece_to_move.position[0].first++;
				piece_to_move.position[0].second--;
				piece_to_move.position[1].first++;
				piece_to_move.position[1].second--;
				piece_to_move.position[2].second++;
				piece_to_move.position[3].second++;
				piece_to_move.state = 0;
		}
	}
	else if (piece_to_move.type == T) {
		if (piece_to_move.state == 0) {
				for (int i = 0; i < piece_to_move.position.size(); i++) table_to_move[piece_to_move.position[i].first][piece_to_move.position[i].second] = EMPTY;
				piece_to_move.position[0].first--;
				piece_to_move.position[0].second++;
				piece_to_move.position[1].second--;
				piece_to_move.position[2].second--;
				piece_to_move.state = 1;
		}
		else if(piece_to_move.state == 1){
				for (int i = 0; i < piece_to_move.position.size(); i++) table_to_move[piece_to_move.position[i].first][piece_to_move.position[i].second] = EMPTY;
				piece_to_move.position[3].first--;
				piece_to_move.position[3].second++;
				piece_to_move.state = 2;
		}
		else if (piece_to_move.state == 2) {
				for (int i = 0; i < piece_to_move.position.size(); i++) table_to_move[piece_to_move.position[i].first][piece_to_move.position[i].second] = EMPTY;
				piece_to_move.position[1].second++;
				piece_to_move.position[2].second++;
				piece_to_move.position[3].first++;
				piece_to_move.position[3].second--;
				piece_to_move.state = 3;
		}
		else if(piece_to_move.state == 3){
				for (int i = 0; i < piece_to_move.position.size(); i++) table_to_move[piece_to_move.position[i].first][piece_to_move.position[i].second] = EMPTY;
				piece_to_move.position[0].first++;
				piece_to_move.position[0].second--;
				piece_to_move.state = 0;
		}
	}
}

void TetrisGame::checkLine() {
	bool clear = true;
	int counter_clear = 0;
	for (int i = 0; i < TABLE_HEIGHT; i++) {
		while (clear) {
			for (int j = 0; j < TABLE_WIDTH && clear; j++) {
				if (table[i][j] == EMPTY) clear = false;
			}
			if (clear) {
				moveTableDown(i);
				counter_clear++;
				lines += 1;
				if (lines % INCREMENT_LEVEL == 0) {
					level++;
					speed += INCREMENT_SPEED;
					newTimer = true;
				}
			}
		}
		clear = true;
	}
	
	if (counter_clear == 1) score += 40;
	else if (counter_clear == 2) score += 120;
	else if (counter_clear == 3) score += 300;
	else if (counter_clear == 4) score += 1200;
}

void TetrisGame::checkGameOver() {
	if (piece.position[0].first == 0) state = GAME_OVER;
}

void TetrisGame::moveTableDown(int t) {
	for (int i = t; i >= 0; i--) {
		for (int j = 0; j < TABLE_WIDTH; j++) {
			if (i == 0) table[i][j] = EMPTY;
			else table[i][j] = table[i-1][j];
		}
	}
}

void TetrisGame::generateNewPiece() {
	int num = 1 + rand()%7;
	//int num = 3;
	piece.placed = false;
	piece.position.clear();
	piece.state = 0;
	if (num == 1) {
		piece.position.push_back({ 0, 4 });
		piece.position.push_back({ 0, 5 });
		piece.position.push_back({ 1, 4 });
		piece.position.push_back({ 1, 5 });
		piece.type = O;
	}
	else if (num == 2) {
		piece.position.push_back({ 0, 3 });
		piece.position.push_back({ 0, 4 });
		piece.position.push_back({ 0, 5 });
		piece.position.push_back({ 0, 6 });
		piece.type = I;
	}
	else if (num == 3) {
		piece.position.push_back({ 0, 4 });
		piece.position.push_back({ 0, 5 });
		piece.position.push_back({ 1, 3 });
		piece.position.push_back({ 1, 4 });
		piece.type = S;
	}
	else if (num == 4) {
		piece.position.push_back({ 0, 3 });
		piece.position.push_back({ 0, 4 });
		piece.position.push_back({ 1, 4 });
		piece.position.push_back({ 1, 5 });
		piece.type = Z;
	}
	else if (num == 5) {
		piece.position.push_back({ 0, 3 });
		piece.position.push_back({ 0, 4 });
		piece.position.push_back({ 0, 5 });
		piece.position.push_back({ 1, 3 });
		piece.type = L;
	}
	else if (num == 6) {
		piece.position.push_back({ 0, 3 });
		piece.position.push_back({ 0, 4 });
		piece.position.push_back({ 0, 5 });
		piece.position.push_back({ 1, 5 });
		piece.type = J;
	}
	else if(num == 7) {
		piece.position.push_back({ 0, 3 });
		piece.position.push_back({ 0, 4 });
		piece.position.push_back({ 0, 5 });
		piece.position.push_back({ 1, 4 });
		piece.type = T;
	}
	for (int i = 0; i < piece.position.size(); i++) table[piece.position[i].first][piece.position[i].second] = piece.type;
}

void TetrisGame::setPlayerName(std::string newName) {
	if (newName == "") player_name = "anonymous";
	else player_name = newName; 
}