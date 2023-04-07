#include <stdio.h>
#include <immintrin.h>
#include "TXLib.h"
#include "SSE.h"

int SIZEX = 800;
int SIZEY = 800;

int BUFSIZE = 8;

#define BYTE unsigned char
#define XSHIFT (params.width / (float) SIZEX)
#define YSHIFT (params.height / (float) SIZEY)

float RMAX = 10 * 10;
int NMAX = 255;

int mandelbrot()
{
    parameters params = {};
    volatile int res = SetInitialParams(&params);

    __m256 offsets = {0, 1, 2, 3, 4, 5, 6, 7};
    BYTE* colors = (BYTE*) calloc(8, sizeof(BYTE));

    txCreateWindow(SIZEX, SIZEY);
    RGBQUAD* mem = txVideoMemory();
    int counter = 0;
    for (;;)
    {
        if (Hotkeys(&params))
            return NOERR;

        __m256 dx = _mm256_set1_ps(XSHIFT);
        dx = _mm256_mul_ps(dx, offsets);

        PrintFPS(&counter);
        for (int i = 0; i < SIZEY; i++)
        {
            float y0 = (float) i * YSHIFT - params.height/2 + params.ystart;
            __m256 y = _mm256_set1_ps(y0);

            for (int j = 0; j < SIZEX; j += 8)
            {
                float x0 = (float) j * XSHIFT - params.width/2 + params.xstart;
                __m256 x = _mm256_set1_ps(x0);
                x = _mm256_add_ps(x, dx);

                CountColor(x , y, colors);

                for (int k = 0; k < 8; k++)
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

int CountColor(__m256 x, __m256 y, BYTE* colors)
{
    __m256 x0 = x;
    __m256 y0 = y;
    __m256 mask = _mm256_set1_ps(1);
    __m256 rad  = _mm256_set1_ps(RMAX);
    __m256 colorscnr = _mm256_set1_ps(1);
    __m256 twos = _mm256_set1_ps(2);
//    printf("HERE");

    for (int n = 0; n < NMAX; n++)
    {
        __m256 x2 = _mm256_mul_ps(x, x);
        __m256 y2 = _mm256_mul_ps(y, y);

        __m256 dist = _mm256_add_ps(x2, y2);
        __m256 Rcmp = _mm256_cmp_ps(rad, dist, _CMP_GT_OQ);
        mask = _mm256_and_ps(Rcmp, mask);
        colorscnr = _mm256_add_ps(colorscnr, mask);

        if (IsZero(mask))
        {
            for (int i = 0; i < 8; i++)
            {
//                printf("COLOR = %d\n", (BYTE)colorscnr[i]);
                colors[i] = (BYTE) colorscnr[i];
            }
            return 0;
        }

        y = _mm256_mul_ps(_mm256_add_ps(_mm256_mul_ps(_mm256_mul_ps(x, y), twos), y0), mask);
        x = _mm256_mul_ps(_mm256_add_ps(_mm256_sub_ps(x2, y2), x0), mask);
    }

    for (int i = 0; i < 8; i++)
    {
//        printf("COLOR = %d\n", (BYTE) colorscnr[i]);
        colors[i] = (BYTE) colorscnr[i];
    }

    return NOERR;
}

bool IsZero(__m256 arr)
{
    for (int i = 0; i < 8; i++)
    {
        if(arr[i] != 0x0)
            return false;
    }

    return true;
}

int PrintFPS(int* counter)
{
    if (*counter < 10)
    {
        (*counter)++;
    }
    else
    {
        for (int i = 0; i < 10; i++)
        {
            printf("\b");
        }
        printf("FPS = %.2lg ", txGetFPS() * 10);
        *counter = 0;
    }

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

int Hotkeys(parameters* params)
{
    if (txGetAsyncKeyState(VK_ESCAPE))
            return 1;
    if (txGetAsyncKeyState(VK_RIGHT))
        params->xstart += params->width/20;
    if (txGetAsyncKeyState(VK_LEFT))
        params->xstart -= params->width/20;
    if (txGetAsyncKeyState(VK_UP))
        params->ystart -= params->height/20;
    if (txGetAsyncKeyState(VK_DOWN))
        params->ystart += params->height/20;
    if (txGetAsyncKeyState('A'))
    {
        params->width /= 2;
        params->height /= 2;
    }
    if (txGetAsyncKeyState('Z'))
    {
        params->width *= 2;
        params->height *= 2;
    }

    return NOERR;
}
