#include "types.h"
#include "user.h"

int main(int argc, char** argv) {
    int mode = atoi(argv[1]);
    if(argc == 1 || mode == 13)
    {
        setvga(0x13);
    }
    else if(mode == 12)
    {
        setvga(0x12);
    }
    else
    {
        printf(1, "Wrong argument supplied.\n");
        exit();
    }

    printf(1, "Using batched draw calls to draw stuff.\n");

    begingraphics();
    for (int i = 0; i < 101; i++)
    {
        for (int j = 0; j < 101; j++)
        {
            bufferpixel(i, j, 5); // Very expensive operation isn't shown due to backbuffer
        }
    }

    bufferline(0, 0, 100, 100, 0xA);

    buffercircle(50, 50, 20, 0xF);
    buffercircle(55, 58, 25, 0xF);
    buffercircle(60, 66, 30, 0xF);

    bufferrect(165, 98, 249, 195, 0x1); // Blue
    bufferrect(134, 122, 310, 138, 0xA); // Green

    bufferrect(100, 101, 120, 210, 0x3); // Cyan
    bufferbox(100, 101, 120, 210, 0xF);

    bufferrect(20, 120, 50, 152, 0x6); // Yellow - Brown
    bufferbox(20, 120, 50, 152, 0xF);

    buffercircle(300, 300, 200, 0xB); // Off limits circle

    endgraphics();
    present();

    getch();
    setvga(0x03);
    
    exit();
}