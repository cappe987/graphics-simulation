#include "circle.h"
#include "physics.h"




void draw_circles(Display* dis, Window win, GC gc, Circle* cs, int n, unsigned long color){
  XSetForeground(dis, gc, color);
  for (int i = 0; i < n; i++){
    int r = cs[i].radius;
    XFillArc(dis, win, gc, cs[i].x-r, cs[i].y-r, r*2, r*2, 0, 360*64);
  }
  return;
}


Circle circle_create(unsigned int x, unsigned int y, int vx, int vy, int radius){
  Circle c;
  c.x = x;
  c.y = y;
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
    /*XPoint center = circle_get_center(obstacles[i]);*/
    int r = obstacles[i].radius;
    int dist = euclid_dist_circle(x, y, obstacles[i]);
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
