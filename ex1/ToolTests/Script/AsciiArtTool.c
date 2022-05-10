#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "AsciiArtTool.h"
#include "/home/mtm/public/2122b/ex1/RLEList.h"

RLEList asciiArtRead(FILE* in_stream)
{
    if (in_stream == NULL)
    {
        return NULL;
    }
    RLEList list = RLEListCreate();
    while (1) //until EOF
    {
        char c = fgetc(in_stream);
        if(feof(in_stream)){
            break;
        }
        RLEListAppend(list, c); //adds next word to the list
    }

    return list;
}
RLEListResult asciiArtPrint(RLEList list, FILE *out_stream)
{
    if (!list)
    {
        return RLE_LIST_NULL_ARGUMENT;
    }
    RLEListResult ptr;
    for (int i = 0; i < RLEListSize(list); i++)
    {
        char tmp = RLEListGet(list, i, &ptr); //the letter in index i
        if (ptr != RLE_LIST_SUCCESS)
        {
            return RLE_LIST_ERROR;
        }
        fputc(tmp, out_stream); //put the letter in the file
    }
    return RLE_LIST_SUCCESS;
}
RLEListResult asciiArtPrintEncoded(RLEList list, FILE *out_stream)
{
    if (!list)
    {
        return RLE_LIST_NULL_ARGUMENT;
    }
    RLEListResult ptr;
    char* str = RLEListExportToString(list, &ptr);
    if(str == NULL){ //file is empty
        free(str);
        return RLE_LIST_SUCCESS;
    }
    fputs(str, out_stream); //put str in the file
    free(str);
    if (ptr != RLE_LIST_SUCCESS)
    {
        return RLE_LIST_ERROR;
    }
    return RLE_LIST_SUCCESS;
}
