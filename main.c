#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/extensions/Xdbe.h>

/* include some silly stuff */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>

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


void* movingBlocks(void *args){
  const int LEN = 100;
  /*XPoint blocks[LEN];*/
  XArc blocks[LEN];
  int speeds[LEN];
  const int radius = 10;
  struct bufAttr bufattr = * (struct bufAttr*) args;
  XWindowAttributes attr = bufattr.attr;
  /*XdbeBackBuffer buf = bufattr.buf;*/

  /*Status st = XGetWindowAttributes(dis, win, &attr);*/
  /*printf("%d\n", attr.width);*/
  /*printf("%d\n", attr.height);*/

  for (int i = 0; i < LEN; i++){
    /*XPoint xy;*/
    XArc xy;
    xy.x = rand() % attr.width;
    xy.y = rand() % attr.height;
    xy.width = radius;
    xy.height = radius;
    xy.angle1 = 0;
    xy.angle2 = 360*64;
    blocks[i] = xy;

    speeds[i] = rand() % 5 + 1;
  }

  while(1){
    usleep(5000);
    XLockDisplay(dis);
    XSetForeground(dis, gc, 0xFFFFFF);
    XFillArcs(dis, win, gc, blocks, LEN);
    for(int i = 0; i < LEN; i++){
      int prevY = blocks[i].y;
      /*blocks[i].y = (blocks[i].y + speeds[i]) % attr.height;*/
      int newY = blocks[i].y + speeds[i];
      if (newY >= attr.height){ // Probably place this after collision detection
        blocks[i].y = 0;
        blocks[i].x = rand() % attr.width;
      }
      else{
        int collided = 0;
        for (int j = 0; j < obstCount; j++){
          int nextDist = sqrt(pow(obstacles[j].x - (blocks[i].x + 5), 2) + pow(obstacles[j].y - (newY + 5), 2));
          int overlapDist = obstacles[j].height/2 + blocks[i].height/2;
          /*printf("Dist: %d | Overlapdist: %d\n", nextDist, overlapDist);*/
          if (nextDist < overlapDist){
            // Intersects
            if (obstacles[j].x > blocks[i].x + 5){
              blocks[i].x -= speeds[i];
            }
            else{
              blocks[i].x += speeds[i];
            }
            collided = 1;
            break;
          }
        }
        if (!collided){
          blocks[i].y = newY;
        }
      }
    }

    usleep(10000);
    XSetForeground(dis, gc, 0x0000FF);
    XFillArcs(dis, win, gc, blocks, LEN);
    XSetForeground(dis, gc, 0xFFFFFF);
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
  int button_holding = 0;
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

    XLockDisplay(dis);
    if (event.type==Expose && event.xexpose.count==0) {
      /* the window was exposed redraw it! */
      redraw();
      if (isFirst){ // Start rain thread first iteration.
        // This stupid first redraw only happens because of i3 resizing the window.
        // Would probably have to be adapted to fit other WM's.
        XGetWindowAttributes(dis, win, &attr);
        pthread_create(&thread_id, NULL, movingBlocks, &attr);
        isFirst = 0;
      }
    }
    else if (event.type==KeyPress&&
        XLookupString(&event.xkey,text,255,&key,0)==1) {
      if (text[0]=='q') {
        close_x();
      }
      else if (text[0] == 'c'){
        XClearWindow(dis, win);
      }
      else{
        printf("You pressed the %c key!\n",text[0]);
      }
    }
    else if (event.type==ButtonPress) {
      /* tell where the mouse Button was Pressed */
      int x=event.xbutton.x,
          y=event.xbutton.y;

      // strcpy(text,"X is FUN!");
      // XSetForeground(dis,gc,rand()%event.xbutton.x%255);
      // XDrawString(dis,win,gc,x,y, text, strlen(text));
      // printf("DRAWING\n");
      if (event.xbutton.button == Button1){
        const int radius = 200;
        XSetForeground(dis, gc, 0x00FF00);
        XFillArc(dis, win, gc, x-(radius/2), y-(radius/2), radius, radius, 0, 360*64);
        XSetForeground(dis, gc, 0x000000);
        /*XDrawPoint(dis, win, gc, x, y);*/
        /*XDrawPoint(dis, win, gc, x+25, y+25);*/
        XArc obst;
        obst.x = x;
        obst.y = y;
        obst.width = radius;
        obst.height = radius;
        obst.angle1 = 0;
        obst.angle2 = 360*64;
        obstacles[obstCount] = obst;
        // XFillRectangle(dis, win, gc, x, y, 500, 500);
        button_holding = 1;
        obstCount++;
      }
      else if (event.xbutton.button == Button3){
        XClearArea(dis, win, x, y, 50, 50, 0);
        button_holding = 3;
      }
       /*XDrawImageString(dis, win, gc, x, y, "HELLO", 5);*/
    }
    else if (event.type == ButtonRelease){
      button_holding = 0;
      // if (event.xbutton.button == Button1){
      //   printf("Released\n");
      //   button_holding = 0;
      // }
    }
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
