#include "types.h"
#include "defs.h"
#include "memlayout.h"
#include "drawshape.h"
#include "math.c"

char backbuffer[153600] = {0};

char* getbackbufferpage(int page) {
    switch (page) {
        case 1:
            return backbuffer + 38400 * 1;
        case 2:
            return backbuffer + 38400 * 2;
        case 3:
            return backbuffer + 38400 * 3;
        default:
            return backbuffer;
    }
}

int sys_getch(void) {
    return consoleget();
}

int sys_setvga(void) {
    int mode;

    if(argint(0, &mode) < 0)
    {
        return -1;
    }

    return consolevgamode(mode);
}

// Sets backbuffer memory to a pixel color. To be used by plot functions.
void plotpixel(int x, int y, int color) {
    int currentmode = getvgamode();

    if(currentmode == 0x13)
    {
        if(y < 0 || y >= 200 || x < 0 || x >= 320)
        {
            return;
        }
        *(char *)(backbuffer + y * 320 + x) = color;
    }
    else if(currentmode == 0x12)
    {
        if(y < 0 || y >= 480 || x < 0 || x >= 640)
        {
            return;
        }

        for (int i = 0; i < 4; i++)
        {
            //consolevgaplane(i);
            //uchar* plane = consolevgabuffer();
            
            // Set pixel
            char pixeloffset = x % 8;
            uchar bitvalue = (color >> i) & 1;

            *(char *)(getbackbufferpage(i) + y * 640 / 8 + x / 8) |= (bitvalue << (7-pixeloffset));
        }
    }
}

void present()
{
    int mode = getvgamode();

    if(mode == 0x13)
    {
        memmove(P2V(0xA0000), backbuffer, 64000);
    }
    else if(mode == 0x12)
    {
        consolevgaplane(0);
        uchar* plane = consolevgabuffer();
        memmove(plane, getbackbufferpage(0), 38400);
        consolevgaplane(1);
        plane = consolevgabuffer();
        memmove(plane, getbackbufferpage(1), 38400);
        consolevgaplane(2);
        plane = consolevgabuffer();
        memmove(plane, getbackbufferpage(2), 38400);
        consolevgaplane(3);
        plane = consolevgabuffer();
        memmove(plane, getbackbufferpage(3), 38400);
    }
    // Clear backbuffer
    memset(backbuffer, 0, 153600);
}

///
/// Some plot functions adopted from https://rosettacode.org/ under GNU Free Documentation License 1.2
///

void plotline(int x0, int y0, int x1, int y1, int color)
{
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1; 
    int err = (dx > dy ? dx : -dy) / 2, e2;
    
    while(true) {
        plotpixel(x0, y0, color);
        if (x0 == x1 && y0 == y1) 
            break;
        e2 = err;
        if (e2 > -dx) {
            err -= dy; x0 += sx;
        }
        if (e2 < dy) { 
            err += dx; y0 += sy; 
        }
    }
}

void plotcircle(int x0, int y0, int radius, int color)
{
    int f = 1 - radius;
    int ddF_x = 0;
    int ddF_y = -2 * radius;
    int x = 0;
    int y = radius;
 
    plotpixel(x0, y0 + radius, color);
    plotpixel(x0, y0 - radius, color);
    plotpixel(x0 + radius, y0, color);
    plotpixel(x0 - radius, y0, color);
 
    while(x < y) 
    {
        if(f >= 0) 
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x + 1;    
        plotpixel(x0 + x, y0 + y, color);
        plotpixel(x0 - x, y0 + y, color);
        plotpixel(x0 + x, y0 - y, color);
        plotpixel(x0 - x, y0 - y, color);
        plotpixel(x0 + y, y0 + x, color);
        plotpixel(x0 - y, y0 + x, color);
        plotpixel(x0 + y, y0 - x, color);
        plotpixel(x0 - y, y0 - x, color);
    }
}

// With fill
void plotrect(int x, int y, int sizex, int sizey, int color)
{
    int currentmode = getvgamode();

    if(currentmode == 0x13)
    {
        int amountOfReps = min(abs(x - sizex), 320 - x);
        int amountOfRows = min(abs(y - sizey), 200 - y);

        for (int i = 0; i < amountOfRows; i++)
        {
            memset(backbuffer + x + (y + i) * 320, (char)color, amountOfReps);
        }
    }
    else if(currentmode == 0x12)
    {
        int amountOfReps = min(abs(x - sizex), 640 - x);
        int amountOfRows = min(abs(y - sizey), 480 - y);

        // I call the 8 bits on each side that I cannot fill with memset "orphans". I am angry.
        int numberoffrontorphans = x % 8;
        int numberofbackorphans = (x + sizex) % 8;

        // cprintf("%d - %d\n", numberoffrontorphans, numberofbackorphans);

        for (int i = 0; i < 4; i++)
        {
            int bitvalue = 0;
            if((color >> i) & 1)
            {
                bitvalue = 0xFFFFFFFF;
            }
            
            for (int j = 0; j < amountOfRows; j++)
            {
                for (int orphanindex = 0; orphanindex < 8; orphanindex++)
                {
                    if(orphanindex < numberoffrontorphans)
                    {
                        plotpixel(x + orphanindex, y + j, color);
                    }
                }
                memset(getbackbufferpage(i) + (y + j) * 640 / 8 + x / 8 + 1, bitvalue, amountOfReps / 8);  // +1 is because of orphans
                for (int orphanindex = 0; orphanindex < 8; orphanindex++)
                {
                    if(orphanindex < numberofbackorphans)
                    {
                        plotpixel(sizex + orphanindex - numberofbackorphans, y + j, color);
                    }
                }
            }
        }
    }
}

// No fill
void plotbox(int x0, int y0, int x1, int y1, int color)
{
    plotline(x0, y0, x1, y0, color);
    plotline(x1, y0, x1, y1, color);
    plotline(x0, y1, x0, y0, color);
    plotline(x0, y1, x1, y1, color);
}

void doskew(int amount)
{
    
}

// Instantly puts pixel in video memory, skipping rendering pipeline. To be called as system call only.
int sys_setpixel(void) {
    int x;
    int y;
    int color;

    if(argint(0, &x) < 0)
    {
        return -1;
    }
    if(argint(1, &y) < 0)
    {
        return -1;
    }
    if(argint(2, &color) < 0)
    {
        return -1;
    }
    plotpixel(x, y, color);
    
    return 0;
}

int sys_clear(void) {
    memset(P2V(0xA0000), 0, 64000);

    return 0;
}

int sys_render(void) {
    int numberofoperations;
    drawshape* bufferposition;

    if(argptr(0, (char**)&bufferposition, sizeof(bufferposition)) < 0)
    {
        return -1;
    }
    if(argint(1, &numberofoperations) < 0)
    {
        return -1;
    }
    
    for (int i = 0; i < numberofoperations; i++)
    {
        switch (bufferposition[i].type)
        {
        case pixel:
            plotpixel(bufferposition[i].x0, bufferposition[i].y0, bufferposition[i].color);
            break;
        case line:
            plotline(bufferposition[i].x0, bufferposition[i].y0, bufferposition[i].x1, bufferposition[i].y1, bufferposition[i].color);
            break;
        case circle:
            plotcircle(bufferposition[i].x0, bufferposition[i].y0, bufferposition[i].x1, bufferposition[i].color);
            break;
        case rect:
            plotrect(bufferposition[i].x0, bufferposition[i].y0, bufferposition[i].x1, bufferposition[i].y1, bufferposition[i].color);
            break;
        case box:
            plotbox(bufferposition[i].x0, bufferposition[i].y0, bufferposition[i].x1, bufferposition[i].y1, bufferposition[i].color);
            break;
        case skew:
            doskew(bufferposition[i].x0);
            break;
        }
    }
    return 0;
}

int sys_present(void)
{
    present();
    return 0;
}

// TODO: Implement your system call for switching video modes (and any other video-related syscalls)
// in here.
