#ifndef COLLISION_H
#define COLLISION_H
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <math.h>

#include "circle.h"



int euclid_dist_circle(int x1, int y1, Circle c);
int euclid_dist_circles(Circle c1, Circle c2);




// Remove this after fixing the bouncing
typedef struct{
  float x;
  float y;
} Velocity;


#endif
