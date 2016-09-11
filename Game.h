#pragma once
#include "Header.h"
#include "Board.h"
#include "Images.h"
#include "Debug.h"

using namespace std;

class Game {
private:
	Fl_Window *window = new Fl_Window(365, 465);;
	Fl_Menu_Bar *menu;
	Board* userBoard = NULL;
	Debug* debugger = NULL;
	static void Menu_CB(Fl_Widget*w, void*data) {
		Game *g = (Game*)data;
		g->Menu_CB2();
	}

	void Menu_CB2() {
		char picked[80];
		menu->item_pathname(picked, sizeof(picked) - 1);
		printf("CALLBACK: You picked '%s'\n", picked);

		// How to handle callbacks..
		if (strcmp(picked, "File/Quit") == 0) exit(0);
			
		if (strcmp(picked, "File/Easy") == 0) {
			//delete newGame;
			window->resize(100, 100, 365, 465);
			menu->resize(0, 0, window->w(), 25);
			//Deconstruct old board
			userBoard->getTimer()->stopTimer();
			delete userBoard;
			delete debugger;
			window->begin();
			Board* easyBoard= new Board(9, 9, 10);
			userBoard = easyBoard;
			userBoard->PrintBoard();
			debugger = new Debug(10, 25, 30, 30, userBoard);
			window->end();
			window->redraw();
		}
		if (strcmp(picked, "File/Medium") == 0) {
			window->resize(100, 100, 645, 745);
			menu->resize(0, 0, window->w(), 25);
			window->begin();
			userBoard->getTimer()->stopTimer();
			//Deconstruct old board
			delete userBoard;
			delete debugger;
			Board* medBoard = new Board(16, 16, 40);
			userBoard = medBoard;
			userBoard->PrintBoard();
			debugger = new Debug(10, 25, 30, 30, userBoard);
			window->end();
			window->redraw();
		}
		if (strcmp(picked, "File/Hard") == 0) {
			window->resize(100, 100, 1205,745);
			menu->resize(0, 0, window->w(), 25);
			window->begin();
			userBoard->getTimer()->stopTimer();
			//Deconstruct old board
			delete userBoard;
			delete debugger;
			Board* hardBoard = new Board(30, 16, 99);
			userBoard = hardBoard;
			userBoard->PrintBoard();
			debugger = new Debug(10, 25, 30, 30, userBoard);
			window->end();
			window->redraw();
		}
		if (strcmp(picked, "File/Custom...") == 0) {
			int r = 0;
			int c = 0; 
			int m = 0;
			Fl_Window *customWindow = new Fl_Window(50, 110);
			customWindow->label("Custom Game");
			{Fl_Input *customRows = new Fl_Input(70, 10, 30, 20, "Rows:");
			}
			{Fl_Input *customCol = new Fl_Input(70, 30, 30, 20, "Columns:");
			}
			{Fl_Input *customMines = new Fl_Input(70, 50, 30, 20, "Mines:");
			}
			customWindow->show();
		}
		//Handle about 
		if (strcmp(picked, "Help/About") == 0) {
			//Open about window
			Fl_Window *aboutWindow = new Fl_Window(550, 400);
			Fl_Text_Buffer *buff = new Fl_Text_Buffer();
			Fl_Text_Display *disp = new Fl_Text_Display(20, 20, 550 - 40, 150 - 40, "Developers");
			disp->buffer(buff);
			aboutWindow->label("About");
			aboutWindow->resizable(*disp);
			Fl_Box* developerImage = new Fl_Box(250, 250, 50, 50);
			developerImage->image(&about);
			buff->text("Frank Dirosa: CEEN, Class of '18. Enjoys the new Justin Beiber Album.\n"
				"Briana Martinez: CEEN, Class of '18. Went to an Early College High School. \n"
				"Clay Joseck: CEEN, Class of '18. Has had a beard since he was 7 years old. \n"
				"Joel Coatney: CEEN, Class of '18. Plays trumpet in the Aggie Band. \n");
			aboutWindow->show();
			aboutWindow->end();
		}
	}
	char difficulty = 'e'; //e-easy, m-medium, h-hard

public:
	Game() {
		window->label("Mario Minesweeper!");
		menu = new Fl_Menu_Bar(0, 0, window->w(), 25);
		menu->add("File/Easy", 0, Menu_CB, (void*)this);
		menu->add("File/Medium", 0, Menu_CB, (void*)this);
		menu->add("File/Hard", 0, Menu_CB, (void*)this);
		menu->add("File/Custom...", 0, Menu_CB, (void*)this);
		menu->add("File/Quit", 0, Menu_CB, (void*)this);
		menu->add("Help/About", 0, Menu_CB, (void*)this);
		Board* startBoard = new Board(9, 9, 10);
		this->userBoard = startBoard;
	}

	int Game::playGame(int argc, char **argv) {

		debugger = new Debug(10, 25, 30, 30, userBoard);
		window->end();
		window->show();
		return Fl::run();
	}


};