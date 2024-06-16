#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmplib.h"
#include "hw2.h"

// Tiling method
#define TILE_SIZE 16

// Merge 2 functions and remove unnecessary memory allocation
void filter_optimized(void* args[]) {
    unsigned int width = *(unsigned int*)args[0];
    unsigned int height = *(unsigned int*)args[1];
    Pixel* input = (Pixel*)args[2];
    Pixel* output = (Pixel*)args[3];
    float* filter = (float*)args[4];

    int tile_x, tile_y, x, y;

    // Pre-calculate for loop-unrolling
    float f0 = filter[0], f1 = filter[1], f2 = filter[2];
    float f3 = filter[3], f4 = filter[4], f5 = filter[5];
    float f6 = filter[6], f7 = filter[7], f8 = filter[8];

    // Tiling method
    for (tile_y = 0; tile_y < height; tile_y += TILE_SIZE) {
        for (tile_x = 0; tile_x < width; tile_x += TILE_SIZE) {
            for (y = tile_y; y < tile_y + TILE_SIZE && y < height; ++y) {
                for (x = tile_x; x < tile_x + TILE_SIZE && x < width; ++x) {
                    double r = 0, g = 0, b = 0;
                    int idx = y * width + x;

                    // Handle boundaries
                    if (x > 0 && x < width - 1 && y > 0 && y < height - 1) {
                        // Loop-unrolling for non-boundary pixels
                        r = input[idx - width - 1].r * f0 +
                            input[idx - width].r * f1 +
                            input[idx - width + 1].r * f2 +
                            input[idx - 1].r * f3 +
                            input[idx].r * f4 +
                            input[idx + 1].r * f5 +
                            input[idx + width - 1].r * f6 +
                            input[idx + width].r * f7 +
                            input[idx + width + 1].r * f8;

                        g = input[idx - width - 1].g * f0 +
                            input[idx - width].g * f1 +
                            input[idx - width + 1].g * f2 +
                            input[idx - 1].g * f3 +
                            input[idx].g * f4 +
                            input[idx + 1].g * f5 +
                            input[idx + width - 1].g * f6 +
                            input[idx + width].g * f7 +
                            input[idx + width + 1].g * f8;

                        b = input[idx - width - 1].b * f0 +
                            input[idx - width].b * f1 +
                            input[idx - width + 1].b * f2 +
                            input[idx - 1].b * f3 +
                            input[idx].b * f4 +
                            input[idx + 1].b * f5 +
                            input[idx + width - 1].b * f6 +
                            input[idx + width].b * f7 +
                            input[idx + width + 1].b * f8;
                    }
                    else {
                        // Only apply convolution for boundary pixels
                        for (int dy = -1; dy <= 1; ++dy) {
                            for (int dx = -1; dx <= 1; ++dx) {
                                int ix = x + dx;
                                int iy = y + dy;
                                if (ix >= 0 && ix < width && iy >= 0 && iy < height) {
                                    int input_idx = iy * width + ix;
                                    int filter_idx = (dy + 1) * 3 + (dx + 1);
                                    r += input[input_idx].r * filter[filter_idx];
                                    g += input[input_idx].g * filter[filter_idx];
                                    b += input[input_idx].b * filter[filter_idx];
                                }
                            }
                        }
                    }

                    output[idx].r = (unsigned char)(r < 0 ? 0 : (r > 255 ? 255 : r));
                    output[idx].g = (unsigned char)(g < 0 ? 0 : (g > 255 ? 255 : g));
                    output[idx].b = (unsigned char)(b < 0 ? 0 : (b > 255 ? 255 : b));
                }
            }
        }
    }
}
