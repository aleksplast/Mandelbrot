#ifndef ALPHABLENDING_H
#define ALPHABLENDING_H

int alphablendNOSSE(RGBQUAD* back, RGBQUAD* front);

int alphablendSSE(RGBQUAD* back, RGBQUAD* front);

HDC LoadImg(char name[], int x, int y, RGBQUAD** pixels);

int PrintFPS(int* counter);

enum Errors
{
    NOERR = 0,      ///<No errors occured
    INPUTERR,     ///<Error with input file
    MEMERR,       ///<Error with memory
    FILERR        ///<Error with file
};


#endif //ALPHABLENDING_H
