#include "collision.h"


int euclid_dist(int x1, int y1, int x2, int y2){
  return sqrt(pow(x1 - x2, 2) + pow(y1 - y2,2));
}

int euclid_dist_xpoint(int x1, int y1, XPoint xy){
  return sqrt(pow(x1 - xy.x, 2) + pow(y1 - xy.y,2));
}

int euclid_dist_xpoint2(XPoint xy1, XPoint xy2){
  return sqrt(pow(xy1.x - xy2.x, 2) + pow(xy1.y - xy2.y,2));
}

void draw_circles(Display* dis, Window win, GC gc, XArc* arcs, int n, unsigned long color){
  XSetForeground(dis, gc, color);
  XFillArcs(dis, win, gc, arcs, n);
  /*for (int i = 0; i < n; i++){*/
    /*int diameter = arcs[i].height;*/
    /*XFillArc(dis, win, gc, arcs[i].x-(arcs[i].width/2), arcs[i].y-(arcs[i].height/2), diameter, diameter, 0, 360*64);*/
  /*}*/
  return;
}


XPoint circle_get_center(XArc arc){
  XPoint xy;
  xy.x = arc.x + (arc.width /2);
  xy.y = arc.y + (arc.height/2);
  return xy;
}

int circle_get_radius(XArc arc){
  return arc.width/2;
}

XArc circle_create(unsigned int x, unsigned int y, int radius){
  XArc arc;
  arc.x = x-radius;
  arc.y = y-radius;
  arc.width = radius*2;
  arc.height = radius*2;
  arc.angle1 = 0;
  arc.angle2 = 360*64;
  return arc;
}
