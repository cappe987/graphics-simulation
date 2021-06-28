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

#include "physics.h"
#include "fps.h"
#include "circle.h"

/* here are our X variables */
Display *dis;
int screen;
Window win;
GC gc;


Circle obstacles[100];
unsigned int obstCount = 0;

/* here are our X routines declared! */
void init_x();
void close_x();
void redraw();

struct bufAttr{
  XWindowAttributes attr;
  XdbeBackBuffer buf;
};




void* game_loop(void *args){
  const int LEN = 100;
  Circle balls[LEN];
  /*int prevXs[LEN]; // Used to make balls not wobble when stuck. Refactor this.*/
  Velocity vel[LEN];
  struct bufAttr bufattr = * (struct bufAttr*) args;
  XWindowAttributes attr = bufattr.attr;
  XdbeBackBuffer buf = bufattr.buf;

  for (int i = 0; i < LEN; i++){
    vel[i].y = rand() % 5 + 5;
    vel[i].x = 0;
    balls[i] = circle_create(rand() % attr.width, rand() % attr.height, vel[i].x, vel[i].y, 10);
    /*prevXs[i] = -1;*/
  }

  init_fps_counter();
  init_game_time();

  XdbeSwapInfo info;
  info.swap_window = win;
  info.swap_action = XdbeBackground;

  while(1){
    usleep(10000); // Maybe make frames not dependent on fps
    XLockDisplay(dis);
    draw_circles(dis, buf, gc, balls, LEN, 0xFFFFFF); // Clear previous balls

    float milli_diff = get_millisecond_diff();

    // Update ball positions
    for(int i = 0; i < LEN; i++){
      /*int newY = balls[i].y + (vel[i].y * milli_diff);*/
      /*if (newY >= attr.height){ // Probably place this after collision detection*/
        /*balls[i].y = 0;*/
        /*balls[i].x = rand() % attr.width;*/
      /*}*/
      /*else{*/
      int collided = 0;
      for (int j = 0; j < obstCount; j++){
        Circle obst = obstacles[j];
        Circle ball = balls[i];
        if (obst_ball_collision(obst, &(balls[i]))){
          collided = 1;
          break;
        }
      }
      if (!collided){
        /*balls[i].y = newY;*/
        update_velocity(&(balls[i]));
      }
      update_position(&(balls[i]), milli_diff, attr.width, attr.height);
      /*update_position(&(balls[i]));*/
      
    }


    // Draw new balls
    draw_circles(dis, buf, gc, balls, LEN, 0x0000FF);
    draw_circles(dis, buf, gc, obstacles, obstCount, 0x00FF00);

    print_fps(dis, buf, gc);
    
    XdbeSwapBuffers(dis, &info, 1);
    XFlush(dis);
    XUnlockDisplay(dis);
  }

  

  return NULL;
}

int main() {

  pthread_t thread_id;

  XEvent event;		/* the XEvent declaration !!! */
  KeySym key;		/* a dealie-bob to handle KeyPress Events */	
  char text[255];		/* a char buffer for KeyPress Events */
  int isFirst = 1;
  XInitThreads();

  init_x();
  struct bufAttr attr;

  /* look for events forever... */
  while(1) {		
    /* get the next event and stuff it into our event variable.
    Note:  only events we set the mask for are detected!  */
    XNextEvent(dis, &event);

    if (event.type==Expose && event.xexpose.count==0) {
      XLockDisplay(dis);
      /* the window was exposed redraw it! */
      redraw();
      if (isFirst){ // Start rain thread first iteration.
        // This stupid first redraw only happens because of i3 resizing the window.
        // Would probably have to be adapted to fit other WM's.
        XGetWindowAttributes(dis, win, &attr.attr);
        attr.buf = XdbeAllocateBackBufferName(dis, win, XdbeBackground);
        pthread_create(&thread_id, NULL, game_loop, &attr);
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
      int x = event.xbutton.x;
      int y = event.xbutton.y;

      if (event.xbutton.button == Button1){ // Left click
        add_green(x, y, obstacles, &obstCount);
      }
      else if (event.xbutton.button == Button3){ // Right click 
        remove_green(dis, win, gc, x, y, obstacles, &obstCount);
      }
      XUnlockDisplay(dis);
    }
    XUnlockDisplay(dis);
  }
}

void init_x() {
  /* get the colors black and white (see section for details) */        
  unsigned long black,white;

  dis    = XOpenDisplay((char *)0);
  screen = DefaultScreen(dis);
  black  = BlackPixel(dis, screen),
  white  = WhitePixel(dis, screen);
  win    = XCreateSimpleWindow(dis, DefaultRootWindow(dis), 0, 0,	300, 300, 5, black, white);
  XSetStandardProperties(dis, win, "Howdy", "Hi", None, NULL, 0, NULL);
  // Set masks for what events to detect
  XSelectInput(dis, win, ExposureMask|ButtonPressMask|KeyPressMask|ButtonMotionMask|ButtonReleaseMask);
  gc = XCreateGC(dis, win, 0,0);        
  XSetBackground(dis, gc, white);
  XSetForeground(dis, gc, black);
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
