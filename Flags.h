#pragma once
#include "Header.h"
#define TICK_COLOR 0
class Flags : Fl_Box {
private:
	long flags = 10;
public:
	Flags(int flags, int X, int Y, int W, int H, const char*L = 0) : Fl_Box(X, Y, W, H, L), flags(flags) {
		box(FL_FLAT_BOX);
	}
	void draw() {
		char secs[200];
		sprintf_s(secs, "%02ld", flags);
		Fl_Box::draw();
		fl_color(TICK_COLOR);
		fl_font(FL_HELVETICA, 16);
		fl_draw(secs, x() + 4, y() + h() - 4);
	}
	void flagUsed() {
		--flags;
		redraw();
	}
	void flagNotUsed() {
		++flags;
		redraw();
	}
	void setFlags(int flags) {
		this->flags = flags;
		return;
	}
};