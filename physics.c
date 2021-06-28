#include <math.h>
#include <stdio.h>

#include "physics.h"

#define PI 3.1415


int euclid_dist_circle(int x1, int y1, Circle c){
  return sqrt(pow(x1 - c.x, 2) + pow(y1 - c.y,2));
}

int euclid_dist_circles(Circle c1, Circle c2){
  return sqrt(pow(c1.x - c2.x, 2) + pow(c1.y - c2.y, 2));
}


//physics - calculating the aerodynamic forces to drag
// -0.5 * Cd * A * v^2 * rho
/*var fx = -0.5 * drag.value * density.value * balls[i].area * balls[i].velocity.x * balls[i].velocity.x * (balls[i].velocity.x / Math.abs(balls[i].velocity.x));*/
/*var fy = -0.5 * drag.value * density.value * balls[i].area * balls[i].velocity.y * balls[i].velocity.y * (balls[i].velocity.y / Math.abs(balls[i].velocity.y));*/

/*fx = (isNaN(fx)? 0 : fx);*/
/*fy = (isNaN(fy)? 0 : fy);*/
/*console.log(fx);*/
/*//Calculating the accleration of the ball*/
/*//F = ma or a = F/m*/
/*var ax = fx / balls[i].mass;*/
/*var ay = (ag * gravity.value) + (fy / balls[i].mass);*/

/*//Calculating the ball velocity */
/*balls[i].velocity.x += ax * fps;*/
/*balls[i].velocity.y += ay * fps;*/

/*//Calculating the position of the ball*/
/*balls[i].position.x += balls[i].velocity.x * fps * 100;*/
/*balls[i].position.y += balls[i].velocity.y * fps * 100;*/

void update_velocity(Circle* ball){
  float drag = 0.2;
  float density = 0.1;
  float gravity = 0.01;
  /*const int fps = 60;*/
  const float ag = 9.81;
  float area = (PI * ball->radius * ball->radius)/1000;
  float fx = -1 * drag * density * area * ball->vx * ball->vx * (ball->vx / fabs(ball->vx));
  float fy = -1 * drag * density * area * ball->vy * ball->vy * (ball->vy / fabs(ball->vy));

  fx = isnan(fx) ? 0 : fx; 
  fy = isnan(fy) ? 0 : fy; 

  float ax = fx / ball->radius;
  float ay = (ag * gravity) + (fy / ball->radius);



  ball->vx += ax;
  ball->vy += ay;

  if (!isfinite(ball->vy)){
    ball->vy = 5;
    ball->y = 0;
  }
  if (!isfinite(ball->vx)){
    ball->vx = 5;
    ball->x = 50;
  }
}

void update_position(Circle* ball, float milli_diff, int width, int height){
  ball->x = ball->x + (ball->vx * milli_diff);
  ball->y = ball->y + (ball->vy * milli_diff);
  float deltax = ball->x + (ball->vx * milli_diff);
  float deltay = ball->y + (ball->vy * milli_diff);

  if (ball->x > width){
    ball->x = ball->x - width;
  }
  else if (ball->x < 0){
    ball->x = ball->x + width;
  }
  if (ball->y > height){
    ball->y = ball->y - height;
  }
  else if (ball->y < 0){
    ball->y = ball->y + height;
  }
}



// Update velocity and post accordingly
int obst_ball_collision(Circle obst, Circle* ball){
  int nextDist = euclid_dist_circles(*ball, obst);
  if (nextDist >= obst.radius + ball->radius){
    return 0; // Doesn't collide
  }

  // Collided
  /*ball->vy = -0.9 * ball->vy;*/
  /*ball->y = ball->y - (obst.radius + ball->radius - nextDist) + 1;*/

  float xdiff = ball->x - obst.x;
  float ydiff = ball->y - obst.y;
  if (xdiff > 0){ // Ball to right of obst
    if (ydiff <= 0){ // Ball above obst
      ball->x = obst.x + xdiff + 1;
      ball->y = obst.y + ydiff - 1;
      ball->vx = 0.9 * ball->vy;// + (xdiff/90);
      ball->vy = -0.9 * ball->vx;// + (ydiff/90);
    }
    else{ // Ball under obst
      ball->x = obst.x + xdiff + 1;
      ball->y = obst.y + ydiff + 1;
      ball->vx = -0.9 * ball->vy;// + (xdiff/90);
      ball->vy = 0.9 * ball->vx;// + (ydiff/90);
    }
  }
  else{ // Ball to left of obst
    if (ydiff <= 0){ // Ball above obst
      ball->x = obst.x + xdiff - 1;
      ball->y = obst.y + ydiff - 1;
      ball->vx = -0.9 * ball->vy;// + (xdiff/90);
      ball->vy = 0.9 * ball->vx;// + (ydiff/90);
    }
    else{ // Ball under obst
      ball->x = obst.x + xdiff - 1;
      ball->y = obst.y + ydiff + 1;
      ball->vx = 0.9 * ball->vy;// + (xdiff/90);
      ball->vy = -0.9 * ball->vx;// + (ydiff/90);
    }

  }

  return 1;
}
