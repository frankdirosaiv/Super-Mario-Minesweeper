#pragma once
#include "Header.h"
#include "Cell.h"

using namespace std;

class Board {
private:
	vector<vector<Cell*>> board;
	vector<vector<int>> numBoard;
	int numMines = 10;
	int boardSizeRow = 9;
	int boardSizeCol = 9;
	Flags* flags = NULL;
	Timer* keepTime = NULL;
	int* tester = new int(78);
	bool keepPlaying = true;
	bool winner = false;
	bool debug = false;
	Fl_Button* progress = NULL;
public:
	Board(int numRow, int numCol, int numMines);
	~Board();
	void setBoardSizeCol(int col);
	void setBoardSizeRow(int row);
	void setNumMines(int mines);
	int getBoardSizeCol();
	int getBoardSizeRow();
	int getNumMines();
	void PrintBoard();
	void uncover(int row, int col);
	void middleClick(int row, int col);
	bool getKeepPlaying();
	void setKeepPlaying(bool keepPlaying);
	void switchDebug();
	bool getDebug();
	bool getWinner();
	void resetFlags();
	Timer* getTimer();
	Flags* getFlags();
	Cell* getBoardCell(int i, int j);
	Fl_Button* getProgressButton();
};