#include "types.h"
#include "user.h"

int main() {
    setvga(0x12);
    
    for (int i = 0; i < 19; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            drawline(i * 16 + j, 0,i * 16 + j, 100, 15-i);
        }
    }
    
    printf(1, "Displaying LINES in 640x480!!!\n");
    getch();

    setvga(0x03);
    exit();
}