#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

#include "BoolEngine\BoolEngine.h"
#include "TetrisGame.h"
#include "TetrisMenu.h"

class UserInterface {
public:
	UserInterface(BoolEngine &engine, int const SCREEN_WIDTH, int const SCREEN_HEIGHT);

	//Enables music
	void setKeyAudio(bool option);

	//Draw what necessary depending on the state of the menu and the game
	void draw();
	
	//Destroys all the sprites freeing memory used
	void destroyUI();

	//Setters
	void setFromPause(int newOption) { from_pause = newOption; }
	void setReadyCounter(int newCounter) { ready_counter = newCounter; }
	void setAudio(bool option) { audio = option; }

	//Getters
	int getReadyCounter() { return ready_counter; }
	bool getAudio() { return audio; }
	bool getFromPause() { return from_pause; }

	TetrisGame game; //The tetris game
	TetrisMenu menu; //The menu

private:

	//Plays the song specified
	void playThisSong(ALLEGRO_SAMPLE_INSTANCE * instance);

	//Draws the menu
	void drawMenu();
	
	//Draws the score board
	void drawScoreboard();
	
	//Draws the introduce name interface
	void drawIntroduceName();
	
	//Draws the game
	void drawGame();
	
	//Draws the game ready countdown
	void drawGameReady();
	
	//Draws the game over screen
	void drawGameOver();
	
	//Draws the pause screen
	void drawGamePause();
	
	//Draws the game running: the table with the pieces
	void drawGameRunning();

	ALLEGRO_BITMAP *controls_sound_UI, *controls_muted_UI; //UI sprites
	ALLEGRO_BITMAP *pieceO_UI, *pieceL_UI, *pieceJ_UI, *pieceZ_UI, *pieceS_UI, *pieceT_UI, *pieceI_UI, *pieceEmpty_UI; //Pieces sprites
	
	//All the fonts used
	ALLEGRO_FONT *font_score, *font_scoreboard, *font_gameover, *font_pause, *font_reset, *font_ready, *font_pause_description, *font_gameover_description;
	ALLEGRO_FONT *font_scoreboard_title, *font_nickname;

	//Songs
	ALLEGRO_SAMPLE *tetris_song = nullptr;
	ALLEGRO_SAMPLE_INSTANCE *tetris_song_instance = nullptr;

	//The bool engine: connection between Allegro and the UI
	BoolEngine engine;

	//Auxiliar variables
	bool audio, from_pause; //Audio enabled
	std::string aux_draw_name; //Name being introduced
	int ready_counter, counter_draw_name; //Countdown counter and counter for the | introduce name character

	//Position in the screen of each element of the UI
	float controls_pos_x, controls_pos_y, swidth_controls, sheight_controls;

	float level_text_pos_x, level_text_pos_y;
	float score_text_pos_x, score_text_pos_y;
	float lines_text_pos_x, lines_text_pos_y;
	float ready_text_pos_x, ready_text_pos_y;
	float scoreboard_text_pos_x, scoreboard_text_pos_y, scoreboard_text_jump_y;
	float enter_text_pos_x, enter_text_pos_y;
	float nickname_text_pos_x, nickname_text_pos_y;
	float initial_table_pos_x, initial_table_pos_y, row_pixel_elem, column_pixel_elem;
	float menu_text_pos_y;

	//Screen width and height
	float screen_width, screen_height;
};

#endif
