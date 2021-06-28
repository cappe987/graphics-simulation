#ifndef FPS_H
#define FPS_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#include <stdio.h>
#include <unistd.h>


struct Fps_info {
  time_t prev;
  int frameCounter;
  int fps;
};




void init_fps_counter();
void print_fps(Display* dis, Window win, GC gc);

void init_game_time();
float get_millisecond_diff();


#endif
