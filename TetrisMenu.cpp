#include "TetrisMenu.h"

void TetrisMenu::execute(BoolEngine &engine, std::vector<bool> &keys, bool refresh_key) {
	for (int i = 0; i < keys.size(); i++) {
		if (keys[i] && (i != KEY_BACKSPACE || (i == KEY_BACKSPACE && refresh_key))) {
			switch (i) {
			case KEY_A:	if (introduce_name.length() < MAX_NAME_LENGHT) introduce_name += 'a'; break;
			case KEY_B: if (introduce_name.length() < MAX_NAME_LENGHT) introduce_name += 'b'; break;
			case KEY_C: if (introduce_name.length() < MAX_NAME_LENGHT) introduce_name += 'c'; break;
			case KEY_D: if (introduce_name.length() < MAX_NAME_LENGHT) introduce_name += 'd'; break;
			case KEY_E: if (introduce_name.length() < MAX_NAME_LENGHT) introduce_name += 'e'; break;
			case KEY_F: if (introduce_name.length() < MAX_NAME_LENGHT) introduce_name += 'f'; break;
			case KEY_G: if (introduce_name.length() < MAX_NAME_LENGHT) introduce_name += 'g'; break;
			case KEY_H: if (introduce_name.length() < MAX_NAME_LENGHT) introduce_name += 'h'; break;
			case KEY_I: if (introduce_name.length() < MAX_NAME_LENGHT) introduce_name += 'i'; break;
			case KEY_J: if (introduce_name.length() < MAX_NAME_LENGHT) introduce_name += 'j'; break;
			case KEY_K: if (introduce_name.length() < MAX_NAME_LENGHT) introduce_name += 'k'; break;
			case KEY_L: if (introduce_name.length() < MAX_NAME_LENGHT) introduce_name += 'l'; break;
			case KEY_M: if (introduce_name.length() < MAX_NAME_LENGHT) introduce_name += 'm'; break;
			case KEY_N: if (introduce_name.length() < MAX_NAME_LENGHT) introduce_name += 'n'; break;
			case KEY_O: if (introduce_name.length() < MAX_NAME_LENGHT) introduce_name += 'o'; break;
			case KEY_P: if (introduce_name.length() < MAX_NAME_LENGHT) introduce_name += 'p'; break;
			case KEY_Q: if (introduce_name.length() < MAX_NAME_LENGHT) introduce_name += 'q'; break;
			case KEY_R: if (introduce_name.length() < MAX_NAME_LENGHT) introduce_name += 'r'; break;
			case KEY_S: if (introduce_name.length() < MAX_NAME_LENGHT) introduce_name += 's'; break;
			case KEY_T: if (introduce_name.length() < MAX_NAME_LENGHT) introduce_name += 't'; break;
			case KEY_U: if (introduce_name.length() < MAX_NAME_LENGHT) introduce_name += 'u'; break;
			case KEY_V: if (introduce_name.length() < MAX_NAME_LENGHT) introduce_name += 'v'; break;
			case KEY_W: if (introduce_name.length() < MAX_NAME_LENGHT) introduce_name += 'w'; break;
			case KEY_X: if (introduce_name.length() < MAX_NAME_LENGHT) introduce_name += 'x'; break;
			case KEY_Y: if (introduce_name.length() < MAX_NAME_LENGHT) introduce_name += 'y'; break;
			case KEY_Z: if (introduce_name.length() < MAX_NAME_LENGHT) introduce_name += 'z'; break;
			case KEY_0: if (introduce_name.length() < MAX_NAME_LENGHT) introduce_name += '0'; break;
			case KEY_1: if (introduce_name.length() < MAX_NAME_LENGHT) introduce_name += '1'; break;
			case KEY_2: if (introduce_name.length() < MAX_NAME_LENGHT) introduce_name += '2'; break;
			case KEY_3: if (introduce_name.length() < MAX_NAME_LENGHT) introduce_name += '3'; break;
			case KEY_4: if (introduce_name.length() < MAX_NAME_LENGHT) introduce_name += '4'; break;
			case KEY_5: if (introduce_name.length() < MAX_NAME_LENGHT) introduce_name += '5'; break;
			case KEY_6: if (introduce_name.length() < MAX_NAME_LENGHT) introduce_name += '6'; break;
			case KEY_7: if (introduce_name.length() < MAX_NAME_LENGHT) introduce_name += '7'; break;
			case KEY_8: if (introduce_name.length() < MAX_NAME_LENGHT) introduce_name += '8'; break;
			case KEY_9: if (introduce_name.length() < MAX_NAME_LENGHT) introduce_name += '9'; break;
			case KEY_BACKSPACE: if (introduce_name.length() > 0) introduce_name.pop_back(); break;
			}
			if(i != KEY_BACKSPACE) keys[i] = false;
		}
	}
}

bool compareSort(std::pair<std::string, std::string> p1, std::pair<std::string, std::string> p2) {
	return std::stoi(p1.second) > std::stoi(p2.second);
}

void TetrisMenu::updateScoreBoard(std::pair<std::string,std::string> const& p) {
	score_board.push_back(p);
	sort(score_board.begin(), score_board.end(), compareSort);
}

void TetrisMenu::loadScoreBoard() {
	std::ifstream file;
	std::string aux;
	std::string aux_name;
	std::string aux_score;

	ALLEGRO_PATH * path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
	al_append_path_component(path, "resources");
	al_append_path_component(path, "data");
	al_change_directory(al_path_cstr(path, '/'));
	al_destroy_path(path);

	file.open(SCOREBOARD_NAME);
	if (file.is_open()) {
		std::getline(file, aux);
		while (!file.eof()) {
			std::stringstream o(aux);
			o >> aux_name;
			o >> aux_score;
			score_board.push_back({ aux_name, aux_score });
			std::getline(file, aux);
		}
	}
	sort(score_board.begin(), score_board.end(), compareSort);
	file.close();
}

void TetrisMenu::saveScoreBoard() {
	std::string aux;
	std::ofstream file;

	ALLEGRO_PATH * path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
	al_append_path_component(path, "resources");
	al_append_path_component(path, "data");
	al_change_directory(al_path_cstr(path, '/'));
	al_destroy_path(path);

	file.open(SCOREBOARD_NAME);
	for (int i = 0; i < score_board.size(); i++) {
		aux = score_board[i].first + " " + score_board[i].second;
		file << aux << '\n';
	}
	file.close();
}