#include <stdio.h>
#include "TXLib.h"
#include "alphablending.h"

int sizex = 800;
int sizey = 800;

#pragma GCC diagnostic ignored "-Wconversion"

int main()
{
    char catpath[30] = "AskhatCat.bmp";
    char tablepath[30] = "Table.bmp";

    RGBQUAD* catpix = NULL;
    HDC cat = LoadImg(catpath, 200, 200, &catpix);
    RGBQUAD* tablepix = NULL;
    HDC table = LoadImg(tablepath, sizex, sizey, &tablepix);

    alphablendNOSSE(tablepix, catpix);

    txDeleteDC(cat);
    txDeleteDC(table);

    return 0;
}
