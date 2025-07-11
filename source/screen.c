#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/polygon.h"
#include "../include/screen.h"

typedef struct {
    int height;
    int width;
    char **pixels;
} window_t;

window_t window;

#define PATTERNSIZE 4
const char *pattern[] = {
    " ",             // empty space
    "\xe2\xa1\x80",  // ⡀
    "\xe2\xa3\x80",  // ⣀
    "\xe2\xa3\x84",  // ⣄
    "\xe2\xa3\xa4",  // ⣤
    "\xe2\xa3\xa6",  // ⣦
    "\xe2\xa3\xb6",  // ⣶
    "\xe2\xa3\xb7",  // ⣷
    "\xe2\xa3\xbf",  // ⣿
    "\xe2\x96\x88"   // █
};


// init_window : initialize a window of size height and width
int init_window(const int height, const int width) {
    window.height = height;
    window.width = width;
    window.pixels = malloc((window.height * window.width) * sizeof(char *));
    if (window.pixels == NULL)
        return 1;
    
    for (int i = 0; i < window.height * window.width; i++) {
        window.pixels[i] = malloc(PATTERNSIZE * sizeof(char));  // Space + null terminator
        strcpy(window.pixels[i], pattern[1]);
    }

    return 0;
}

// destroy_window: free the memory contained in the window
void destroy_window(void) {
    for (int i = 0; i < window.height * window.width; i++)
        free(window.pixels[i]);
    free(window.pixels);
}

// draw_window : draw the buffer of characters
int draw_window(void) {
    for (int i = 0; i < window.height * window.width; i++) {
        if (i % window.width == 0 && i != 0) printf("\n");
        printf("%s", window.pixels[i]);
    }
    printf("\n");
}

// plot : put vertex in pixels buffer
void plot(vertex_t v) {
    double d = 1; // distance of image plane (screen)

    // projected coordinated on a the image plane
    double projx = (v.x * d) / v.z;
    double projy = (v.y * d) / v.z;

    // convert to pixel coordinates
    double pxx = (window.width / 2) + projx;
    double pxy = (window.height / 2) + projy;

    // array index
    int index = (window.width * (int) pxy) + (int) pxx;
    if (index >= 0 && index < window.height * window.width) // quick bounds check
        strcpy(window.pixels[index], v.pattern);
}