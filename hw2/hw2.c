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
static inline Pixel convolution(const Pixel* input, int x, int y, int width, int height, const int* filter) {
    int r = 0, g = 0, b = 0;

    // 블록 매트릭스와 루프 언롤링을 사용하여 캐시 효율성을 높임
    for (int dy = 0; dy < 3; ++dy) {
        int iy = y + dy - 1;
        if (iy < 0 || iy >= height) continue;

        for (int dx = 0; dx < 3; ++dx) {
            int ix = x + dx - 1;
            if (ix < 0 || ix >= width) continue;

            const Pixel* pixel = &input[ix + iy * width];
            int filter_value = filter[dx + dy * 3];

            r += pixel->r * filter_value;
            g += pixel->g * filter_value;
            b += pixel->b * filter_value;
        }
    }

    // 값을 1/9로 나누는 대신 9로 곱하는 대신 1/9를 나누는 대신 시프트 연산으로 변환
    r = (r + 4) / 9;  // 근사화된 결과를 위해 반올림
    g = (g + 4) / 9;
    b = (b + 4) / 9;

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
    Pixel* input = (Pixel*)args[2];
    Pixel* output = (Pixel*)args[3];
    float* float_filter = (float*)args[4];

    // 부동 소수점 필터를 정수 필터로 변환
    int filter[9];
    for (int i = 0; i < 9; ++i) {
        filter[i] = (int)(float_filter[i] * 9);  // 정수로 변환
    }

    // 블록 단위로 처리하여 캐시 효율성을 높임
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