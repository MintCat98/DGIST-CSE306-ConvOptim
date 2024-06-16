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

// This implementation is simply copied from "main.c".
// Your job is to modify and optimize it for better performance!
inline static Pixel convolution(Pixel* input, int x, int y, int width, int height, const int* filter) {
    int r = 0, g = 0, b = 0; // double to int
    int index = 0;

    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            int ix = x + dx;
            int iy = y + dy;
            if (ix >= 0 && ix < width && iy >= 0 && iy < height) {
                index = (dy + 1) * 3 + (dx + 1);
                int pixel_index = iy * width + ix;
                r += input[pixel_index].r * filter[index];
                g += input[pixel_index].g * filter[index];
                b += input[pixel_index].b * filter[index];
            }
        }
    }

    r = r < 0 ? 0 : (r > 255 ? 255 : r);
    g = g < 0 ? 0 : (g > 255 ? 255 : g);
    b = b < 0 ? 0 : (b > 255 ? 255 : b);

    Pixel p = { (unsigned char)r, (unsigned char)g, (unsigned char)b };
    return p;
}


void filter_optimized(void* args[]) {
    unsigned int width = *(unsigned int*)args[0];
    unsigned int height = *(unsigned int*)args[1];
    Pixel* input = args[2];
    Pixel* output = args[3];
    const float* filter_float = args[4];

    // Convert filter to integer scale (e.g., multiply by 256 and round)
    int filter[9];
    for (int i = 0; i < 9; ++i) {
        filter[i] = (int)(filter_float[i] * 256);
    }

    // Define block size
    int blockSize = 16;

    for (int by = 0; by < height; by += blockSize) {
        for (int bx = 0; bx < width; bx += blockSize) {
            for (int y = by; y < by + blockSize && y < height; ++y) {
                for (int x = bx; x < bx + blockSize && x < width; x += 4) {
                    // Loop unrolling for x direction
                    output[y * width + x] = convolution(input, x, y, width, height, filter);
                    if (x + 1 < bx + blockSize && x + 1 < width) {
                        output[y * width + (x + 1)] = convolution(input, x + 1, y, width, height, filter);
                    }
                    if (x + 2 < bx + blockSize && x + 2 < width) {
                        output[y * width + (x + 2)] = convolution(input, x + 2, y, width, height, filter);
                    }
                    if (x + 3 < bx + blockSize && x + 3 < width) {
                        output[y * width + (x + 3)] = convolution(input, x + 3, y, width, height, filter);
                    }
                }
            }
        }
    }
}