#include "olive.c"

#define WIDTH  800
#define HEIGHT 600

#define PI 3.14159265359

uint32_t pixels[WIDTH*HEIGHT];

float angle = 0.0f;

int circle_x = WIDTH*2/3;
int circle_y = HEIGHT/2;
int circle_radius = 50;
int circle_dx = 100;
int circle_dy = 100;

float sqrtf(float x);
float atan2f(float y, float x);
float sinf(float x);
float cosf(float x);

void rotate_point(int *x, int *y)
{
    int dx = *x - WIDTH/2;
    int dy = *y - HEIGHT/2;
    float mag = sqrtf(dx*dx + dy*dy);
    float dir = atan2f(dy, dx) + angle;
    *x = cosf(dir)*mag + WIDTH/2; 
    *y = sinf(dir)*mag + HEIGHT/2;
}

uint32_t *render(float dt)
{

    // triangle
    olivec_fill(pixels,  WIDTH, HEIGHT, 0xFF181818);
    {
	angle += 0.5*PI*dt;
	int x1 = WIDTH/2,   y1 = HEIGHT/8;
	int x2 = WIDTH/8,   y2 = HEIGHT/2;
	int x3 = WIDTH*7/8, y3 = HEIGHT*7/8;
	rotate_point(&x1,&y1);
	rotate_point(&x2,&y2);
	rotate_point(&x3,&y3);
	olivec_fill_triangle(pixels, WIDTH, HEIGHT, x1,y1,x2,y2,x3,y3, 0xFF2020FF);
    }

    // circle
    {
	int x = circle_x + circle_dx*dt;
	if (x-circle_radius < 0 || x+circle_radius >= WIDTH) {
	    circle_dx *= -1;
	} else {
	    circle_x = x;
	}
	int y = circle_y + circle_dy*dt;
	if (y-circle_radius < 0 || y+circle_radius >= HEIGHT) {
	    circle_dy *= -1;
	} else {
	    circle_y = y;
	}
	olivec_fill_circle(pixels, WIDTH, HEIGHT, circle_x, circle_y, circle_radius, 0xAAFF2020);
    }
    return pixels;
}
