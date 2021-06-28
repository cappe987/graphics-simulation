#include "circle.h"
#include "collision.h"






void draw_circles(Display* dis, Window win, GC gc, Circle* cs, int n, unsigned long color){
  XSetForeground(dis, gc, color);
  /*XFillArcs(dis, win, gc, arcs, n);*/
  for (int i = 0; i < n; i++){
    int r = cs[i].radius;
    XFillArc(dis, win, gc, cs[i].x-r, cs[i].y-r, r*2, r*2, 0, 360*64);
  }
  return;
}


XPoint circle_get_center(Circle arc){
  XPoint xy;
  xy.x = arc.x;
  xy.y = arc.y;
  return xy;
}

int circle_get_radius(Circle arc){
  return arc.radius;
}

Circle circle_create(unsigned int x, unsigned int y, int vx, int vy, int radius){
  Circle c;
  c.x = x;//-radius;
  c.y = y;//-radius;
  c.vx = vx;
  c.vy = vy;
  c.radius = radius;
  return c;
}



void add_green(int x, int y, Circle* obstacles, unsigned int* obstCount){
  Circle c = circle_create(x, y, 0, 0, 100);
  obstacles[*obstCount] = c;
  *obstCount = *obstCount + 1;
}

void remove_green(Display* dis, Window win, GC gc, int x, int y, Circle* obstacles, unsigned int* obstCount){
  for (int i = 0; i < *obstCount; i++){
    XPoint center = circle_get_center(obstacles[i]);
    int r = circle_get_radius(obstacles[i]);
    int dist = euclid_dist_xpoint(x, y, center);
    if (dist < r){
      draw_circles(dis, win, gc, &(obstacles[i]), 1, 0xFFFFFF);
      for (int j = i; j < *obstCount-1; j++){
        obstacles[j] = obstacles[j+1];
      }
      *obstCount = *obstCount - 1;
      break;
    }
  }
}
