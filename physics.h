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


void update_position(Circle* ball, float milli_diff, int width, int height);
void update_velocity(Circle* ball);

int obst_ball_collision(Circle obst, Circle* ball);


// Remove this after fixing the bouncing
typedef struct{
  float x;
  float y;
} Velocity;


#endif
