#ifndef NO_SSE_H
#define NO_SSE_H

int mandelbrot();

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
