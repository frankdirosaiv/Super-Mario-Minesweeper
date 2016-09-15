#pragma once
#include "Header.h"
#include "Board.h"

using namespace std;

class Cell : public Fl_Button {
private:
	int row;
	int col;
	int value;
	int count;
	bool mine;
	bool checked;
	bool flagged;
	Board* gameBoard;
public:
	Cell(int x, int y, int w, int h, int r, int c, Board* board, int count = 0, bool mine = false, bool checked = false, bool flagged = false) : 
		Fl_Button(x, y, w, h), row(r), col(c), gameBoard(board) {}
	void setRow(int row);
	void setCol(int col);
	void setVal(int value);
	void setMine(bool mine);
	void setCount(int count);
	void setChecked(bool checked);
	void setFlagged(bool flagged);
	int getCount();
	int getRow();
	int getCol();
	int getVal();
	bool isMine();
	bool isChecked();
	bool isFlagged();
	int handle(int event);
};