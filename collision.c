#include "collision.h"
#include "circle.h"


int euclid_dist(int x1, int y1, int x2, int y2){
  return sqrt(pow(x1 - x2, 2) + pow(y1 - y2,2));
}

int euclid_dist_xpoint(int x1, int y1, XPoint xy){
  return sqrt(pow(x1 - xy.x, 2) + pow(y1 - xy.y,2));
}

int euclid_dist_xpoint2(XPoint xy1, XPoint xy2){
  return sqrt(pow(xy1.x - xy2.x, 2) + pow(xy1.y - xy2.y,2));
}

int euclid_dist_circles(Circle c1, Circle c2){
  return sqrt(pow(c1.x - c2.x, 2) + pow(c1.y - c2.y, 2));
}



/*void circle_bounce(XArc ball, XArc obst){*/
  /*XPoint ball_center = circle_get_center(ball);*/
  /*XPoint obst_center = circle_get_center(obst);*/


/*}*/
