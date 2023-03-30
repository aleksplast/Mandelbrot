#ifndef NO_SSE_H
#define NO_SSE_H

struct parameters
{
    float width;
    float height;

    float xstart;
    float ystart;
};


int mandelbrot();

int CountColor(__m128 x, __m128 y, parameters* params, BYTE* colors);

int PrintFPS();

bool IsZero(__m128 arr);

int SetInitialParams(parameters* params);

enum Errors
{
    NOERR = 0,      ///<No errors occured
    INPUTERR,     ///<Error with input file
    MEMERR,       ///<Error with memory
    FILERR        ///<Error with file
};

#endif      //NO_SSE_H
