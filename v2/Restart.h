#pragma once
#include "Header.h"
#include "Images.h"
#include "Board.h"
#include "Game.h"
class Restart : public Fl_Button {
private:
	Board* userBoard;
	//Game* game;
public:
	Restart(int x, int y, int w, int h, Board* board) : Fl_Button(x, y, w, h), userBoard(board) {
		this->image(&mario);
		this->align(FL_ALIGN_IMAGE_BACKDROP);
	}
	int handle(int event);
	~Restart() {
		if (userBoard != 0) {
			delete userBoard;
		}
	}
	void draw() {
		Fl_Button::draw();
	}
};

int Restart::handle(int event) {
	if (event == FL_RELEASE) {
		//delete userBoard;
		//userBoard = new Board(9, 9, 10);
		userBoard->resetBoard();
		userBoard->PrintBoard();
		//userBoard->resetBoard();
		//userBoard->getTimer()->restartTimer();
		//userBoard->getTimer()->stopTimer();
		//userBoard->PrintBoard();
	}
	return 1;
}