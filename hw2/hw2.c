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
static inline Pixel convolution( // make this function as an inline
    Pixel* input, int x, int y, int width, int height, float* filter) {
    double r = 0, g = 0, b = 0;

    int x_start = (x - 1 < 0) ? 0 : x - 1;
    int x_end = (x + 1 >= width) ? width - 1 : x + 1;
    int y_start = (y - 1 < 0) ? 0 : y - 1;
    int y_end = (y + 1 >= height) ? height - 1 : y + 1;

    // Loop unrolling
    for (int dy = y_start; dy <= y_end; ++dy) {
        for (int dx = x_start; dx <= x_end; dx += 2) {
            int filter_x1 = dx - x + 1;
            int filter_y1 = dy - y + 1;
            float filter_value1 = filter[filter_x1 + filter_y1 * 3];

            r += input[dx + dy * width].r * filter_value1;
            g += input[dx + dy * width].g * filter_value1;
            b += input[dx + dy * width].b * filter_value1;

            if (dx + 1 <= x_end) {
                int filter_x2 = filter_x1 + 1;
                float filter_value2 = filter[filter_x2 + filter_y1 * 3];

                r += input[(dx + 1) + dy * width].r * filter_value2;
                g += input[(dx + 1) + dy * width].g * filter_value2;
                b += input[(dx + 1) + dy * width].b * filter_value2;
            }
        }
    }

    r = (r < 0) ? 0 : (r > 255) ? 255 : r;
    g = (g < 0) ? 0 : (g > 255) ? 255 : g;
    b = (b < 0) ? 0 : (b > 255) ? 255 : b;

    Pixel p;
    memset(&p, 0, sizeof(p));  // Pixel 구조체를 0으로 초기화

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

    // Use stack memory
    Pixel p1, p2;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; x += 4) {
            // Loop unrolling
            p1 = convolution(input, x, y, width, height, filter);
            output[x + y * width] = p1;

            if (x + 1 < width) {
                p2 = convolution(input, x + 1, y, width, height, filter);
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
        }
    }
}