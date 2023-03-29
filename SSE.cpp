#include <stdio.h>
#include <xmmintrin.h>
#include "TXLib.h"

#define BYTE unsigned char

#include "SSE.h"

int SIZEX = 800;
int SIZEY = 800;
float RMAX = 10 * 10;
int NMAX = 255;

float XSHIFT = 4/((float) SIZEX);
float YSHIFT = 4/((float) SIZEY);

int mandelbrot()
{
    __m128 _0123 = {0, 1, 2, 3};
    __m128 dx = {XSHIFT, XSHIFT, XSHIFT, XSHIFT};
    dx = _mm_mul_ps(dx, _0123);
    BYTE* colors = (BYTE*) calloc(4, sizeof(BYTE));

    txCreateWindow(SIZEX, SIZEY);
    RGBQUAD* mem = txVideoMemory();
    for (;;)
    {
        for (;;)
        {
            if (txGetAsyncKeyState(VK_ESCAPE))
                return 0;

            printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\bFPS = %.2lg", txGetFPS());
            for (int i = 0; i < SIZEY; i++)
            {
                float y0 = (float) i * YSHIFT - 2;
                __m128 y = {y0, y0, y0, y0};

                for (int j = 0; j < SIZEX; j += 4)
                {
                    float x0 = (float) j * XSHIFT - 2;
                    __m128 x = {x0, x0, x0, x0};
                    x = _mm_add_ps(x, dx);

                    CountColor(x , y, colors);

                    for (int k = 0; k < 4; k++)
                    {
//                        printf("LOADING COLORS\n");
//                        printf("COLOR = %c\n", (BYTE) colors[k]);
                        mem[j + k + (SIZEY - 1 - i) * SIZEX] = {(BYTE) colors[k], (BYTE) colors[k], (BYTE) colors[k], 0};
                    }
                }
            }
            txRedrawWindow();
        }
    }

    free(colors);

    return NOERR;
}

int CountColor(__m128 x, __m128 y, BYTE* colors)
{
    __m128 x0 = x;
    __m128 y0 = y;
    __m128 mask = {1, 1, 1, 1};
    __m128 rad  = {RMAX, RMAX, RMAX, RMAX};
    __m128 colorscnr = {1, 1, 1, 1};
    __m128 twos = {2, 2, 2, 2};
    __m64 res = {0, 0};
//    printf("HERE");

    for (int n = 0; n < NMAX; n++)
    {
        __m128 x2 = _mm_mul_ps(x, x);
        __m128 y2 = _mm_mul_ps(y, y);

        __m128 dist = _mm_add_ps(x2, y2);
        __m128 Rcmp = _mm_cmpgt_ps(rad, dist);
        mask = _mm_and_ps(Rcmp, mask);
        colorscnr = _mm_add_ps(colorscnr, mask);

        if (IsZero(mask))
        {
            for (int i = 0; i < 4; i++)
            {
//                printf("COLOR = %d\n", (BYTE)colorscnr[i]);
                colors[i] = (BYTE) colorscnr[i];
            }
            return 0;
        }

        y = _mm_mul_ps(_mm_add_ps(_mm_mul_ps(_mm_mul_ps(x, y), twos), y0), mask);
        x = _mm_mul_ps(_mm_add_ps(_mm_sub_ps(x2, y2), x0), mask);
    }

    for (int i = 0; i < 4; i++)
    {
//        printf("COLOR = %d\n", (BYTE) colorscnr[i]);
        colors[i] = (BYTE) colorscnr[i];
    }

    return NOERR;
}

bool IsZero(__m128 arr)
{
    for (int i = 0; i < 4; i++)
    {
        if(arr[i] != 0x0)
            return false;
    }

    return true;
}
