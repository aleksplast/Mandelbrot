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

int CountColor(__m256 x, __m256 y, BYTE* colors);

int Hotkeys(parameters* params);

int PrintFPS(int* counter);

bool IsZero(__m256 arr);

int SetInitialParams(parameters* params);

enum Errors
{
    NOERR = 0,      ///<No errors occured
    INPUTERR,     ///<Error with input file
    MEMERR,       ///<Error with memory
    FILERR        ///<Error with file
};

#endif      //NO_SSE_H
