#include "Header.H"
#include "Board.h"
#include "Cell.h"
#include "Images.h"
#include "Timer.h"
#include "Debug.h"
#include "Flags.h"
#include "Game.h"
using namespace std;

/*
TO-D0 List:
Figure out why it crashes sometimes when switching to another level //Have no idea
Check to make sure the reset always works //I checked and theres something wrong
Add Debug Mode to the other levels //Crashes sometimes so had to comment out
Put everything in cpp an h files
Change the flag count for higher levels //DONE
Change the timer position //DONE
Change the flag position //DONE
Change the reset button position //DONE
Custom Level
Put keep time in a destructor
Somehow need to destruct restart as well
*/


void Cell::setRow(int row) {
	this->row = row;
	return;
}

void Cell::setCol(int col) {
	this->col = col;
	return;
}

void Cell::setVal(int value) {
	this->value = value;
	return;
}

void Cell::setMine(bool mine) {
	this->mine = mine;
	return;
}

void Cell::setChecked(bool checked) {
	this->checked = checked;
	return;
}

void Cell::setFlagged(bool flagged) {
	this->flagged = flagged;
	return;
}

int Cell::getRow() {
	return row;
}

int Cell::getCol() {
	return col;
}

int Cell::getVal() {
	return value;
}

bool Cell::isMine() {
	return mine;
}

bool Cell::isChecked() {
	return checked;
}

bool Cell::isFlagged() {
	return flagged;
}

int Cell::getCount() {
	return count;
}

void Cell::setCount(int count) {
	this->count = count;
}

int Cell::handle(int event) { //commented it out so I can mess with it
	if (event == FL_RELEASE && gameBoard->getKeepPlaying()) {
		if (Fl::event_button() == FL_LEFT_MOUSE) {
			if (gameBoard->getDebug()) {
				cout << "FL_LEFT_BUTTON" << endl;
			}
			if (count != 1) {
				if (value == -1) {
					//this->deactivate();
					gameBoard->uncover(row, col); // I think there is a problem with handeling the cell because it prints fine
					this->image(&bbr);
					if (bbr.w() == 0 || bbr.h() == 0 || bbr.d() == 0) {
						cerr << "Error loading image.\nPress enter to close...";
						cin.get();
						exit(1);
					}
					this->align(FL_ALIGN_IMAGE_BACKDROP);
				}
				else {
					if (gameBoard->getDebug()) {
						cout << "Unconvering Cell: Row: " << row << " Cell: " << col << endl;
						cout << "It is touching " << value << " mines." << endl << endl;
					}
					gameBoard->uncover(row, col);
				}
			}
		}
		if (Fl::event_button() == FL_RIGHT_MOUSE && !this->isChecked()) {
			if (gameBoard->getDebug()) {
				cout << "FL_RIGHT_BUTTON" << endl;
			}
			++count;
			if (count == 1) {
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
			else if (count == 2) {
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
			else {
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
		else if (Fl::event_button() == FL_MIDDLE_MOUSE) {
			cout << "middle button";
			gameBoard->middleClick(row, col);
		}
		return 1;
	}
	this->redraw();
}



void Board::setBoardSizeCol(int col) {
	boardSizeCol = col;
}
void Board::setBoardSizeRow(int row) {
	boardSizeRow = row;
}
void Board::setNumMines(int mines) {
	numMines = mines;
}
int Board::getBoardSizeCol() {
	return boardSizeCol;
}
int Board::getBoardSizeRow() {
	return boardSizeRow;
}
int Board::getNumMines() {
	return numMines;
}


Board::Board(int numRow, int numCol, int numMines) {
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
	cout << "Creating mines. \n";
	while (creatingMines < numMines) {
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
	cout << "Filling board with new cells.\n";
	for (int i = 0; i < boardSizeRow; ++i) {
		for (int j = 0; j < boardSizeCol; ++j) {
			Cell *mybutton = new Cell(i * 40 + 5, j * 40 + 100, 35, 35, i, j, this);
			board.at(i).at(j) = mybutton;
			board.at(i).at(j)->setVal(numBoard.at(i).at(j));
			board.at(i).at(j)->image(&brick);
			board.at(i).at(j)->align(FL_ALIGN_IMAGE_BACKDROP);
		}
	}
	if (numMines == 10) {
		keepTime = new Timer(290, 30, 50, 50);
	}
	else if (numMines == 40) {
		keepTime = new Timer(580, 30, 50, 50);
	}
	else {
		keepTime = new Timer(1000, 30, 50, 50);
	}
	flags = new Flags(numMines, 10, 60, 30, 30);
	return;
}


Board::~Board() {
	cout << "Destructor called. Clearing number and button vectors. \n";
	board.clear();
	numBoard.clear();
}

void Board::PrintBoard() {
	int const WID = 5;
	for (int i = 0; i < boardSizeCol; ++i) {
		for (int j = 0; j < boardSizeRow; ++j) {
			cout << setw(WID) << numBoard.at(j).at(i); //change this back. I was testing something. Nvm. Leave. I was correct:)
		}
		cout << endl;
	}
	cout << endl;
	return;
}

//Problem with swtich levels after a win??

void Board::uncover(int row, int col) {
	keepTime->startTimer();
	if (numBoard.at(row).at(col) == 0 && board.at(row).at(col)->isChecked() == false && board.at(row).at(col)->isFlagged() == false) {
		board.at(row).at(col)->setChecked(true);
		for (int i = row - 1; i <= row + 1; ++i) {
			for (int j = col - 1; j <= col + 1; ++j) {
				if (i < boardSizeCol && j < boardSizeRow && i >= 0 && j >= 0 && board.at(i).at(j)->isChecked() == false) { //changed this too
					board.at(i).at(j)->handle(FL_PUSH);
					board.at(row).at(col)->image(&ub);
					board.at(row).at(col)->align(FL_ALIGN_IMAGE_BACKDROP);
					board.at(row).at(col)->redraw();
					uncover(i, j);
				}
			}
		}
	}
	else if (numBoard.at(row).at(col) == -1 && board.at(row).at(col)->isChecked() == false && board.at(row).at(col)->isFlagged() == false) {
		board.at(row).at(col)->setChecked(true);
		if (this->getDebug()) {
			cout << "Unconvering MINE: Row: " << row << " Col: " << col << endl;
		}
		if (keepPlaying == true) {
			cout << "YOU LOSE!!!" << endl;

		}
		this->setKeepPlaying(false);
		this->getTimer()->stopTimer();
		for (int i = 0; i <= boardSizeRow; ++i) { //changed
			for (int j = 0; j <= boardSizeCol; ++j) { // changed
				if (i < boardSizeRow && j < boardSizeCol && i >= 0 && j >= 0 && numBoard.at(i).at(j) == -1 && board.at(i).at(j)->isChecked() == false && board.at(i).at(j)->isFlagged() == false) {
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
	else if (board.at(row).at(col)->getVal() > 0 && board.at(row).at(col)->isFlagged() == false) {
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
		cout << "YOU WIN!!!";
	}
	return;
}

void Board::setKeepPlaying(bool keepPlaying) {
	this->keepPlaying = keepPlaying;
	return;
}

bool Board::getKeepPlaying() {
	return keepPlaying;
}

Timer* Board::getTimer() {
	return keepTime;
}

Flags* Board::getFlags() {
	return flags;
}

void Board::resetFlags() {
	this->getFlags()->setFlags(numMines);
}

void Board::switchDebug() {
	debug = !debug;
	return;
}

bool Board::getDebug() {
	return debug;
}

void Board::middleClick(int row, int col) {

	int surroundingFlags = 0;
	cout << "made it" << endl;
	for (int i = row - 1; i <= row + 1; ++i) {
		for (int j = col - 1; j <= col + 1; ++j) {
			if (i < boardSizeRow && j < boardSizeCol && i >= 0 && j >= 0) {
				if (board.at(i).at(j)->getCount() == 1) {
					++surroundingFlags;
					cout << "found one " << endl;
				}
			}
		}
	}
	if (surroundingFlags == board.at(row).at(col)->getVal()) {
		cout << "they matched" << endl;
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

bool Board::getWinner() {
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
					board.at(i).at(j)->deimage();
					board.at(i).at(j)->image(&flag);
					if (flag.w() == 0 || flag.h() == 0 || flag.d() == 0) {
						cerr << "Error loading image.\nPress enter to close...";
						cin.get();
						exit(1);
					}
					board.at(i).at(j)->align(FL_ALIGN_IMAGE_BACKDROP);
				}
			}
		}
	}
	return winner;
}

Cell* Board::getBoardCell(int i, int j) {
	return board.at(i).at(j);
}

int Debug::handle(int event) {
	if (Fl_Check_Button::handle(event) && Fl::event_button() == FL_LEFT_MOUSE) {
		if (event = FL_RELEASE && Fl::event_button() == FL_LEFT_MOUSE) {
			userBoard->switchDebug();
			if (userBoard->getKeepPlaying() == true) {
				if (userBoard->getDebug()) {
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
				else {
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


void Quit_CB(Fl_Widget *, void *) {
	exit(0);
}

void About_CB(Fl_Widget *, void *) {
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
		"Joel Coatney: CEEN, Class of '18. Speaks fluent Java. \n");
	aboutWindow->show();
	aboutWindow->end();
}


int main(int argc, char **argv) {

	Game game;
	game.playGame(argc, argv);

	
	return Fl::run();
}
