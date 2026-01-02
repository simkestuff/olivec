#include <stdio.h>
#include <math.h>
#include "raylib.h"
#include "olive.c"

#define WIDTH 800
#define HEIGHT 600

#define BACKGROUND CLITERAL(Color){24, 24, 24, 255 }

uint32_t pixels[WIDTH*HEIGHT];

float angle = 0.0f;

void rotate_point(int *x, int *y);

int main(void)
{
    InitWindow(WIDTH, HEIGHT, "raylib olivec demo");

    Image img = {
	.data = pixels,
	.width = WIDTH,
	.height = HEIGHT,
	.mipmaps = 1,
	.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8
    };

    Texture2D tx = LoadTextureFromImage(img);

    SetTargetFPS(60);
    
    int circle_dx = 100;
    int circle_dy = 100;
    int circle_x = WIDTH*2/3;
    int circle_y = HEIGHT/2;
    int circle_radius = 50;
	
    while (!WindowShouldClose()) {
	int x1 = WIDTH/2,   y1 = HEIGHT/8;
	int x2 = WIDTH/8,   y2 = HEIGHT/2;
	int x3 = WIDTH*7/8, y3 = HEIGHT*7/8;
	
	// update
	olivec_fill(pixels,  WIDTH, HEIGHT, 0xFF181818);
	
	angle += 0.5*M_PI*GetFrameTime();
	int x = circle_x + circle_dx*GetFrameTime();
	int y = circle_y + circle_dy*GetFrameTime();

	rotate_point(&x1, &y1);
	rotate_point(&x2, &y2);
	rotate_point(&x3, &y3);

	if (x-circle_radius < 0 || x+circle_radius >= WIDTH) circle_dx *= -1;
	else circle_x = x;
	if (y-circle_radius < 0 || y+circle_radius >= HEIGHT) circle_dy *= -1;
	else circle_y = y;
	
	olivec_fill_triangle(pixels, WIDTH, HEIGHT, x1,y1,x2,y2,x3,y3, 0xFF2020FF);
	olivec_fill_circle(pixels, WIDTH, HEIGHT, circle_x, circle_y, circle_radius, 0xAAFF2020);
	
	UpdateTexture(tx, pixels);

	// draw
	BeginDrawing();

	ClearBackground(BACKGROUND);

	DrawTexture(tx, 0, 0, WHITE);
	
	EndDrawing();
    }

    CloseWindow();
    
    return 0;
}

void rotate_point(int *x, int *y)
{
    int dx = *x - WIDTH/2;
    int dy = *y - HEIGHT/2;
    float mag = sqrtf(dx*dx + dy*dy);
    float dir = atan2f(dy, dx) + angle;
    *x = cosf(dir)*mag + WIDTH/2; 
    *y = sinf(dir)*mag + HEIGHT/2;
}
