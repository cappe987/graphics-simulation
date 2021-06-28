#ifndef COLLISION_H
#define COLLISION_H
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <math.h>

#include "circle.h"



int euclid_dist(int x1, int y1, int x2, int y2);
int euclid_dist_xpoint(int x1, int y1, XPoint xy);
int euclid_dist_xpoint2(XPoint xy1, XPoint xy2);
int euclid_dist_circles(Circle c1, Circle c2);





typedef struct{
  float x;
  float y;
} Velocity;


#endif
