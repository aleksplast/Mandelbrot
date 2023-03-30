#include <stdio.h>
#include <xmmintrin.h>
#include "TXLib.h"
#include "SSE.h"

int SIZEX = 800;
int SIZEY = 800;

#define BYTE unsigned char
#define XSHIFT (params.width / (float) SIZEX)
#define YSHIFT (params.height / (float) SIZEY)

float RMAX = 10 * 10;
int NMAX = 255;

int mandelbrot()
{
    parameters params = {};
    SetInitialParams(&params);

    __m128 _0123 = {0, 1, 2, 3};
    BYTE* colors = (BYTE*) calloc(4, sizeof(BYTE));

    txCreateWindow(SIZEX, SIZEY);
    RGBQUAD* mem = txVideoMemory();
    for (;;)
    {
        if (txGetAsyncKeyState(VK_ESCAPE))
            return 0;
        if (txGetAsyncKeyState(VK_RIGHT))
            params.xstart += params.width/20;
        if (txGetAsyncKeyState(VK_LEFT))
            params.xstart -= params.width/20;
        if (txGetAsyncKeyState(VK_UP))
            params.ystart -= params.height/20;
        if (txGetAsyncKeyState(VK_DOWN))
            params.ystart += params.height/20;
        if (txGetAsyncKeyState('A'))
        {
            params.width /= 2;
            params.height /= 2;
        }
        if (txGetAsyncKeyState('Z'))
        {
            params.width *= 2;
            params.height *= 2;
        }

        __m128 dx = {XSHIFT, XSHIFT, XSHIFT, XSHIFT};
        dx = _mm_mul_ps(dx, _0123);

        PrintFPS();
        for (int i = 0; i < SIZEY; i++)
        {
            float y0 = (float) i * YSHIFT - params.height/2 + params.ystart;
            __m128 y = _mm_set1_ps(y0);            //_mm_set1_ps

            for (int j = 0; j < SIZEX; j += 4)
            {
                float x0 = (float) j * XSHIFT - params.width/2 + params.xstart;
                __m128 x = _mm_set1_ps(x0);        //_mm_set1_ps
                x = _mm_add_ps(x, dx);

                CountColor(x , y, &params, colors);

                for (int k = 0; k < 4; k++)
                {
                    mem[j + k + (SIZEY - 1 - i) * SIZEX] = {(BYTE) colors[k], (BYTE) colors[k], (BYTE) colors[k], 0};
                }
            }
        }
        txRedrawWindow();
    }

    free(colors);

    return NOERR;
}

int CountColor(__m128 x, __m128 y, parameters* params, BYTE* colors)
{
    __m128 x0 = x;
    __m128 y0 = y;
    __m128 mask = {1, 1, 1, 1};
    __m128 rad  = {RMAX, RMAX, RMAX, RMAX};
    __m128 colorscnr = {1, 1, 1, 1};
    __m128 twos = {2, 2, 2, 2};
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

int PrintFPS()
{
    for (int i = 0; i < 10; i++)
    {
        printf("\b");
    }
    printf("FPS = %.2lg ", txGetFPS());

    return 0;
}

int SetInitialParams(parameters* params)
{
    params->height = 4;
    params->width = 4;
    params->xstart = 0;
    params->ystart = 0;

    return NOERR;
}
