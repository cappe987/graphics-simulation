#ifndef CIRCLE_H
#define CIRCLE_H

#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <math.h>



typedef struct {
  float x;
  float y;
  float vx;
  float vy;
  unsigned int radius;
} Circle;


void draw_circles(Display* dis, Window win, GC gc, Circle* cs, int n, unsigned long color);

Circle circle_create(unsigned int x, unsigned int y, int vx, int vy, int radius);


void add_green(int x, int y, Circle* obstacles, unsigned int* obstCount);
void remove_green(Display* dis, Window win, GC gc, int x, int y, Circle* obstacles, unsigned int* obstCount);




#endif
