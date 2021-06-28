#include "collision.h"
#include "circle.h"



int euclid_dist_circle(int x1, int y1, Circle c){
  return sqrt(pow(x1 - c.x, 2) + pow(y1 - c.y,2));
}

int euclid_dist_circles(Circle c1, Circle c2){
  return sqrt(pow(c1.x - c2.x, 2) + pow(c1.y - c2.y, 2));
}

