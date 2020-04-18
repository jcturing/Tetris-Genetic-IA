#include "UserInterface.h"

//USER INTERFACE

UserInterface::UserInterface(BoolEngine &boolengine, int SCREEN_WIDTH, int SCREEN_HEIGHT) {
	menu.setState(ON);
	game.setState(GAME_OVER);
	ready_counter = 3;
	engine = boolengine;
	screen_width = SCREEN_WIDTH;
	screen_height = SCREEN_HEIGHT;
	aux_draw_name = "";

	counter_draw_name = 0;
	from_pause = false;
	audio = false; // true -> off | false -> on
	engine.audio.loadSong(tetris_song, "TetrisSong.wav", tetris_song_instance, loop);


	engine.graphics.loadFont(font_nickname, "Pixeled.ttf", 20);
	engine.graphics.loadFont(font_ready, "Pixeled.ttf", 100);
	engine.graphics.loadFont(font_pause, "Pixeled.ttf", 30);
	engine.graphics.loadFont(font_pause_description, "Pixeled.ttf", 20);
	engine.graphics.loadFont(font_reset, "Pixeled.ttf", 30);
	engine.graphics.loadFont(font_gameover, "Pixeled.ttf", 30);
	engine.graphics.loadFont(font_gameover_description, "Pixeled.ttf", 15);
	engine.graphics.loadFont(font_score, "Pixeled.ttf", 20);
	engine.graphics.loadFont(font_scoreboard, "Pixeled.ttf", 20);
	engine.graphics.loadFont(font_scoreboard_title, "Pixeled.ttf", 30);

	engine.graphics.loadSprite(controls_sound_UI, "UI_Controls_sound.png");
	engine.graphics.loadSprite(controls_muted_UI, "UI_Controls_muted.png");

	controls_pos_x = screen_width * 1 / 1.55;
	controls_pos_y = screen_height * 1 / 3.25;
	swidth_controls = (screen_width * 1 / 3) / engine.graphics.getSpriteWidth(controls_sound_UI);
	sheight_controls = (screen_height * 2 / 3) / engine.graphics.getSpriteHeight(controls_sound_UI);

	level_text_pos_x = screen_width * 1 / 1.5;
	level_text_pos_y = screen_height * 1 / 14;

	score_text_pos_x = screen_width * 1 / 1.5;
	score_text_pos_y = screen_height * 1 / 7;
	
	lines_text_pos_x = screen_width * 1 / 1.5;
	lines_text_pos_y = screen_height * 1 / 4.75;
	
	initial_table_pos_x = screen_width * 1 / 20;
	initial_table_pos_y = screen_height * 1 / 20;
	row_pixel_elem = 35;
	column_pixel_elem = 35;

	ready_text_pos_x = initial_table_pos_x + (TABLE_WIDTH*column_pixel_elem) / 2;
	ready_text_pos_y = initial_table_pos_y + (TABLE_HEIGHT*row_pixel_elem) / 5;

	menu_text_pos_y = screen_height * 1 / 100;
	
	nickname_text_pos_x = screen_width / 2;
	nickname_text_pos_y = menu_text_pos_y + 75;
	
	enter_text_pos_x = screen_width * 1 / 2;
	enter_text_pos_y = nickname_text_pos_y + 120;
	
	scoreboard_text_pos_x = screen_width * 1 / 6;
	scoreboard_text_pos_y = screen_height * 1 / 1.9;
	scoreboard_text_jump_y = screen_height * 1 / 20;

	engine.graphics.loadSprite(pieceEmpty_UI, "whitePiece.png");
	engine.graphics.loadSprite(pieceO_UI, "redPiece.png");
	engine.graphics.loadSprite(pieceI_UI, "bluePiece.png");
	engine.graphics.loadSprite(pieceT_UI, "yellowPiece.png");
	engine.graphics.loadSprite(pieceJ_UI, "purplePiece.png");
	engine.graphics.loadSprite(pieceL_UI, "lightGreenPiece.png");
	engine.graphics.loadSprite(pieceZ_UI, "greenPiece.png");
	engine.graphics.loadSprite(pieceS_UI, "pinkPiece.png");
}

void UserInterface::playThisSong(ALLEGRO_SAMPLE_INSTANCE * instance) {
	if (audio) engine.audio.playSong(instance);
}

void UserInterface::setKeyAudio(bool option) {
	if (!option) engine.audio.stopSong(tetris_song_instance); //Stop all the music and sound.
	else playThisSong(tetris_song_instance);
}

void UserInterface::draw() {
	if (menu.getState() != OFF) drawMenu();
	else drawGame();
}

void UserInterface::drawGame() {
	if (audio)	engine.graphics.drawSprite(controls_sound_UI, controls_pos_x, controls_pos_y, swidth_controls, sheight_controls);
	else engine.graphics.drawSprite(controls_muted_UI, controls_pos_x, controls_pos_y, swidth_controls, sheight_controls);
	engine.graphics.drawFont(font_score, al_map_rgb(0, 0, 0), level_text_pos_x, level_text_pos_y, ALLIGN_LEFT, "LEVEL: " + std::to_string(game.getLevel()));
	engine.graphics.drawFont(font_score, al_map_rgb(0, 0, 0), score_text_pos_x, score_text_pos_y, ALLIGN_LEFT, "SCORE: " + std::to_string(game.getScore()));
	engine.graphics.drawFont(font_score, al_map_rgb(0, 0, 0), lines_text_pos_x, lines_text_pos_y, ALLIGN_LEFT, "LINES:  " + std::to_string(game.getLines()));

	if (game.getState() == READY) drawGameReady();
	else if (game.getState() == RUNNING) drawGameRunning();
	else if (game.getState() == PAUSE) drawGamePause();
	else if (game.getState() == GAME_OVER) drawGameOver();
}

void UserInterface::drawGameReady() {
	if (ready_counter > 0) {
		for (int i = 0; i < TABLE_HEIGHT; i++) {
			for (int j = 0; j < TABLE_WIDTH; j++) {
				ALLEGRO_BITMAP *sprite_draw = nullptr;
				GameObject object_draw = game.getTablePos(i, j);
				float swidth = 1.4, sheight = 1.4;
				switch (object_draw) {
				case O: sprite_draw = pieceO_UI; break;
				case I: sprite_draw = pieceI_UI; break;
				case L: sprite_draw = pieceL_UI; break;
				case J: sprite_draw = pieceJ_UI; break;
				case Z: sprite_draw = pieceZ_UI; break;
				case S: sprite_draw = pieceS_UI; break;
				case T: sprite_draw = pieceT_UI; break;
				case EMPTY:
					sprite_draw = pieceEmpty_UI;
					swidth = 1.75;
					sheight = 1.75;
					break;
				}
				engine.graphics.drawSprite(sprite_draw, initial_table_pos_x + j*column_pixel_elem, initial_table_pos_y + i*row_pixel_elem, swidth, sheight);
			}
		}		
		engine.graphics.drawFont(font_ready, al_map_rgb(255, 0, 0), ready_text_pos_x, ready_text_pos_y, ALLIGN_CENTRE, std::to_string(ready_counter));
	}
	else { //Start game!
		game.setState(RUNNING);
		if(!from_pause) game.generateNewPiece();
		audio = true;
		setKeyAudio(audio);
	}
}

void UserInterface::drawGameRunning() {
	al_draw_filled_rectangle(initial_table_pos_x, initial_table_pos_y, initial_table_pos_x + TABLE_WIDTH*column_pixel_elem,
		initial_table_pos_y + TABLE_HEIGHT*row_pixel_elem, al_map_rgb(0, 0, 0));
	for (int i = 0; i < TABLE_HEIGHT; i++) {
		for (int j = 0; j < TABLE_WIDTH; j++) {
			ALLEGRO_BITMAP *sprite_draw = nullptr;
			GameObject object_draw = game.getTablePos(i, j);
			float swidth = 1.4, sheight = 1.4;
			switch (object_draw) {
			case O: sprite_draw = pieceO_UI; break;
			case I: sprite_draw = pieceI_UI; break;
			case L: sprite_draw = pieceL_UI; break;
			case J: sprite_draw = pieceJ_UI; break;
			case Z: sprite_draw = pieceZ_UI; break;
			case S: sprite_draw = pieceS_UI; break;
			case T: sprite_draw = pieceT_UI; break;
			case EMPTY:
				sprite_draw = pieceEmpty_UI;
				swidth = 1.75;
				sheight = 1.75;
				break;
			}
			engine.graphics.drawSprite(sprite_draw, initial_table_pos_x + j*column_pixel_elem, initial_table_pos_y + i*row_pixel_elem, swidth, sheight);
		}
	}
}

void UserInterface::drawGamePause() {
	engine.graphics.drawFont(font_pause, al_map_rgb(255, 255, 255), ready_text_pos_x, ready_text_pos_y, ALLIGN_CENTRE, "GAME PAUSED");
	engine.graphics.drawFont(font_pause_description, al_map_rgb(255, 255, 255), ready_text_pos_x - 100, ready_text_pos_y + 100, ALLIGN_LEFT, "P: Keep Playing");
	engine.graphics.drawFont(font_pause_description, al_map_rgb(255, 255, 255), ready_text_pos_x - 100, ready_text_pos_y + 150, ALLIGN_LEFT, "Q: Quit Game");
}

void UserInterface::drawGameOver(){
	engine.graphics.drawFont(font_gameover, al_map_rgb(255, 0, 0), ready_text_pos_x, ready_text_pos_y, ALLIGN_CENTRE, "GAME OVER");
	engine.graphics.drawFont(font_gameover_description, al_map_rgb(255, 0, 0), ready_text_pos_x, ready_text_pos_y + 100, ALLIGN_CENTRE, "Press Q to exit");
}

void UserInterface::drawMenu() {
	engine.graphics.drawFont(font_scoreboard_title, al_map_rgb(0, 0, 0), screen_width/2, menu_text_pos_y, ALLIGN_CENTRE, "MENU");
	drawIntroduceName();
	al_draw_rectangle(enter_text_pos_x - 260, enter_text_pos_y+5, enter_text_pos_x + 260, enter_text_pos_y + 90, al_map_rgb(255, 0, 0), 5);
	engine.graphics.drawFont(font_gameover, al_map_rgb(255, 0, 0), enter_text_pos_x, enter_text_pos_y, ALLIGN_CENTRE, "ENTER: Start Game");
	drawScoreboard();
}

void UserInterface::drawScoreboard() {
	engine.graphics.drawFont(font_scoreboard_title, al_map_rgb(0, 0, 0), scoreboard_text_pos_x+200, scoreboard_text_pos_y-70, ALLIGN_CENTRE, "TOP");
	for (int i = 0; i < std::min(SCOREBOARD_DIM, int(menu.getScoreBoard().size())); i++) {
			engine.graphics.drawFont(font_scoreboard, al_map_rgb(0, 0, 0), scoreboard_text_pos_x, scoreboard_text_pos_y + i*scoreboard_text_jump_y, ALLIGN_LEFT, std::to_string(i + 1) + ". " + menu.getScoreBoard()[i].first);
			engine.graphics.drawFont(font_scoreboard, al_map_rgb(0, 0, 0), scoreboard_text_pos_x + 400, scoreboard_text_pos_y + i*scoreboard_text_jump_y, ALLIGN_RIGHT, menu.getScoreBoard()[i].second);
	}
}

void UserInterface::drawIntroduceName() {
	engine.graphics.drawFont(font_nickname, al_map_rgb(255, 255, 255), nickname_text_pos_x, nickname_text_pos_y, ALLIGN_CENTRE, "Introduce Your Nickname:");
	if (menu.getIntroduceName() == "") {
		if (counter_draw_name == 15) {
			counter_draw_name = 0;
			if (aux_draw_name == "") aux_draw_name = "|";
			else aux_draw_name = "";
		}
		else {
			counter_draw_name++;
		}
		engine.graphics.drawFont(font_nickname, al_map_rgb(255, 255, 255), nickname_text_pos_x, nickname_text_pos_y + 50, ALLIGN_CENTRE, aux_draw_name);
	}
	else engine.graphics.drawFont(font_nickname, al_map_rgb(255, 255, 255), nickname_text_pos_x, nickname_text_pos_y + 50, ALLIGN_CENTRE, menu.getIntroduceName());
}

void UserInterface::destroyUI() {
	engine.graphics.destroySprite(controls_sound_UI);
	engine.graphics.destroySprite(controls_muted_UI);
	
	engine.graphics.destroySprite(pieceO_UI);
	engine.graphics.destroySprite(pieceI_UI);
	engine.graphics.destroySprite(pieceS_UI);
	engine.graphics.destroySprite(pieceZ_UI);
	engine.graphics.destroySprite(pieceT_UI);
	engine.graphics.destroySprite(pieceL_UI);
	engine.graphics.destroySprite(pieceJ_UI);
}
