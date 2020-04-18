#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <string>
#include <fstream>
#include <vector>
#include <windows.h>

#include "BoolEngine\BoolEngine.h"
#include "UserInterface.h"
#include "TetrisHedonisticIA.h"
#include "TetrisGeneticIA.h"

const float SCREEN_WIDTH = 614;  // GetSystemMetrics(SM_CXSCREEN) * 1 / 2.5;
const float SCREEN_HEIGHT = 692; // GetSystemMetrics(SM_CYSCREEN) - GetSystemMetrics(SM_CYSCREEN) * 1 / 5;
const float GENERAL_REFRESH_FPS = 50;
const float KEY_REFRESH_FPS = 13;
const float KEY_READY_FPS = 1;

int main(){

	//FreeConsole(); //Hides the console.
	srand(static_cast <unsigned> (time(0))); //Random numbers.

	bool done = false, draw = false, refresh_key = false;
	std::vector<bool> keys(ALLEGRO_KEY_MAX);
	ALLEGRO_DISPLAY *display = nullptr;
	ALLEGRO_TIMER *general_timer = nullptr, *down_timer = nullptr, *key_timer = nullptr, *ready_timer = nullptr;
	ALLEGRO_EVENT_QUEUE *event_queue = nullptr;
	ALLEGRO_KEYBOARD_STATE keyState;
	ALLEGRO_BITMAP *icon = nullptr;

	//Initialize Programme
	BoolEngine boolengine;

	boolengine.initializeFramework(true, true, false, true, true, true, 2);
	boolengine.control.createDisplay(display, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, "Tetris", boolengine.control.standard, icon);
	boolengine.control.initializeEvent(display, event_queue, true, true, false);

	UserInterface UI(boolengine, SCREEN_WIDTH, SCREEN_HEIGHT);

	//TetrisGeneticIA trainer;
	//trainer.setPopulation(trainer.generateInitialPopulation());
	//trainer.trainPopulation();
	
	//chromosome bot_brain = trainer.getBestChromosome();
	//TetrisHedonisticIA bot(bot_brain.height_weight, bot_brain.clears_weight, bot_brain.hole_weight, bot_brain.blockade_weight);
	TetrisHedonisticIA bot;
	bool start_bot = true;
	bool ia_enable = true;

	//Load Score board
	UI.menu.loadScoreBoard();

	//Show initial UI
	UI.draw();
	boolengine.graphics.showScreen();
	
	//Set Timers
		//Timer for the general refresh game
	boolengine.control.createTimer(general_timer, GENERAL_REFRESH_FPS);
	boolengine.control.registerTimer(event_queue, general_timer);
	boolengine.control.startTimer(general_timer);	

		//Timer for the speed of the pieces going down
	boolengine.control.createTimer(down_timer, UI.game.getSpeed());
	boolengine.control.registerTimer(event_queue, down_timer);
	boolengine.control.startTimer(down_timer);

		//Timer for the smooth piece movement: POSSIBLE CHANGE?
	boolengine.control.createTimer(key_timer, KEY_REFRESH_FPS);
	boolengine.control.registerTimer(event_queue, key_timer);
	boolengine.control.startTimer(key_timer);

		//Timer for the ready countdown
	boolengine.control.createTimer(ready_timer, KEY_READY_FPS);
	boolengine.control.registerTimer(event_queue, ready_timer);
	boolengine.control.startTimer(ready_timer);
	boolengine.control.stopTimer(ready_timer);

	//Game Loop
	while (!done) {

		//1) UPDATE
			ALLEGRO_EVENT events;
			boolengine.control.waitEvent(event_queue, events);

			//Keyboard events
			if (events.type == EVENT_KEY_DOWN) { //If some key has been Pressed
				if (events.keyboard.keycode == KEY_ESCAPE) done = true; //Escape Pressed
				else if (events.keyboard.keycode == KEY_P && UI.menu.getState() == OFF) { //P pressed and we are not in the menu
					if (UI.game.getState() == PAUSE) { //If the game is in pause -> Run the game again!
						UI.game.setState(READY);
						UI.setFromPause(true);
						UI.setReadyCounter(3);
						boolengine.control.startTimer(ready_timer);
					}
					else if (UI.game.getState() == RUNNING) { //If the game is running -> Pause
						UI.setAudio(false);
						UI.setKeyAudio(UI.getAudio());
						UI.game.setState(PAUSE);
					}
				}
				else if (events.keyboard.keycode == KEY_Q) { //If Q pressed
					if (UI.game.getState() == PAUSE) { //If the game is in pause -> Reset the game
						UI.game.setState(RESET);
						UI.setAudio(false);
						UI.setKeyAudio(UI.getAudio());
					}
					else if (UI.game.getState() == GAME_OVER) { //If the game is over -> Reset the game
						UI.menu.updateScoreBoard({ UI.game.getPlayerName(), std::to_string(UI.game.getScore()) });
						UI.game.setState(RESET);
					}
				}
				else if (events.keyboard.keycode == KEY_O && UI.menu.getState() == OFF) { //If O pressed and we are in the game -> change audio mode
					UI.setAudio(!UI.getAudio());
					UI.setKeyAudio(UI.getAudio());
				}
				else if (events.keyboard.keycode == KEY_ENTER && UI.menu.getState() == ON) { //If ENTER pressed and we are in the menu -> start game!
						UI.menu.setState(OFF);
						UI.game.setState(READY);
						UI.setFromPause(false);
						UI.setReadyCounter(3);
						UI.game.setPlayerName(UI.menu.getIntroduceName());
						UI.menu.setIntroduceName("");
						boolengine.control.startTimer(ready_timer);
				}
				else if (events.keyboard.keycode == KEY_COMMA) {
					if (ia_enable) ia_enable = false;
					else ia_enable = true;
				}
				else keys[events.keyboard.keycode] = true; //If other key has been pressed, register it.
			}
			else if (events.type == EVENT_KEY_UP) {
				keys[events.keyboard.keycode] = false;
			}
			//Display events
			else if (events.type == EVENT_DISPLAY_CLOSE) done = true; //If x of the window has been clicked -> close game
			//Timer Events
			else if (events.type == EVENT_TIMER) {
				if (events.timer.source == ready_timer) { //If ready_timer -> decrement countdown
					if (UI.getReadyCounter() == 0) boolengine.control.stopTimer(ready_timer);
					else UI.setReadyCounter(UI.getReadyCounter() - 1);
				}
				if (events.timer.source == down_timer) {
					//if (ia_enable) bot.removeDown();
					//keys[0] = true; //If down_timer -> move piece down
				}
				if (events.timer.source == key_timer) refresh_key = true; //If key_timer -> smooth movement
				if (events.timer.source == general_timer) { //If general_timer -> update the game
					draw = true;
					if (UI.menu.getState() != OFF) { //If the menu is ON, we execute it
						UI.menu.execute(boolengine, keys, refresh_key);
						if (refresh_key) refresh_key = false;
					}
					else{ //If the menu is OFF, we execute the game

						//Ask the Bot to give us the next movement
						if (UI.game.getState() == RUNNING && ia_enable) {
							//If it is the first piece -> calculate first piece
							if (start_bot) {
								start_bot = false;
								bot.calculateDestination(UI.game.getTable(), UI.game.getPiece());
							}
							GameDirection bot_next_movement = bot.getNextMovement(UI.game.getTable(), UI.game.getPiece());
							switch (bot_next_movement){
							case UP: keys[KEY_UP] = true; break;
							case RIGHT: keys[KEY_RIGHT] = true; break;
							case DOWN: keys[KEY_DOWN] = true; break;
							case LEFT: keys[KEY_LEFT] = true; break;
							}
						}

						UI.game.execute(keys, refresh_key, ia_enable);

						if (UI.game.getState() == RUNNING) {

							if (ia_enable && UI.game.getPiecePlaced()) { //Calculate next destination of Bot if the piece has been placed
								bot.calculateDestination(UI.game.getTable(), UI.game.getPiece());
								UI.game.setPiecePlaced(false);
							}

							if (refresh_key) refresh_key = false;
							if (UI.game.getNewTimer()) { //If we have passed of level: increment speed (new timer with more FPS)
								boolengine.control.stopTimer(down_timer);
								boolengine.control.destroyTimer(down_timer);
								boolengine.control.createTimer(down_timer, UI.game.getSpeed());
								boolengine.control.registerTimer(event_queue, down_timer);
								boolengine.control.startTimer(down_timer);
								UI.game.setNewTimer(false);
							}
						}
						else if (UI.game.getState() == RESET) { //If the game has been reseted, we enable the menu
							UI.menu.setState(ON);
						}
						else if (UI.game.getState() == GAME_OVER) { //If the player has lost, disable audio
							UI.setAudio(false);
							UI.setKeyAudio(UI.getAudio());
						}
					}
				}
			}

		//2) DRAW
		if (draw) {
			UI.draw();
			boolengine.graphics.showScreen(); //Shows everything drawed
			draw = false;
		}
	}

	//Saving Data
	UI.menu.saveScoreBoard();

	//Destroying programme
	UI.destroyUI();
	boolengine.destroyFramework(true, true, false, true, true, true);

	return 0;
}