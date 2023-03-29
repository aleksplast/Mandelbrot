#ifndef NO_SSE_H
#define NO_SSE_H

int mandelbrot();

int CountColor(__m128 x, __m128 y, BYTE* colors);

bool IsZero(__m128 arr);

struct point
{
    float x;
    float y;
};

enum Errors
{
    NOERR = 0,      ///<No errors occured
    INPUTERR,     ///<Error with input file
    MEMERR,       ///<Error with memory
    FILERR        ///<Error with file
};

#endif      //NO_SSE_H
