#include "fps.h"


// Returns if it printed the fps, meaning a second had passed and variables should reset.
void print_fps(Display* dis, Window win, GC gc, struct Fps_info* fps_info){
  fps_info->frameCounter++;
  time_t curr = time(0);
  if (curr > fps_info->prev){
    int diff = curr - fps_info->prev;
    fps_info->fps = fps_info->frameCounter / diff;
    fps_info->frameCounter = 0;
    fps_info->prev = curr;
  }
  char str[10] = {0};
  snprintf(str, 10, "%d", fps_info->fps);
  for (int i = 0; i < 10; i++){
    if (str[i] == 0){
      str[i] = ' ';
    }
  }
  XGCValues values;
  values.font = XLoadFont(dis, "lucidasans-bold-18");
  GC str_gc = XCreateGC(dis, win, 1L<<14, &values);
  XSetForeground(dis, gc, 0x4c594b);
  XFillRectangle(dis, win, gc, 10, 10, 100, 40);
  XSetForeground(dis, str_gc, 0x319e28);
  XDrawString(dis, win, str_gc, 15, 40, str, 10);
  /*return retvalue;*/
}


struct Fps_info init_fps_counter(){
  struct Fps_info fps_info;
  fps_info.frameCounter = 0;
  fps_info.fps = 0;
  fps_info.prev = time(0);
  return fps_info;
}