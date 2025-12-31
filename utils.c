#ifndef UTILS_C_
#define UTILS_C_

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>

#define return_defer(res) do { result = (res); goto defer;} while (0)

typedef int Errno;

Errno utils_save_to_ppm(uint32_t *pixels, size_t width, size_t height, const char *filepath)
{
    FILE *f = NULL;
    int result = 0;

    {
	f = fopen(filepath, "w");
	if (f == NULL) return_defer(errno);

	fprintf(f, "P6 %zu %zu 255\n", width, height);
	if (ferror(f)) return_defer(errno);

	for (size_t i = 0; i < width*height; i++) {
	    // 0xAABBGGRR
	    uint32_t pixel = pixels[i];
	    uint8_t bytes[3] = {
		pixel & 0xFF,
		(pixel>>8) & 0xFF,
		(pixel>>8*2) & 0xFF,
	    };
	    fwrite(bytes, sizeof(bytes), 1, f);
	    if (ferror(f)) return_defer(errno);
	}
    }
    
defer:
    if (f) fclose(f);
    return result;
}


#endif // UTILS_C_
