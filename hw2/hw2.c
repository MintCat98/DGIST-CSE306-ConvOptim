// Assignment 2 - Efficient C Programming
// System Programming, DGIST, Prof. Yeseong Kim
// 
// YOU WILL TURN IN THIS FILE.
// Read the provided instruction carefully.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmplib.h"
#include "hw2.h"

#define BLOCK_SIZE 16

// This implementation is simply copied from "main.c".
// Your job is to modify and optimize it for better performance!
static inline Pixel convolution(Pixel* input, int x, int y, int width, int height, float* filter) {
    double r = 0, g = 0, b = 0;

    for (int dy = 0; dy < 3; ++dy) {
        for (int dx = 0; dx < 3; ++dx) {
            int ix = x + dx - 1;
            int iy = y + dy - 1;

            if (ix < 0 || ix >= width || iy < 0 || iy >= height) {
                continue;
            }

            Pixel* pixel = &input[ix + iy * width];
            float filter_value = filter[dx + dy * 3];

            r += pixel->r * filter_value;
            g += pixel->g * filter_value;
            b += pixel->b * filter_value;
        }
    }

    r = (r < 0) ? 0 : (r > 255) ? 255 : r;
    g = (g < 0) ? 0 : (g > 255) ? 255 : g;
    b = (b < 0) ? 0 : (b > 255) ? 255 : b;

    Pixel p;
    p.r = (unsigned char)r;
    p.g = (unsigned char)g;
    p.b = (unsigned char)b;

    return p;
}


void filter_optimized(void* args[]) {
    unsigned int width = *(unsigned int*)args[0];
    unsigned int height = *(unsigned int*)args[1];
    Pixel* input = args[2];
    Pixel* output = args[3];
    float* filter = args[4];

    for (int by = 0; by < height; by += BLOCK_SIZE) {
        for (int bx = 0; bx < width; bx += BLOCK_SIZE) {
            for (int y = by; y < by + BLOCK_SIZE && y < height; ++y) {
                for (int x = bx; x < bx + BLOCK_SIZE && x < width; x += 8) {
                    Pixel p1 = convolution(input, x, y, width, height, filter);
                    output[x + y * width] = p1;

                    if (x + 1 < width) {
                        Pixel p2 = convolution(input, x + 1, y, width, height, filter);
                        output[(x + 1) + y * width] = p2;
                    }
                    if (x + 2 < width) {
                        Pixel p3 = convolution(input, x + 2, y, width, height, filter);
                        output[(x + 2) + y * width] = p3;
                    }
                    if (x + 3 < width) {
                        Pixel p4 = convolution(input, x + 3, y, width, height, filter);
                        output[(x + 3) + y * width] = p4;
                    }
                    if (x + 4 < width) {
                        Pixel p5 = convolution(input, x + 4, y, width, height, filter);
                        output[(x + 4) + y * width] = p5;
                    }
                    if (x + 5 < width) {
                        Pixel p6 = convolution(input, x + 5, y, width, height, filter);
                        output[(x + 5) + y * width] = p6;
                    }
                    if (x + 6 < width) {
                        Pixel p7 = convolution(input, x + 6, y, width, height, filter);
                        output[(x + 6) + y * width] = p7;
                    }
                    if (x + 7 < width) {
                        Pixel p8 = convolution(input, x + 7, y, width, height, filter);
                        output[(x + 7) + y * width] = p8;
                    }
                }
            }
        }
    }
}