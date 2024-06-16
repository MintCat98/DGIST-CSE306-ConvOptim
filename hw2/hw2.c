#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmplib.h"
#include "hw2.h"

#define TILE_SIZE 32

static Pixel convolution_optimized(
    Pixel* input, int x, int y, int width, int height, int* filter) {
    int r = 0;
    int g = 0;
    int b = 0;

    Pixel* pixel;
    int* filter_val;
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            int nx = x + dx;
            int ny = y + dy;
            pixel = &input[nx + ny * width];
            filter_val = &filter[(dx + 1) + (dy + 1) * 3];

            r += pixel->r * (*filter_val);
            g += pixel->g * (*filter_val);
            b += pixel->b * (*filter_val);
        }
    }

    r = (r + 128) >> 8; // Equivalent to dividing by 256 and rounding
    g = (g + 128) >> 8;
    b = (b + 128) >> 8;

    Pixel p;
    p.r = (unsigned char)(r < 0 ? 0 : (r > 255 ? 255 : r));
    p.g = (unsigned char)(g < 0 ? 0 : (g > 255 ? 255 : g));
    p.b = (unsigned char)(b < 0 ? 0 : (b > 255 ? 255 : b));

    return p;
}

void filter_optimized(void* args[]) {
    unsigned int width = *(unsigned int*)args[0];
    unsigned int height = *(unsigned int*)args[1];
    Pixel* input = args[2];
    Pixel* output = args[3];
    float* filter_float = args[4];

    // Precompute filter values as integers
    int filter[9];
    for (int i = 0; i < 9; ++i) {
        filter[i] = (int)(filter_float[i] * 256);
    }

    for (int ty = 0; ty < height; ty += TILE_SIZE) {
        for (int tx = 0; tx < width; tx += TILE_SIZE) {
            int x_end = tx + TILE_SIZE < width ? tx + TILE_SIZE : width;
            int y_end = ty + TILE_SIZE < height ? ty + TILE_SIZE : height;

            for (int y = ty; y < y_end; ++y) {
                for (int x = tx; x < x_end; ++x) {
                    if (x > 0 && x < width - 1 && y > 0 && y < height - 1) {
                        Pixel p = convolution_optimized(input, x, y, width, height, filter);
                        output[x + y * width] = p;
                    }
                    else {
                        output[x + y * width] = (Pixel){ 0, 0, 0 };
                    }
                }
            }
        }
    }
}
