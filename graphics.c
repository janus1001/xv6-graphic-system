#include "types.h"
#include "user.h"
#include "drawshape.h"
#include "math.c"

#define BUFFER_SIZE 1000 // Can be changed for more complex programs - smaller size is less performant but uses less memory

drawshape shapes[BUFFER_SIZE];
int currentbufferindex = 0;

// Draw line by calling setpixel repeatedly. Very slow.
void drawline(int x0, int y0, int x1, int y1, int color) {
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1; 
    int err = (dx > dy ? dx : -dy) / 2, e2;
    
    while(true) {
        setpixel(x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        e2 = err;
        if (e2 > -dx) { err -= dy; x0 += sx; }
        if (e2 < dy) { err += dx; y0 += sy; }
    }
}

// Run at the end of each buffer to update buffer index and check for full buffer
void processbuffer() {
    currentbufferindex++;
    if(currentbufferindex == BUFFER_SIZE)
    {
        endgraphics();
        begingraphics();
    }
}

// buffers for each of the operations: pixel, line, circle, rect, box, skew

void bufferpixel(int x, int y, int color) {
    shapes[currentbufferindex].type = pixel;
    shapes[currentbufferindex].x0 = x;
    shapes[currentbufferindex].y0 = y;
    shapes[currentbufferindex].color = color;
    processbuffer();
}

void bufferline(int x0, int y0, int x1, int y1, int color) {
    shapes[currentbufferindex].type = line;
    shapes[currentbufferindex].x0 = x0;
    shapes[currentbufferindex].y0 = y0;
    shapes[currentbufferindex].x1 = x1;
    shapes[currentbufferindex].y1 = y1;
    shapes[currentbufferindex].color = color;
    processbuffer();
}

void buffercircle(int x, int y, int radius, int color) {
    shapes[currentbufferindex].type = circle;
    shapes[currentbufferindex].x0 = x;
    shapes[currentbufferindex].y0 = y;
    shapes[currentbufferindex].x1 = radius;
    shapes[currentbufferindex].color = color;
    processbuffer();
}

void bufferrect(int x0, int y0, int x1, int y1, int color) {
    shapes[currentbufferindex].type = rect;
    shapes[currentbufferindex].x0 = x0;
    shapes[currentbufferindex].y0 = y0;
    shapes[currentbufferindex].x1 = x1;
    shapes[currentbufferindex].y1 = y1;
    shapes[currentbufferindex].color = color;
    processbuffer();
}

void bufferbox(int x0, int y0, int x1, int y1, int color) {
    shapes[currentbufferindex].type = box;
    shapes[currentbufferindex].x0 = x0;
    shapes[currentbufferindex].y0 = y0;
    shapes[currentbufferindex].x1 = x1;
    shapes[currentbufferindex].y1 = y1;
    shapes[currentbufferindex].color = color;
    processbuffer();
}

void bufferskew(int amount) {
    shapes[currentbufferindex].type = skew;
    shapes[currentbufferindex].x0 = amount;
    processbuffer();
}

void begingraphics() {
    // Clear the buffer
    memset(shapes, 0, sizeof(drawshape) * BUFFER_SIZE);
    currentbufferindex = 0;
}

void endgraphics(){
    render(shapes, currentbufferindex);
}
