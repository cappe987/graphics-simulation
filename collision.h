#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <math.h>



int euclid_dist(int x1, int y1, int x2, int y2);
int euclid_dist_xpoint(int x1, int y1, XPoint xy);
int euclid_dist_xpoint2(XPoint xy1, XPoint xy2);

void draw_circles(Display* dis, Window win, GC gc, XArc* arcs, int n, unsigned long color);

XPoint circle_get_center(XArc arc);
int    circle_get_radius(XArc arc);
XArc   circle_create(unsigned int x, unsigned int y, int radius);




typedef struct{
  unsigned int x;
  unsigned int y;
} Direction;
