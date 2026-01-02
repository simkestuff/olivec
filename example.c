#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

#include "olive.c"
//#include "utils.c" // zamjenjeno sa stb library-em

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define WIDTH 800
#define HEIGHT 600

#define COLS 8
#define ROWS 6
#define CELL_WIDTH  (WIDTH/COLS)
#define CELL_HEIGHT (HEIGHT/ROWS)

#define BACKGROUND_COLOR 0xFF202020
#define FOREGROUND_COLOR 0xFF2020FF
static uint32_t pixels[WIDTH * HEIGHT];

#define PICS "pics/"

bool checker_example(void)
{
    olivec_fill(pixels, WIDTH, HEIGHT, BACKGROUND_COLOR);
    for (int y = 0; y < ROWS; y++) {
	for (int x = 0; x < COLS; x++) {
	    uint32_t color;
	    if ((x + y) % 2 == 0) {
		color = BACKGROUND_COLOR;
	    } else {
		color = 0xFF2020FF;
	    }
	    olivec_fill_rect(pixels, WIDTH, HEIGHT,
			     x*CELL_WIDTH,
			     y*CELL_HEIGHT,
			     CELL_WIDTH,
			     CELL_HEIGHT,
			     color);
	}
    }

     
    const char *filepath = PICS"checker.png";
    if (!stbi_write_png(filepath, WIDTH, HEIGHT, 4, pixels, WIDTH*sizeof(uint32_t))) {
	fprintf(stderr, "Couldn't save file %s: %s\n", filepath, strerror(errno));
	return false;
    }
    
    return true;
}

bool circle_example(void)
{
    olivec_fill(pixels, WIDTH, HEIGHT, 0xFFFFFFFF);
    olivec_fill_circle(pixels, WIDTH, HEIGHT, WIDTH*2/3, HEIGHT/2, 200, 0xFF1E9EF7);
    olivec_fill_circle(pixels, WIDTH, HEIGHT, WIDTH/3, HEIGHT/2, 200, 0xAA0000FF);

    const char *filepath = PICS"circle.png";
    if (!stbi_write_png(filepath, WIDTH, HEIGHT, 4, pixels, WIDTH*sizeof(uint32_t))) {
	fprintf(stderr, "Couldn't save file %s: %s\n", filepath, strerror(errno));
	return false;
    }
    
    return true;
}

bool lines_example(void)
{
    olivec_fill(pixels, WIDTH, HEIGHT, BACKGROUND_COLOR);
    olivec_draw_line(pixels, WIDTH, HEIGHT,
		     0,0,WIDTH, HEIGHT,
		     FOREGROUND_COLOR);
    olivec_draw_line(pixels, WIDTH, HEIGHT,
		     WIDTH,0,0, HEIGHT,
		     FOREGROUND_COLOR);
    olivec_draw_line(pixels, WIDTH, HEIGHT,
		     0,0, 200, HEIGHT,
		     0xFF20FF20);
    olivec_draw_line(pixels, WIDTH, HEIGHT,
		     0, HEIGHT/2, WIDTH, 0,
		     0xFF20FF20);

    const char *filepath = PICS"line.png";
    if (!stbi_write_png(filepath, WIDTH, HEIGHT, 4, pixels, WIDTH*sizeof(uint32_t))) {
	fprintf(stderr, "Couldn't save file %s: %s\n", filepath, strerror(errno));
	return false;
    }
 
    return true;
}

bool triangle_example(void)
{
    olivec_fill(pixels, WIDTH, HEIGHT, BACKGROUND_COLOR);

    olivec_fill_triangle(pixels, WIDTH, HEIGHT,
			 WIDTH*1/8, HEIGHT*2/8,
			 WIDTH*7/8, HEIGHT*2/8,
			 WIDTH*4/8, HEIGHT*7/8,
			 0xFF2020FF);
    olivec_fill_triangle(pixels, WIDTH, HEIGHT,
			 WIDTH*3/8, HEIGHT*1/8,
			 WIDTH*1/8, HEIGHT*3/8,
			 WIDTH*6/8, HEIGHT*5/8,
			 0xAA20FF20);
    olivec_fill_triangle(pixels, WIDTH, HEIGHT,
			 WIDTH*2/8, HEIGHT*3/8,
			 WIDTH*2/8, HEIGHT*5/8,
			 WIDTH*5/8, HEIGHT*5/8,
			 0xFFFF2020);

    const char *filepath = PICS"triangles.png";
    if (!stbi_write_png(filepath, WIDTH, HEIGHT, 4, pixels, WIDTH*sizeof(uint32_t))) {
	fprintf(stderr, "Couldn't save file %s: %s\n", filepath, strerror(errno));
	return false;
    }
 
    return true;
}

int main(void)
{

    if (!checker_example()) return -1;
    if (!circle_example()) return -1;
    if (!lines_example()) return -1;
    if (!triangle_example()) return -1;
       
    return 0;
}
