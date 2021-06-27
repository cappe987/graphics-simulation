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




struct Fps_info init_fps_counter();
void print_fps(Display* dis, Window win, GC gc, struct Fps_info* fps);



