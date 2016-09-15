#include "Header.H"
#include "Board.h"
#include "Cell.h"
#include "Images.h"
#include "Timer.h"
#include "Debug.h"
#include "Flags.h"
#include "Game.h"

using namespace std;

void Cell::setRow(int row) { //Sets the Cell Row
	this->row = row;
	return;
}

void Cell::setCol(int col) { //Sets the Cell Column
	this->col = col;
	return;
}

void Cell::setVal(int value) { //Sets the Cell value
	this->value = value;
	return;
}

void Cell::setMine(bool mine) { //Sets if a cell is a mine
	this->mine = mine;
	return;
}

void Cell::setChecked(bool checked) { //Sets if a cell has been checked
	this->checked = checked;
	return;
}

void Cell::setFlagged(bool flagged) { //Sets is a cell has been flagged
	this->flagged = flagged;
	return;
}

int Cell::getRow() { //Gets the Cell Row
	return row;
}

int Cell::getCol() { //Gets the Cell Column
	return col;
}

int Cell::getVal() { //Gets the Cell Value
	return value;
}

bool Cell::isMine() { //Gets whether the cell is a mine
	return mine;
}

bool Cell::isChecked() { //Gets whether the cell has been checked
	return checked;
}

bool Cell::isFlagged() { //Gets whether the cell has been flagged
	return flagged;
}

int Cell::getCount() { //Gets the Cell Count
	return count;
}

void Cell::setCount(int count) { //Sets the Cell Count
	this->count = count;
}

int Cell::handle(int event) { //Cell Handler
	if (event == FL_RELEASE && gameBoard->getKeepPlaying()) { //Makes sure the user has not won or lost
		if (Fl::event_button() == FL_LEFT_MOUSE) { //Handles Left Click
			if (gameBoard->getDebug()) {
				cout << "FL_LEFT_BUTTON" << endl;
			}
			if (count != 1) {
				if (value == -1) { //Case when you click on a mine
					gameBoard->uncover(row, col); 
					this->image(&bbr);
					if (bbr.w() == 0 || bbr.h() == 0 || bbr.d() == 0) {
						cerr << "Error loading image.\nPress enter to close...";
						cin.get();
						exit(1);
					}
					this->align(FL_ALIGN_IMAGE_BACKDROP);
				}
				else { //Case when it is not a mine
					if (gameBoard->getDebug()) {
						cout << "Unconvering Cell: Row: " << row << " Cell: " << col << endl;
						cout << "It is touching " << value << " mines." << endl << endl;
					}
					gameBoard->uncover(row, col);
				}
			}
		}
		if (Fl::event_button() == FL_RIGHT_MOUSE && !this->isChecked()) { //Handles Right Click
			if (gameBoard->getDebug()) {
				cout << "FL_RIGHT_BUTTON" << endl;
			}
			++count;
			if (count == 1) { //Flags a Cell
				if (gameBoard->getDebug()) {
					cout << "Cell: Row: " << row << " Col: " << col << " has been flagged." << endl << endl;
				}
				this->image(&flag);
				if (flag.w() == 0 || flag.h() == 0 || flag.d() == 0) {
					cerr << "Error loading image.\nPress enter to close...";
					cin.get();
					exit(1);
				}
				this->align(FL_ALIGN_IMAGE_BACKDROP);
				this->setFlagged(true);
				gameBoard->getFlags()->flagUsed();
				redraw();
			}
			else if (count == 2) { //Question Marks a Cell
				if (gameBoard->getDebug()) {
					cout << "Cell: Row: " << row << " Col: " << col << " has been question marked." << endl << endl;
				}
				this->image(&qm);
				if (qm.w() == 0 || qm.h() == 0 || qm.d() == 0) {
					cerr << "Error loading image.\nPress enter to close...";
					cin.get();
					exit(1);
				}
				this->align(FL_ALIGN_IMAGE_BACKDROP);
				this->setFlagged(false);
				gameBoard->getFlags()->flagNotUsed();
				redraw();
			}
			else { //Returns it to a regular cell
				cout << endl;
				count = 0;
				this->image(&brick);
				if (brick.w() == 0 || brick.h() == 0 || brick.d() == 0) {
					cerr << "Error loading image.\nPress enter to close...";
					cin.get();
					exit(1);
				}
				this->align(FL_ALIGN_IMAGE_BACKDROP);
				redraw();
			}
		}
		else if (Fl::event_button() == FL_MIDDLE_MOUSE) { //Handles Middle Click
			cout << "middle butt";
			gameBoard->middleClick(row, col);
		}
		return 1;
	}
	this->redraw();
}


void Board::setBoardSizeCol(int col) { //Sets the Board Column Size
	boardSizeCol = col;
}

void Board::setBoardSizeRow(int row) { //Sets the Board Row Size
	boardSizeRow = row;
}

void Board::setNumMines(int mines) { //Sets the Number of Mines
	numMines = mines;
}

int Board::getBoardSizeCol() { //Gets the Board Column Size
	return boardSizeCol;
}

int Board::getBoardSizeRow() { //Gets the Board Row Size
	return boardSizeRow;
}

int Board::getNumMines() { //Gets the Number of Mines
	return numMines;
}

Board::Board(int numRow, int numCol, int numMines) { //Board Constructor
	boardSizeRow = numRow;
	boardSizeCol = numCol;
	int mines = numMines;
	int creatingMines = 0;
	int row = 0;
	int col = 0;
	srand(time(0));
	numBoard.resize(boardSizeRow + 2);
	for (int i = 0; i < boardSizeRow + 2; ++i) {
		numBoard.at(i).resize(boardSizeCol + 2);
	}
	
	while (creatingMines < numMines) { //Creates the mines
		row = rand() % (boardSizeRow);
		col = rand() % (boardSizeCol);
		if (numBoard.at(row).at(col) != -1 && row != 0 && col != 0 && row != numBoard.size() - 1 && col != numBoard.at(row).size() - 1) {
			numBoard.at(row).at(col) = -1;
			++creatingMines;
		}
	}
	for (int i = 1; i < numBoard.size(); ++i) {
		for (int j = 1; j < numBoard.at(0).size(); ++j) {
			if (numBoard.at(i).at(j) == -1) {
				for (int k = i - 1; k <= i + 1; ++k) {
					for (int l = j - 1; l <= j + 1; ++l) {
						if (numBoard.at(k).at(l) != -1) {
							numBoard.at(k).at(l) = numBoard.at(k).at(l) + 1;
						}
					}
				}
			}
		}
	}
	numBoard.resize(boardSizeRow);
	for (int i = 0; i < boardSizeRow; ++i) {
		numBoard.at(i).resize(boardSizeCol);
	}
	board.resize(boardSizeRow);
	for (int i = 0; i < boardSizeRow; ++i) {
		board.at(i).resize(boardSizeCol);
	}
	
	for (int i = 0; i < boardSizeRow; ++i) { //Fills the board with cells
		for (int j = 0; j < boardSizeCol; ++j) {
			Cell *mybutton = new Cell(i * 40 + 5, j * 40 + 100, 35, 35, i, j, this);
			board.at(i).at(j) = mybutton;
			board.at(i).at(j)->setVal(numBoard.at(i).at(j));
			board.at(i).at(j)->image(&brick);
			board.at(i).at(j)->align(FL_ALIGN_IMAGE_BACKDROP);
		}
	}
	if (numMines == 10) { //Sets the Timer and Progress Button for Easy Mode
		keepTime = new Timer(290, 30, 50, 50);
		progress = new Fl_Button(165, 40, 35, 35);
		progress->image(&mario); 
		progress->align(FL_ALIGN_IMAGE_BACKDROP);
	}
	else if (numMines == 40) { //Sets the Timer and Progress Button for Medium Mode
		keepTime = new Timer(580, 30, 50, 50);
		progress = new Fl_Button(305, 40, 35, 35);
		progress->image(&mario);
		progress->align(FL_ALIGN_IMAGE_BACKDROP);
	}
	else { //Sets the Timer and Progress Button for Hard Mode
		keepTime = new Timer(1100, 30, 50, 50);
		progress = new Fl_Button(585, 40, 35, 35);
		progress->image(&mario);
		progress->align(FL_ALIGN_IMAGE_BACKDROP);
	}
	flags = new Flags(numMines, 10, 60, 30, 30); //Creates the Flag Counter
	return;
}

Board::~Board() { //Board Destructor
	if (debug) {
		cout << "Destructor called." << endl;
		cout << "Clearing number and button vectors." << endl;
		cout << "Creating mines." << endl;
		cout << "Filling board with new cells." << endl;
	}
	board.clear();
	numBoard.clear();
}

void Board::PrintBoard() { //Prints the Board
	int const WID = 5;
	for (int i = 0; i < boardSizeCol; ++i) {
		for (int j = 0; j < boardSizeRow; ++j) {
			cout << setw(WID) << numBoard.at(j).at(i);
		}
		cout << endl;
	}
	cout << endl;
	return;
}

void Board::uncover(int row, int col) { //The uncover function
	keepTime->startTimer();
	if (numBoard.at(row).at(col) == 0 && board.at(row).at(col)->isChecked() == false && board.at(row).at(col)->isFlagged() == false) { //Handles when the cell value is 0
		board.at(row).at(col)->setChecked(true);
		for (int i = row - 1; i <= row + 1; ++i) {
			for (int j = col - 1; j <= col + 1; ++j) {
				if (i < boardSizeCol && j < boardSizeRow && i >= 0 && j >= 0 && board.at(i).at(j)->isChecked() == false) { 
					board.at(i).at(j)->handle(FL_PUSH);
					board.at(row).at(col)->image(&ub);
					board.at(row).at(col)->align(FL_ALIGN_IMAGE_BACKDROP);
					board.at(row).at(col)->redraw();
					uncover(i, j); //recursive uncover
				}
			}
		}
	}
	else if (numBoard.at(row).at(col) == -1 && board.at(row).at(col)->isChecked() == false && board.at(row).at(col)->isFlagged() == false) { //Handles when its a mine
		board.at(row).at(col)->setChecked(true);
		if (this->getDebug()) {
			cout << "Unconvering MINE: Row: " << row << " Col: " << col << endl;
		}
		if (keepPlaying == true) {
			if (debug) {
				cout << "OH NOOOO!! YOU LOSE!!!" << endl;
			}
			progress->image(&bjr);
			progress->align(FL_ALIGN_IMAGE_BACKDROP);
			progress->redraw();
		}
		this->setKeepPlaying(false);
		this->getTimer()->stopTimer();
		for (int i = 0; i <= boardSizeRow; ++i) { //changed
			for (int j = 0; j <= boardSizeCol; ++j) { // changed
				if (i < boardSizeRow && j < boardSizeCol && i >= 0 && j >= 0 && numBoard.at(i).at(j) == -1 && board.at(i).at(j)->isFlagged() == false) {
					board.at(row).at(col)->image(&bwb);
					board.at(row).at(col)->align(FL_ALIGN_IMAGE_BACKDROP);
					board.at(i).at(j)->redraw();
					uncover(i, j);
				}
				if (i < boardSizeRow && j < boardSizeCol && i >= 0 && j >= 0 && numBoard.at(i).at(j) != -1 && board.at(i).at(j)->isFlagged() == true) {//flagged cell is not mine and other mine clicked
					board.at(i).at(j)->image(&nm);
					board.at(i).at(j)->align(FL_ALIGN_IMAGE_BACKDROP);
					board.at(i).at(j)->redraw();
				}
			}
		}
	}
	else if (board.at(row).at(col)->getVal() > 0 && board.at(row).at(col)->isFlagged() == false) { //Handles when it is touching a mine
		board.at(row).at(col)->setChecked(true);
		switch (board.at(row).at(col)->getVal()) {
		case 1:
			board.at(row).at(col)->image(&one);
			board.at(row).at(col)->align(FL_ALIGN_IMAGE_BACKDROP);
			break;
		case 2:
			board.at(row).at(col)->image(&two);
			board.at(row).at(col)->align(FL_ALIGN_IMAGE_BACKDROP);
			break;
		case 3:
			board.at(row).at(col)->image(&three);
			board.at(row).at(col)->align(FL_ALIGN_IMAGE_BACKDROP);
			break;
		case 4:
			board.at(row).at(col)->image(&four);
			board.at(row).at(col)->align(FL_ALIGN_IMAGE_BACKDROP);
			break;
		case 5:
			board.at(row).at(col)->image(&five);
			board.at(row).at(col)->align(FL_ALIGN_IMAGE_BACKDROP);
			break;
		case 6:
			board.at(row).at(col)->image(&six);
			board.at(row).at(col)->align(FL_ALIGN_IMAGE_BACKDROP);
			break;
		case 7:
			board.at(row).at(col)->image(&seven);
			board.at(row).at(col)->align(FL_ALIGN_IMAGE_BACKDROP);
			break;
		case 8:
			board.at(row).at(col)->image(&eight);
			board.at(row).at(col)->align(FL_ALIGN_IMAGE_BACKDROP);
			break;
		}
		board.at(row).at(col)->redraw();
	}

	if (getWinner()) {
		if (debug) {
			cout << "YOU'RE NUMBER 1!!!"; 
		}
		progress->image(&peach); //changes the progress button when you win
		progress->align(FL_ALIGN_IMAGE_BACKDROP);
		progress->redraw();
	}
	return;
}

void Board::setKeepPlaying(bool keepPlaying) { //Sets wether the game is still running
	this->keepPlaying = keepPlaying;
	return;
}

bool Board::getKeepPlaying() { //Gets whether the game is still running
	return keepPlaying;
}

Timer* Board::getTimer() { //Gets the Timer
	return keepTime;
}

Flags* Board::getFlags() { //Gets the Flag
	return flags;
}

void Board::resetFlags() { //Resets the Flag Count
	this->getFlags()->setFlags(numMines);
}

void Board::switchDebug() { //Switches it to Debug Mode
	debug = !debug;
	return;
}

bool Board::getDebug() { //Gets whether Debug Mode is On or Off
	return debug;
}

void Board::middleClick(int row, int col) { //Middle Click Function

	int surroundingFlags = 0;

	for (int i = row - 1; i <= row + 1; ++i) { //Gets the Number of Surrounding Flags
		for (int j = col - 1; j <= col + 1; ++j) {
			if (i < boardSizeRow && j < boardSizeCol && i >= 0 && j >= 0) {
				if (board.at(i).at(j)->getCount() == 1) {
					++surroundingFlags;
				}
			}
		}
	}

	if (surroundingFlags == board.at(row).at(col)->getVal()) { //Only runs if the surrounding flags is correct
		for (int i = row - 1; i <= row + 1; ++i) {
			for (int j = col - 1; j <= col + 1; ++j) {
				if (i < boardSizeRow && j < boardSizeCol && i >= 0 && j >= 0 && board.at(i).at(j)->isChecked() == false && board.at(i).at(j)->isFlagged() == false) {
					board.at(i).at(j)->handle(FL_PUSH);
					cout << i << " " << j << endl;
					this->uncover(i, j);
				}
			}
		}
	}
	return;
}

Fl_Button* Board::getProgressButton() { //Gets the Progress Button
	return progress;
}

bool Board::getWinner() { //Gets whether the game has been won
	int minesFound = 0;
	int checkedBoxes = 0;
	for (int i = 0; i < boardSizeRow; ++i) {
		for (int j = 0; j < boardSizeCol; ++j) {
			if (board.at(i).at(j)->isChecked()) {
				++checkedBoxes;
				if (board.at(i).at(j)->isMine()) {
					++minesFound;
					keepPlaying = false;
				}
			}
		}
	}
	if (checkedBoxes == (boardSizeCol*boardSizeRow - numMines) && minesFound < 1) {
		winner = true;
		keepPlaying = false;
		this->getTimer()->stopTimer();
		for (int i = 0; i < boardSizeRow; ++i) {
			for (int j = 0; j < boardSizeCol; ++j) {
				if (board.at(i).at(j)->getVal() == -1) {
					board.at(i).at(j)->deimage(); //Flags all the mines when the game is won
					board.at(i).at(j)->image(&flag);
					if (flag.w() == 0 || flag.h() == 0 || flag.d() == 0) {
						cerr << "Error loading image.\nPress enter to close...";
						cin.get();
						exit(1);
					}
					board.at(i).at(j)->align(FL_ALIGN_IMAGE_BACKDROP);
					board.at(i).at(j)->redraw();
				}
			}
		}
	}
	return winner;
}

Cell* Board::getBoardCell(int i, int j) { //Returns the cell at that specific position
	return board.at(i).at(j);
}

int Debug::handle(int event) {
	//show mines when in debug mode and recovers them if debug mode is turned off
	if (Fl_Check_Button::handle(event) && Fl::event_button() == FL_LEFT_MOUSE) {
		if (event = FL_RELEASE && Fl::event_button() == FL_LEFT_MOUSE) {
			userBoard->switchDebug();
			if (userBoard->getKeepPlaying() == true) {
				if (userBoard->getDebug()) { //Case for when debug mode is on
					for (int i = 0; i < userBoard->getBoardSizeRow(); ++i) {
						for (int j = 0; j < userBoard->getBoardSizeCol(); ++j) {
							if (userBoard->getBoardCell(i, j)->getVal() == -1) {
								userBoard->getBoardCell(i, j)->image(&luigi);
								userBoard->getBoardCell(i, j)->align(FL_ALIGN_IMAGE_BACKDROP);
								userBoard->getBoardCell(i, j)->redraw();
							}
						}
					}
				}
				else { //Case for when debug mode is off
					for (int i = 0; i < userBoard->getBoardSizeRow(); ++i) {
						for (int j = 0; j < userBoard->getBoardSizeCol(); ++j) {
							if (userBoard->getBoardCell(i, j)->getVal() == -1 && userBoard->getKeepPlaying() == true) {
								userBoard->getBoardCell(i, j)->image(&brick);
								userBoard->getBoardCell(i, j)->align(FL_ALIGN_IMAGE_BACKDROP);
								userBoard->getBoardCell(i, j)->redraw();
							}
						}
					}
				}
			}
		}
	}
	return 0;
}

int main(int argc, char **argv) {

	Game game;
	game.playGame(argc, argv); //plays the game

	
	return Fl::run();
}
