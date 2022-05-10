//
// Created by Yonadav on 20/4/22.
//
#include "/home/mtm/public/2122b/ex1/RLEList.h"
#include "AsciiArtTool.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

#define FLAG 1
#define SOURCE 2
#define TARGET 3
#define NEEDED_ARGS 4

/**
* invertingFunc - Inverts a space to a @ and a @ to a space
*
* @param letter - The letter to potentially invert.
 *
 * @return
 *  @ if letter is a space
 *  a space if the letter is @
 *  the letter if it is neither
*/
static char invertingFunc(char letter);


int main(int argc, char** argv){
    if (argc != NEEDED_ARGS) {
        fprintf(stderr, "Invalid arguments. Correct format is: ./AsciiArtTool <flags> <source> <target>");
        return 0;
    }
    if(strcmp(argv[FLAG] ,"-i") != 0 && strcmp(argv[FLAG],"-e") != 0){
        fprintf(stderr, "Invalid flag. flags are: -i for inverted output, -e for encoded output."
                        "only one flag is possible at a time");
        return 0;
    }
    FILE* input = fopen(argv[SOURCE],"r");
    RLEList src = asciiArtRead(input);
    if(!src){
        fprintf(stderr, "Error: Source file not found");
        return 0;
    }
    FILE *out = fopen(argv[TARGET], "w");
    if(!out){
        fprintf(stderr,"Memory allocation error");
        RLEListDestroy(src);
        return 0;
    }
    if(strcmp(argv[FLAG],"-i") == 0){ //prints the inverted source into the target
        RLEListMap(src, invertingFunc);
        asciiArtPrint(src, out);
    }
    else { //prints the encoded source into the target
        asciiArtPrintEncoded(src, out);
    }
    RLEListDestroy(src);
    fclose(input);
    fclose(out);
    return 0;
}


static char invertingFunc(char letter) {
    if (letter == ' ') {
        return '@';
    }
    if (letter == '@') {
        return ' ';
    }
    return letter;
}
