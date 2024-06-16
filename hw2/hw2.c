#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmplib.h"
#include "hw2.h"

// Define the tile size
#define TILE_SIZE 16

void filter_optimized(void* args[]) {
    unsigned int width = *(unsigned int*)args[0];
    unsigned int height = *(unsigned int*)args[1];
    Pixel* input = (Pixel*)args[2];
    Pixel* output = (Pixel*)args[3];
    float* filter = (float*)args[4];

    int tile_x, tile_y, x, y;

    // Process the image in tiles to improve cache performance
    for (tile_y = 0; tile_y < height; tile_y += TILE_SIZE) {
        for (tile_x = 0; tile_x < width; tile_x += TILE_SIZE) {
            for (y = tile_y; y < tile_y + TILE_SIZE && y < height; ++y) {
                for (x = tile_x; x < tile_x + TILE_SIZE && x < width; ++x) {
                    double r = 0, g = 0, b = 0;
                    int idx = y * width + x;

                    // Handle boundaries
                    if (x > 0 && x < width - 1 && y > 0 && y < height - 1) {
                        // Unrolling the loop for better performance for non-boundary pixels
                        r += input[idx - width - 1].r * filter[0];
                        r += input[idx - width].r * filter[1];
                        r += input[idx - width + 1].r * filter[2];
                        r += input[idx - 1].r * filter[3];
                        r += input[idx].r * filter[4];
                        r += input[idx + 1].r * filter[5];
                        r += input[idx + width - 1].r * filter[6];
                        r += input[idx + width].r * filter[7];
                        r += input[idx + width + 1].r * filter[8];

                        g += input[idx - width - 1].g * filter[0];
                        g += input[idx - width].g * filter[1];
                        g += input[idx - width + 1].g * filter[2];
                        g += input[idx - 1].g * filter[3];
                        g += input[idx].g * filter[4];
                        g += input[idx + 1].g * filter[5];
                        g += input[idx + width - 1].g * filter[6];
                        g += input[idx + width].g * filter[7];
                        g += input[idx + width + 1].g * filter[8];

                        b += input[idx - width - 1].b * filter[0];
                        b += input[idx - width].b * filter[1];
                        b += input[idx - width + 1].b * filter[2];
                        b += input[idx - 1].b * filter[3];
                        b += input[idx].b * filter[4];
                        b += input[idx + 1].b * filter[5];
                        b += input[idx + width - 1].b * filter[6];
                        b += input[idx + width].b * filter[7];
                        b += input[idx + width + 1].b * filter[8];
                    }
                    else {
                        // Apply convolution for boundary pixels
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
