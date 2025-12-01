// Compile: gcc -Os img2txt.c -o img2txt -lm
// Needs stb_image.h in the same folder
// Run: ./img2txt image.png

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <math.h>
#define STBI_NO_SIMD

static inline void set_fg(unsigned char r, unsigned char g, unsigned char b) {
    printf("\x1b[38;2;%u;%u;%um", r, g, b);
}

static inline void set_bg(unsigned char r, unsigned char g, unsigned char b) {
    printf("\x1b[48;2;%u;%u;%um", r, g, b);
}

static inline void reset_colors() {
    printf("\x1b[0m");
}

// Simple nearest-neighbor resize
unsigned char* resize_nn(unsigned char* src, int w, int h, int ch, int new_w, int new_h) {
    unsigned char* out = malloc(new_w * new_h * ch);
    for (int y = 0; y < new_h; y++) {
        int sy = y * h / new_h;
        for (int x = 0; x < new_w; x++) {
            int sx = x * w / new_w;
            for (int c = 0; c < ch; c++) {
                out[(y * new_w + x) * ch + c] = src[(sy * w + sx) * ch + c];
            }
        }
    }
    return out;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s image\n", argv[0]);
        return 1;
    }

    int w, h, n;
    unsigned char* img = stbi_load(argv[1], &w, &h, &n, 3);
    if (!img) {
        fprintf(stderr, "Could not load: %s\n", argv[1]);
        return 1;
    }

    // Get terminal size using ioctl, fallback to 80x24
    int term_w = 80, term_h = 24;
    if (isatty(STDOUT_FILENO)) {
        struct winsize ws;
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0) {
            if (ws.ws_col > 0) term_w = ws.ws_col;
            if (ws.ws_row > 0) term_h = ws.ws_row;
        }
    }

    int max_h = term_h * 2; // Each printed row represents 2 pixels vertically

    // Compute scale to fit
    double scale_w = (double)term_w / w;
    double scale_h = (double)max_h / h;
    double scale = (scale_w < scale_h) ? scale_w : scale_h;

    int new_w = (int)(w * scale);
    int new_h = (int)(h * scale);
    if (new_w < 1) new_w = 1;
    if (new_h < 1) new_h = 1;

    unsigned char* resized = resize_nn(img, w, h, 3, new_w, new_h);
    stbi_image_free(img);

    // Print using ▀ half-block trick
    for (int y = 0; y < new_h; y += 2) {
        for (int x = 0; x < new_w; x++) {
            int i1 = (y * new_w + x) * 3;
            unsigned char r1 = resized[i1];
            unsigned char g1 = resized[i1 + 1];
            unsigned char b1 = resized[i1 + 2];

            unsigned char r2 = 0, g2 = 0, b2 = 0;
            if (y + 1 < new_h) {
                int i2 = ((y + 1) * new_w + x) * 3;
                r2 = resized[i2];
                g2 = resized[i2 + 1];
                b2 = resized[i2 + 2];
            }

            set_fg(r1, g1, b1);
            set_bg(r2, g2, b2);
            printf("▀");
        }
        reset_colors();
        printf("\n");
    }

    free(resized);
    return 0;
}
