#ifndef OLIVE_C_
#define OLIVE_C_

#include <stdint.h>
#include <stddef.h>


// 0xFFBBGGRR
#define RED_COMP(color)   ((color)>>(8*0)&0xFF)
#define GREEN_COMP(color) ((color)>>(8*1)&0xFF)
#define BLUE_COMP(color)  ((color)>>(8*2)&0xFF)
#define ALPHA_COMP(color) ((color)>>(8*3)&0xFF)

void olivec_fill(uint32_t *pixels, size_t width, size_t height, uint32_t color)
{
    for (size_t i = 0; i < width*height; i++) {
	pixels[i] = color;
    }
}


void olivec_alpha_blend(uint32_t *c1, uint32_t c2)
{
    uint8_t r1 = RED_COMP(*c1);
    uint8_t g1 = GREEN_COMP(*c1);
    uint8_t b1 = BLUE_COMP(*c1);
    float a1 = ALPHA_COMP(*c1)/255.0f;

    uint8_t r2 = RED_COMP(c2);
    uint8_t g2 = GREEN_COMP(c2);
    uint8_t b2 = BLUE_COMP(c2);
    float a2 = ALPHA_COMP(c2)/255.0f;

    float alpha = a2 + (float)(a1*(1.0f-a2));
    uint8_t r = (r2*a2 + r1*a1*(1-a2))/alpha;
    uint8_t g = (g2*a2 + g1*a1*(1-a2))/alpha;
    uint8_t b = (b2*a2 + b1*a1*(1-a2))/alpha;

    uint32_t a = alpha * 255;
    
    *c1 = (((0x00000000|(a<<(8*3)))|(b<<(8*2)))|(g<<(8*1)))|(r<<(8*0));
}

void olivec_fill_rect(uint32_t *pixels, size_t pixels_width, size_t pixels_height,
		      int x0, int y0, size_t w, size_t h,
		      uint32_t color)
{
    for (int dy = 0; dy < (int) h; dy++) {
	int y = y0 + dy;
	if (y >= 0 && y < (int) pixels_height) {
	    for (int dx = 0; dx < (int) w; dx++) {
		int x = x0 + dx;
		if (x >= 0 && x < (int) pixels_width) {
		    olivec_alpha_blend(&pixels[y*pixels_width + x], color);
		    //pixels[y*pixels_width + x] = color;
		}
	    }
	}
    }
}


void olivec_fill_circle(uint32_t *pixels, size_t pixels_width, size_t pixels_height,
			int cx, int cy, size_t r,
			uint32_t color)
{
    int x1 = cx - (int) r;
    int x2 = cx + (int) r;
    int y1 = cy - (int) r;
    int y2 = cy + (int) r;
    for (int y = y1; y <= y2; y++) {
	if (y >= 0 && y < (int) pixels_height) {
	    for (int x = x1; x <= x2; x++) {
		if (x >= 0 && x < (int) pixels_width) {
		    int dx = x - cx;
		    int dy = y - cy;
		    if (dx*dx + dy*dy <= (int) (r*r)) {
			//pixels[y*pixels_width + x] = color;
			olivec_alpha_blend(&pixels[y*pixels_width + x], color);
		    }
		}
	    }
	}
    }
}

#define olivec_swap(T,a,b) do { T tmp = (a); (a) = (b); (b) = tmp; } while (0)

void olivec_draw_line(uint32_t *pixels, size_t pixels_width, size_t pixels_height,
		      int x1, int y1, int x2, int y2,
		      uint32_t color)
{
    int dx = x2 - x1;
    int dy = y2 - y1;
    if (dx != 0) {
	int c = y1 - dy*x1/dx;
	if (x1 > x2) olivec_swap(int, x1, x2);
	for (int x = x1; x < x2; x++) {
	    if (x >= 0 && x < (int) pixels_width) {
		int sy1 = dy*x/dx + c;
		int sy2 = dy*(x+1)/dx + c;
		if (sy1 > sy2) olivec_swap(int, sy1, sy2);
		for (int y = sy1; y <= sy2; y++) {
		    if (y >= 0 && y < (int) pixels_height) {
			//pixels[y*pixels_width + x] = color;
			olivec_alpha_blend(&pixels[y*pixels_width + x], color);
		    }
		}
	    }
	}
    } else {
	int x = x1;
	if (x >= 0 && x < (int) pixels_width) {
	    if (y1 > y2) olivec_swap(int, y1, y2);
	    for (int y = y1; y <= y2; y++) {
		if (y >= 0 && y < (int) pixels_height) {
		    olivec_alpha_blend(&pixels[y*pixels_width + x], color);
		    //pixels[y*pixels_width + x] = color;
		}
	    }
	}
    }
	
}

void olivec_sort_triangle_points_by_y(int *x1, int *y1, int *x2, int *y2, int *x3, int *y3)
{
    // ručni bubble sort
    if (*y1 > *y2) {
	olivec_swap(int, *y1, *y2);
	olivec_swap(int, *x1, *x2);
    }

    if (*y2 > *y3) {
	olivec_swap(int, *y2, *y3);
	olivec_swap(int, *x2, *x3);
    }

    if (*y1 > *y2) {
	olivec_swap(int, *y1, *y2);
	olivec_swap(int, *x1, *x2);
    }

}

void olivec_fill_triangle(uint32_t *pixels, size_t pixels_width, size_t pixels_height,
			  int x1, int y1,
			  int x2, int y2,
			  int x3, int y3,
			  uint32_t color)
{
    // uvijek će biti y1 < y2 < y3
    olivec_sort_triangle_points_by_y(&x1, &y1, &x2, &y2, &x3, &y3);

    if (y1 != y2) { 
	for (int y = y1; y < y2; y++) {
	    int x12 = (y-y2) * (x1-x2)/(y1-y2) + x2;
	    int x13 = (y-y3) * (x1-x3)/(y1-y3) + x3;
	    if (x12 > x13) olivec_swap(int, x12, x13);
	    for (int x = x12; x <= x13; x++) {
		if (x >= 0 && x < (int) pixels_width && y >= 0 && y < (int) pixels_height) {
		    olivec_alpha_blend(&pixels[y*pixels_width + x], color);
		    //pixels[y*pixels_width + x] = color;
		}     
	    }
	}
    }

    if (y2 != y3) { 
	for (int y = y2; y <= y3; y++) {
	    int x23 = (y-y3) * (x2-x3)/(y2-y3) + x3;
	    int x13 = (y-y3) * (x1-x3)/(y1-y3) + x3;
	    if (x23 > x13) olivec_swap(int, x23, x13);
	    for (int x = x23; x <= x13; x++) {
		if (x >= 0 && x < (int) pixels_width && y >= 0 && y < (int) pixels_height) {
		    olivec_alpha_blend(&pixels[y*pixels_width + x],color);
		    //pixels[y*pixels_width + x] = color;
		}
	    }
	}
 
    }
}
		  

#endif // OLIVE_C_
