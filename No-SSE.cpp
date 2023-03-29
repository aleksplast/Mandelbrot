#include <stdio.h>
#include <xmmintrin.h>
#include "TXLib.h"

#define BYTE unsigned char

#include "mandelbrot.h"

int SIZEX = 800;
int SIZEY = 800;
float RMAX = 10;
int NMAX = 255;

float XSHIFT = 4/((float) SIZEX);
float YSHIFT = 4/((float) SIZEY);

int mandelbrot()
{
    txCreateWindow(SIZEX, SIZEY);
    RGBQUAD* mem = txVideoMemory();
    for (;;)
    {
        for (;;)
        {
            if (txGetAsyncKeyState(VK_ESCAPE))
                return 0;

            printf("\b\b\b\b\b\b\b\bFPS = %.2lg", txGetFPS());
            for (int i = 0; i < SIZEY; i++)
            {
                float y0 = (float) i * YSHIFT - 2;

                for (int j = 0; j < SIZEX; j++)
                {
                    RGBQUAD pix = {};
                    float x0 = (float) j * XSHIFT - 2;
                    float x = x0;
                    float y = y0;
                    BYTE n = 0;

                    for (; n < NMAX; n++)
                    {
                        float x2 = x * x;
                        float y2 = y * y;

                        if (x2 + y2 > RMAX)
                            break;

                        x = x * y + x * y + x0;
                        y = y2 - x2 + y0;
                    }

                    pix.rgbBlue = n;
                    pix.rgbGreen = n;
                    pix.rgbRed = n;

                    mem[i + (SIZEY - 1 - j) * SIZEX] = pix;
                }
            }
        }
        txRedrawWindow();
    }

    return NOERR;
}
