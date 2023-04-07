#include <stdio.h>
#include <xmmintrin.h>
#include "TXLib.h"

#define BYTE unsigned char

#include "NO-SSE.h"

int SIZEX = 800;
int SIZEY = 800;
float RMAX = 10 * 10;
int NMAX = 255;

float XSHIFT = 4/((float) SIZEX);
float YSHIFT = 4/((float) SIZEY);

int mandelbrot()
{
    txCreateWindow(SIZEX, SIZEY);
    RGBQUAD* mem = txVideoMemory();
        for (;;)
        {
            if (txGetAsyncKeyState(VK_ESCAPE))
                return 0;

            printf("\b\b\b\b\b\b\b\b\bFPS = %.2lg", txGetFPS());
            for (int i = 0; i < SIZEY; i++)
            {
                float y0 = (float) i * YSHIFT - 2;

                for (int j = 0; j < SIZEX; j++)
                {
                    float x0 = (float) j * XSHIFT - 2;
                    float x = x0;
                    float y = y0;
                    BYTE n = 0;

                        for (; n < NMAX; n++)
                        {
                            float x2 = x * x;
                            float y2 = y * y;

                            if (x2 + y2 > RMAX)
                            {
                                break;
                            }

                            y = x * y + x * y + y0;
                            x = x2 - y2 + x0;
                        }

                    if (n == 255)
                        n = 0;

                    mem[j + (SIZEY - 1 - i) * SIZEX] = {n * 2, n, n * 2, 0};
                }
            }
            txRedrawWindow();
        }

    return NOERR;
}
