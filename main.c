#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/extensions/Xdbe.h>
#include <X11/Xft/Xft.h>
#include <X11/extensions/Xrender.h>
#include <X11/Xcms.h>

/* include some silly stuff */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>

#include "collision.h"

/* here are our X variables */
Display *dis;
int screen;
Window win;
GC gc;

pthread_mutex_t lock;

XArc obstacles[100];
unsigned int obstCount = 0;

/* here are our X routines declared! */
void init_x();
void close_x();
void redraw();

struct bufAttr{
  XWindowAttributes attr;
  XdbeBackBuffer buf;
};


// Returns if it printed the fps, meaning a second had passed and variables should reset.
int printFPS(Display* dis, Window win, GC gc, time_t curr, time_t prev, int frameCounter, int prevFPS){
  int retvalue = 0;
  int fps;
  if (curr > prev){
    int diff = curr - prev;
    fps = frameCounter / diff;
    retvalue = fps;
  }
  else{
    fps = prevFPS;
  }
  char str[10] = {0};
  snprintf(str, 10, "%d", fps);
  for (int i = 0; i < 10; i++){
    if (str[i] == 0){
      str[i] = ' ';
    }
  }
  XGCValues values;
  values.font = XLoadFont(dis, "lucidasans-bold-18");
  GC str_gc = XCreateGC(dis, win, 1L<<14, &values);
  XSetForeground(dis, gc, 0x4c594b);
  XFillRectangle(dis, win, gc, 10, 10, 100, 40);
  XSetForeground(dis, str_gc, 0x319e28);
  XDrawString(dis, win, str_gc, 15, 40, str, 10);
  return retvalue;
}


void* movingBlocks(void *args){
  const int LEN = 100;
  /*XPoint balls[LEN];*/
  XArc balls[LEN];
  int prevXs[LEN];
  Direction dirs[LEN];
  struct bufAttr bufattr = * (struct bufAttr*) args;
  XWindowAttributes attr = bufattr.attr;
  /*XdbeBackBuffer buf = bufattr.buf;*/

  /*Status st = XGetWindowAttributes(dis, win, &attr);*/
  /*printf("%d\n", attr.width);*/
  /*printf("%d\n", attr.height);*/

  for (int i = 0; i < LEN; i++){
    balls[i] = circle_create(rand() % attr.width, rand() % attr.height, 10);
    dirs[i].y = rand() % 5 + 1;
    dirs[i].x = 0;
    prevXs[i] = -1;
  }

  time_t prev = time(0);
  time_t curr; 
  int frameCounter = 0;
  int prevFPS = 0;

  while(1){
    usleep(10000); // Maybe make frames not dependent on fps
    frameCounter++;
    XLockDisplay(dis);
    draw_circles(dis, win, gc, balls, LEN, 0xFFFFFF); // Clear previous balls

    // Update ball positions
    for(int i = 0; i < LEN; i++){
      int newY = balls[i].y + dirs[i].y;
      if (newY >= attr.height){ // Probably place this after collision detection
        balls[i].y = 0;
        balls[i].x = rand() % attr.width;
      }
      else{
        int collided = 0;
        for (int j = 0; j < obstCount; j++){
          XPoint center1 = circle_get_center(obstacles[j]);
          int r1 = circle_get_radius(obstacles[j]);
          XPoint center2 = circle_get_center(balls[i]);
          int r2 = circle_get_radius(balls[i]);
          int nextDist = euclid_dist_xpoint2(center1, center2);
          if (nextDist < r1 + r2){
            // Intersects
            if (center1.x > center2.x){
              if (balls[i].x - dirs[i].y == prevXs[i]){
                collided = 1; // Keep it still when stuck
                break;
              }
              else{
                prevXs[i] = balls[i].x;
                balls[i].x -= dirs[i].y;
              }
            }
            else{
              if (balls[i].x + dirs[i].y == prevXs[i]){
                collided = 1; // Keep it still when stuck
                break;
              }
              else{
                prevXs[i] = balls[i].x;
                balls[i].x += dirs[i].y;
              }
            }
            collided = 1;
            break;
          }
        }
        if (!collided){
          balls[i].y = newY;
        }
      }
    }


    // Draw new balls
    draw_circles(dis, win, gc, balls, LEN, 0x0000FF);
    draw_circles(dis, win, gc, obstacles, obstCount, 0x00FF00);

    /*usleep(10000);*/
    /*XSetForeground(dis, gc, 0x0000FF);*/
    /*XFillArcs(dis, win, gc, balls, LEN);*/
    /*XSetForeground(dis, gc, 0xFFFFFF);*/

    curr = time(0);
    int res = printFPS(dis, win, gc, curr, prev, frameCounter, prevFPS);
    if (res != 0){
      frameCounter = 0;
      prev = curr;
      prevFPS = res;
    }

    XFlush(dis);
    XUnlockDisplay(dis);
  }

  

  return NULL;
}

int main() {

  pthread_t thread_id;
  pthread_mutex_init(&lock, NULL);

  XEvent event;		/* the XEvent declaration !!! */
  KeySym key;		/* a dealie-bob to handle KeyPress Events */	
  char text[255];		/* a char buffer for KeyPress Events */
  /*int button_holding = 0;*/
  int isFirst = 1;
  XInitThreads();

  init_x();
  XWindowAttributes attr;
  XGetWindowAttributes(dis, win, &attr);

  /* look for events forever... */
  while(1) {		
    /* get the next event and stuff it into our event variable.
    Note:  only events we set the mask for are detected!
    */
    XNextEvent(dis, &event);

    /*if (isFirst){*/
      /*redraw();*/
      /*XGetWindowAttributes(dis, win, &attr);*/
      /*XdbeBackBuffer buf = XdbeAllocateBackBufferName(dis, win, XdbeBackground);*/
      /*struct bufAttr info;*/
      /*info.attr = attr;*/
      /*info.buf = buf;*/
      /*pthread_create(&thread_id, NULL, movingBlocks, &info);*/
      /*isFirst = 0;*/
    /*}*/
    /*continue;*/
    ////////////////////////////////////////////////////////////////////

    if (event.type==Expose && event.xexpose.count==0) {
      XLockDisplay(dis);
      /* the window was exposed redraw it! */
      redraw();
      if (isFirst){ // Start rain thread first iteration.
        // This stupid first redraw only happens because of i3 resizing the window.
        // Would probably have to be adapted to fit other WM's.
        XGetWindowAttributes(dis, win, &attr);
        pthread_create(&thread_id, NULL, movingBlocks, &attr);
        isFirst = 0;
      }
      XUnlockDisplay(dis);
    }
    else if (event.type==KeyPress && XLookupString(&event.xkey,text,255,&key,0)==1) {
      XLockDisplay(dis);
      if (text[0]=='q') {
        close_x();
      }
      else if (text[0] == 'c'){
        XClearWindow(dis, win);
      }
      else{
        printf("You pressed the %c key!\n",text[0]);
      }
      XUnlockDisplay(dis);
    }
    else if (event.type==ButtonPress) {
      XLockDisplay(dis);
      /* tell where the mouse Button was Pressed */
      int x=event.xbutton.x,
          y=event.xbutton.y;

      /*char text[20];*/
      /*strcpy(text,"X is FUN!");*/
      // XSetForeground(dis,gc,rand()%event.xbutton.x%255);
      /*XDrawString(dis,win,gc,x,y, text, strlen(text));*/
      // printf("DRAWING\n");
      if (event.xbutton.button == Button1){
        /*XSetForeground(dis, gc, 0x00FF00);*/
        /*XFillArc(dis, win, gc, x-(radius/2), y-(radius/2), radius, radius, 0, 360*64);*/
        /*XSetForeground(dis, gc, 0x000000);*/
        /*XDrawPoint(dis, win, gc, x, y);*/
        /*XDrawPoint(dis, win, gc, x+25, y+25);*/
        XArc obst = circle_create(x, y, 100);
        obstacles[obstCount] = obst;
        // XFillRectangle(dis, win, gc, x, y, 500, 500);
        /*button_holding = 1;*/
        obstCount++;
      }
      else if (event.xbutton.button == Button3){
        /*XClearArea(dis, win, x, y, 50, 50, 0);*/
        /*button_holding = 3;*/
        for (int i = 0; i < obstCount; i++){
          XPoint center = circle_get_center(obstacles[i]);
          int r = circle_get_radius(obstacles[i]);
          int dist = euclid_dist_xpoint(x, y, center);
          if (dist < r){
            /*XSetForeground(dis, gc, 0xFFFFFF);*/
            /*XFillArc(dis, win, gc, obstacles[i].x-(radius/2), obstacles[i].y-(radius/2), radius, radius, 0, 360*64);*/
            draw_circles(dis, win, gc, &(obstacles[i]), 1, 0xFFFFFF);
            for (int j = i; j < obstCount-1; j++){
              obstacles[j] = obstacles[j+1];
            }
            obstCount--;
            break;
          }
        }
      }
       /*XDrawImageString(dis, win, gc, x, y, "HELLO", 5);*/
      XUnlockDisplay(dis);
    }
    /*else if (event.type == ButtonRelease){*/
      /*button_holding = 0;*/
      /*// if (event.xbutton.button == Button1){*/
      /*//   printf("Released\n");*/
      /*//   button_holding = 0;*/
      /*// }*/
    /*}*/
    /*else if (event.type == MotionNotify){*/
      /*int x=event.xbutton.x,*/
          /*y=event.xbutton.y;*/
      /*if (button_holding == 1){*/
        /*XDrawArc(dis, win, gc, x-25, y-25, 50, 50, 0, 360*64);*/
      /*}*/
      /*if (button_holding == 3){*/
        /*XClearArea(dis, win, x-25, y-25, 50, 50, 0);*/
      /*}*/

    /*}*/
    XUnlockDisplay(dis);

  }
}

void init_x() {
  /* get the colors black and white (see section for details) */        
  unsigned long black,white;

  dis=XOpenDisplay((char *)0);
  screen=DefaultScreen(dis);
  black=BlackPixel(dis,screen),
  white=WhitePixel(dis, screen);
  win=XCreateSimpleWindow(dis,DefaultRootWindow(dis), 0, 0,	300, 300, 5, black, white);
  XSetStandardProperties(dis,win,"Howdy","Hi",None,NULL,0,NULL);
  XSelectInput(dis, win, ExposureMask|ButtonPressMask|KeyPressMask|ButtonMotionMask|ButtonReleaseMask);
  gc=XCreateGC(dis, win, 0,0);        
  XSetBackground(dis,gc,white);
  XSetForeground(dis,gc,black);
  XClearWindow(dis, win);
  XMapRaised(dis, win);
};

void close_x() {
  XFreeGC(dis, gc);
  XDestroyWindow(dis,win);
  XCloseDisplay(dis);	
  exit(1);				
};

void redraw() {
  XClearWindow(dis, win);
};
