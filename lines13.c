#include "types.h"
#include "user.h"

int main() {
    setvga(0x13);

    for (int i = 0; i < 21; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            drawline(5 + i * 16 + j, 5, 5 + i * 16 + j, 100, i);
        }
    }

    printf(1, "Displaying LINES in 320x200!\n");
    getch();
    setvga(0x03);
    
    exit();
}