#pragma once
#include "Header.h"
using namespace std;

class Debug : public Fl_Check_Button {
private:
	bool debugLocal = false;
	Board* userBoard;
public:
	Debug(int X, int Y, int W, int H, Board* board) : Fl_Check_Button(X, Y, W, H, "Debug"), userBoard(board) {}
	int handle(int event);
};