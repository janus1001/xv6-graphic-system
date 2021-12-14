#include "types.h"
#include "user.h"

int absol (int i)
{
  return i < 0 ? -i : i;
}

int offset(int m, int i)
{
    i %= 30;
    m /= 10;
    return (70 - absol(i % (m * 2) - m));
}

int voffset(int m, int i)
{
    i %= 30;
    i+=52;
    m /= 10;
    return -(absol(i % (m * 2) - m));
}

int main(int argc, char** argv) {
    int mode = atoi(argv[1]);
    int sleeptime = 3;
    if(argc == 1 || mode == 13)
    {
        setvga(0x13);
    }
    else if(mode == 12)
    {
        sleeptime = 1;
        setvga(0x12);
    }
    else
    {
        printf(1, "Wrong argument supplied.\n");
        exit();
    }

    printf(1, "Get stickbugged lol\n");

    for (int i = 0; i < 100; i++)
    {
        begingraphics();
        bufferrect(0,0,1000,1000, 0x4);  // Fill whole buffer with color - overflow check included

        bufferline(13 + offset(175, i), 175+ voffset(175, i), 19 + offset(151, i), 151+ voffset(151, i) , 0xE);
        bufferline(19 + offset(151, i), 151+ voffset(151, i), 51 + offset(150, i), 150+ voffset(150, i) , 0xE);
        bufferline(51 + offset(150, i), 150+ voffset(150, i), 52 + offset(182, i) ,182+ voffset(182, i) , 0xE);
        bufferline(68 + offset(161, i), 161+ voffset(161, i), 46 + offset(147, i) ,147+ voffset(147, i) , 0xE);
        bufferline(94 + offset(161, i), 161+ voffset(161, i), 68 + offset(161, i) ,161+ voffset(161, i) , 0xE);
        bufferline(44 + offset(169, i), 169+ voffset(169, i), 58 + offset(156, i) ,156+ voffset(156, i) , 0xE);
        bufferline(44 + offset(169, i), 169+ voffset(169, i), 50 + offset(184, i) ,184+ voffset(184, i) , 0xE);
        bufferline(61 + offset(167, i), 167+ voffset(167, i), 66 + offset(161, i) ,161+ voffset(161, i) , 0xE);
        bufferline(63 + offset(185, i), 185+ voffset(185, i), 61 + offset(167, i) ,167+ voffset(167, i) , 0xE);
        bufferline(70 + offset(168, i), 168+ voffset(168, i), 68 + offset(162, i) ,162+ voffset(162, i) , 0xE);
        bufferline(70 + offset(168, i), 168+ voffset(168, i), 72 + offset(180, i) ,180+ voffset(180, i) , 0xE);
        bufferline(91 + offset(171, i), 171+ voffset(171, i), 73 + offset(161, i) ,161+ voffset(161, i) , 0xE);
        bufferline(91 + offset(171, i), 171+ voffset(171, i), 110 + offset(190, i),190 + voffset(190, i), 0xE);
        
        endgraphics();
        present();

        sleep(sleeptime);
    }
    setvga(0x03);
    
    exit();
}