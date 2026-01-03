// folowing https://youtu.be/qjWkNZ0SXfo?si=io9W39_F1vkXytOf

#include <stdio.h>
#include <math.h>
#include "raylib.h"
#include "olive.c"

#define ARRAYLEN(a) (sizeof((a))/sizeof((a)[0])) 

#define WIDTH 800
#define HEIGHT 600

uint32_t pixels[WIDTH*HEIGHT];


void render_point(int x, int y)
{
    int s = 20;
    olivec_fill_rect(pixels, WIDTH, HEIGHT, x-s/2, y-s/2, s, s, 0xFF18FF18);
    
}

void project2screen(Vector2 p, int *sx, int *sy)
{
    *sx = (p.x + 1)/2*GetScreenWidth();
    *sy = (1-(p.y + 1)/2)*GetScreenHeight();
}

Vector2 project2D(Vector3 p)
{
    Vector2 v = {
	v.x = p.x/p.z,
	v.y = p.y/p.z,
    };
    return v;	
}

Vector3 translate_z(Vector3 p, float dz)
{
    Vector3 v = {
	.x = p.x,
	.y = p.y,
	.z = p.z + dz,
    };
    return v;
}

Vector3 rotate_xz(Vector3 p, float angle)
{
    float c = cosf(angle);
    float s = sinf(angle);
    Vector3 v = {
	.x = p.x*c - p.z*s,
	.y = p.y,
	.z = p.x*s + p.z*c,
    };
    return v;
}

int main(void)
{
    InitWindow(WIDTH, HEIGHT, "cube demo");
    Image img = {
	.data    = pixels, 
	.width   = WIDTH,
	.height  = HEIGHT,
	.mipmaps = 1,
	.format  = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8	
    };
    Texture2D tex = LoadTextureFromImage(img);

    float angle = 0.0f;
    float dz = 1.0f;

    // vertices
    Vector3 vs[] = {
	{ 0.25f,  0.25f, 0.25f},
	{-0.25f,  0.25f, 0.25f},
	{-0.25f, -0.25f, 0.25f},
	{ 0.25f, -0.25f, 0.25f},

	{ 0.25f,  0.25f, -0.25f},
	{-0.25f,  0.25f, -0.25f},
	{-0.25f, -0.25f, -0.25f},
	{ 0.25f, -0.25f, -0.25f},
    };

    // faces
    int fs[][4] = {
	{0,1,2,3},
	{4,5,6,7},
	{0,4,7,3},
	{1,5,6,2},
    };
    
    SetTargetFPS(60);
		
    while (!WindowShouldClose()) {

	// update
	float dt = GetFrameTime();
	// dz += 1*dt;
	angle += 0.5*M_PI*dt;
	olivec_fill(pixels, WIDTH, HEIGHT, 0xFF181818);
	// for (size_t i = 0; i < ARRAYLEN(vs); i++) {
	//     int x=0, y=0;
	//     Vector3 v = vs[i];
	//     v = rotate_xz(v, angle);
	//     v = translate_z(v, dz);
	//     Vector2 q = project2D(v);
	//     project2screen(q, &x, &y);
	//     render_point(x,y);
	// }

	for (size_t i = 0; i < ARRAYLEN(fs); i++) {
	    for (size_t j = 0; j < 4; j++) {
		Vector3 a = vs[fs[i][j]];
		Vector3 b = vs[fs[i][(j+1)%4]];
		int x1=0, y1=0;
		a = rotate_xz(a, angle);
		a = translate_z(a, dz);
		Vector2 q = project2D(a);
		project2screen(q, &x1, &y1);
		int x2=0, y2=0;
		b = rotate_xz(b, angle);
		b = translate_z(b, dz);
		q = project2D(b);
		project2screen(q, &x2, &y2);
		olivec_draw_line(pixels, WIDTH, HEIGHT, x1, y1, x2, y2, 0xFF18FF18);
	    }
	}
	
	UpdateTexture(tex, pixels);

	// draw
	BeginDrawing();
	ClearBackground(GetColor(0x181818FF));
	DrawTexture(tex, 0, 0, WHITE);
	EndDrawing();
    }

    CloseWindow();
    
    return 0;
}
    
