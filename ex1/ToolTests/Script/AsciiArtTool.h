#ifndef HW1_ASCIIARTTOOL_H
#define HW1_ASCIIARTTOOL_H

#include <stdio.h>
#include <stdbool.h>
#include "/home/mtm/public/2122b/ex1/RLEList.h"

/**
* Run Length Encoding List
*
* handling picture representation by RLE list
*
* The following functions are available:
*   asciiArtRead	        - Compresses the picture file to RLE list.
*   asciiArtPrint		    - Draws a picture according to a given RLE list.
*   asciiArtPrintEncoded    - Writes the exported string of RLE list to output file.
*/


/**
* asciiArtRead: Compresses the picture file to RLE list.
*
* @param in_stream - picture file to be compressed.
*
* @return 
*     NULL if the file is null
*     the list which representing the picture in case of success
*/
RLEList asciiArtRead(FILE* in_stream);


/**
* asciiArtPrint: Draws a picture according to a given RLE list.
*
* @param list - The RLE list which we want to draw the picture by. 
* @param out_stream - output picture file. 
*
* @return
*     RLE_LIST_NULL_ARGUMENT if a NULL was sent as the list
*     RLE_LIST_SUCCESS if picture was drawn successfully to the file
*/
RLEListResult asciiArtPrint(RLEList list, FILE *out_stream);


/**
* asciiArtPrintEncoded: Writes the exported string of RLE list to output file.
*
* @param list - The RLE list which we want write its string to a file. 
* @param out_stream - exported string file. 
*
* @return
*     RLE_LIST_NULL_ARGUMENT if a NULL was sent as the list
*     RLE_LIST_SUCCESS if exported string was written successfully to the file
*/
RLEListResult asciiArtPrintEncoded(RLEList list, FILE *out_stream);

#endif // HW1_ASCIIARTTOOL_H
