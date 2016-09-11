#pragma once
#include "Header.h"
#define TICK_COLOR 0
class Timer : public Fl_Box {
private:
	bool starting = false;
	bool started = true;
	bool pause = true;
	long endingTick = 0;
	static void Timer_CB(void *userdata) {
		Timer *o = (Timer*)userdata;
		o->redraw(); // this would stop the timer
		Fl::repeat_timeout(1.0, Timer_CB, userdata);
	}

public:
	// CONSTRUCTOR
	Timer(int X, int Y, int W, int H, const char*L = 0) : Fl_Box(X, Y, W, H, L) {
		box(FL_FLAT_BOX);
	}
	void draw() {
			static long start = time(NULL);
			long tick = time(NULL) - start - endingTick;
			if (starting) {
				start = time(NULL);
				tick = time(NULL) - start - endingTick;
				endingTick = tick;
				starting = false;
			}
			char secs[200];
			sprintf_s(secs, "%02ld:%02ld", tick / 60, tick % 60);
			Fl_Box::draw();
			// DRAW TIMER TEXT STRING
			fl_color(TICK_COLOR);
			fl_font(FL_HELVETICA, 16);
			fl_draw(secs, x() + 4, y() + h() - 4);
	}
	void startTimer() {
			if (started) {
				starting = true;
				started = false;
			}
			Fl::add_timeout(1.0, Timer_CB, (void*)this);
		return;
	}
	void stopTimer() {
		Fl::remove_timeout(Timer_CB, (void*)this);
	}

	void restartTimer() {
		started = false;
		starting = true;
		return;
	}
	
};