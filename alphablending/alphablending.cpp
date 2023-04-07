#include <stdio.h>
#include <immintrin.h>
#include "TXLib.h"
#include "alphablending.h"

int SIZEX = 800;
int SIZEY = 800;
#define INDEX (y * SIZEX + x)

#pragma GCC diagnostic ignored "-Wconversion"

int alphablendNOSSE(RGBQUAD* back, RGBQUAD* front)
{
    txCreateWindow(SIZEX, SIZEY);
    RGBQUAD* mem = txVideoMemory();
    int counter = 0;

    for (;;)
    {
        if (txGetAsyncKeyState(VK_ESCAPE))
            return 1;

        for (int y = 0; y < SIZEY; y++)
        {
            for (int x = 0; x < SIZEX; x++)
            {
                mem[INDEX].rgbBlue = (front[INDEX].rgbBlue * front[INDEX].rgbReserved + back[INDEX].rgbBlue * (255 - front[INDEX].rgbReserved)) >> 8;
                mem[INDEX].rgbRed = (front[INDEX].rgbRed * front[INDEX].rgbReserved + back[INDEX].rgbRed * (255 -  front[INDEX].rgbReserved)) >> 8;
                mem[INDEX].rgbGreen = (front[INDEX].rgbGreen * front[INDEX].rgbReserved + back[INDEX].rgbGreen * (255 - front[INDEX].rgbReserved)) >> 8;
            }
        }
        txRedrawWindow();
        PrintFPS(&counter);

    }

    return NOERR;
}

int alphablendSSE(RGBQUAD* back, RGBQUAD* front)
{
    txCreateWindow(SIZEX, SIZEY);
    RGBQUAD* mem = txVideoMemory();
    int counter = 0;

    __m128i zeros = _mm_set1_epi8(0);

    for (;;)
    {
        if (txGetAsyncKeyState(VK_ESCAPE))
            return 1;

        for (int y = 0; y < SIZEY; y++)
        {
            for (int x = 0; x < SIZEX; x += 4)
            {
                __m128i frontL  = _mm_loadu_si128((__m128i*)(front + INDEX));
                __m128i backL   = _mm_loadu_si128((__m128i*)(back + INDEX));

                __m128i frontH  = (__m128i) (_mm_movehl_ps((__m128)zeros, (__m128)frontL));
                __m128i backH   = (__m128i) (_mm_movehl_ps((__m128)zeros, (__m128)backL));

                frontL  = _mm_cvtepi8_epi16(frontL);
                backL   = _mm_cvtepi8_epi16(backL);
                frontH  = _mm_cvtepi8_epi16(frontH);
                backH   = _mm_cvtepi8_epi16(backH);

                __m128i mask            = _mm_set_epi8(0x80, 14, 0x80, 14, 0x80, 14, 0x80, 14,
                                                       0x80,  6, 0x80,  6, 0x80,  6, 0x80,  6);
                __m128i transparencyL   = _mm_shuffle_epi8(frontL, mask);
                __m128i transparencyH   = _mm_shuffle_epi8(frontH, mask);

                frontL  = _mm_mullo_epi16(frontL, transparencyL);
                backL   = _mm_mullo_epi16(backL, _mm_sub_epi16(_mm_set1_epi16(255), transparencyL));
                frontH  = _mm_mullo_epi16(frontH, transparencyH);
                backH   = _mm_mullo_epi16(backH, _mm_sub_epi16(_mm_set1_epi16(255), transparencyH));

                __m128i sumL = _mm_add_epi16(frontL, backL);
                __m128i sumH = _mm_add_epi16(frontH, backH);

                mask = _mm_set_epi8(0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
                                      15,   13,   11,    9,    7,    5,    3,    1);
                sumL = _mm_shuffle_epi8(sumL, mask);
                sumH = _mm_shuffle_epi8(sumH, mask);

                __m128i result = (__m128i) (_mm_movelh_ps((__m128)sumL, (__m128)sumH));

                _mm_storeu_si128((__m128i*)(mem + INDEX), result);
            }
        }

        PrintFPS(&counter);
        txRedrawWindow();
    }

    return NOERR;
}

HDC LoadImg(char name[], int x, int y, RGBQUAD** pixels)
{
    HDC dc = txLoadImage(name, x, y, IMAGE_BITMAP);

    if (dc == NULL)
    {
        printf("Cannot opend image %s", name);
        return NULL;
    }

    HDC newdc = txCreateDIBSection(SIZEX, SIZEY, pixels);
    txBitBlt(newdc, (SIZEX - x) / 2, (SIZEY - y)/2, x, y, dc);

    txDeleteDC(dc);
    return newdc;
}

int PrintFPS(int* counter)
{
    if (*counter < 100)
    {
        (*counter)++;
    }
    else
    {
        for (int i = 0; i < 20; i++)
        {
            printf("\b");
        }
        printf("FPS = %.5lg", txGetFPS() * 100);
        *counter = 0;
    }

    return 0;
}
