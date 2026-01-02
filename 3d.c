#include "olive.c"

#define WIDTH  800
#define HEIGHT 600

#define GRID_COUNT 6
#define GRID_PAD 0.5/GRID_COUNT
#define GRID_SIZE ((GRID_COUNT - 1) * GRID_PAD)
#define CIRCLE_RADIUS 5
#define CIRCLE_COLOR 0xFF00FF00
#define Z_START 0.5

#define PI 3.14159265359

float angle = 0.0f;

uint32_t pixels[WIDTH*HEIGHT];

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
    angle += 0.25*PI*dt;
    
    olivec_fill(pixels, WIDTH, HEIGHT, 0xFF181818);

    for (size_t iy = 0; iy < GRID_COUNT; iy++) {
	for (size_t ix = 0; ix < GRID_COUNT; ix++) {
	    for (size_t iz = 0; iz < GRID_COUNT; iz++) {
		float x = -GRID_SIZE/2 + (ix*GRID_PAD);
		float y = -GRID_SIZE/2 + (iy*GRID_PAD);
		float z = Z_START + iz*GRID_PAD;

		float cx = 0.0;
		float cz = Z_START + GRID_SIZE/2;

		float dx = x - cx;
		float dz = z - cz;

		float a = atan2f(dz, dx);
		float m = sqrtf(dx*dx + dz*dz);

		dx = cosf(a + angle) * m;
		dz = sinf(a + angle) * m;
		x = dx + cx;
		z = dz + cz;
		
		x /= z;
		y /= z;
		olivec_fill_circle(pixels, WIDTH, HEIGHT, (x+1)/2*WIDTH, (y+1)/2*HEIGHT, CIRCLE_RADIUS, CIRCLE_COLOR);
	    }
	}
    }
    
    return pixels;
}
