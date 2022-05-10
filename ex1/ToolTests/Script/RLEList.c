#include "/home/mtm/public/2122b/ex1/RLEList.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define RESULT_CHECK(ptr,res) ptr!=NULL?*ptr=res:0

typedef struct RLEList_t{
    char letter;
    int repetitions;
    struct RLEList_t* next;
}RLEList_t;


/**
*   RLEListGetMaxRepetitionChars: Scans a list and returns the string length of the longest repetitions in the list
*
* @param list - The RLE list to check
 * for example - returns 1 if the longest repetitions in a list's nodes is 7
 *               returns 2 if the longest repetitions in a list's nodes is 38
*/
static int RLEListGetMaxRepetitionChars(RLEList list);


/**
*   RLEListGetLen: returns a list's number of nodes
*
* @param list - The RLE list to check
*/
static int RLEListGetLen(RLEList list);


/**
*   RLEListRemoveSingle: Removes a list's next node. If firstNode is true, removes the list's first node
*
* @param list - The RLE list to remove from
* @param firstNode - true if the first node needs to be removed, false if the second node needs to be removed
*/
static void RLEListRemoveSingle(RLEList list, bool firstNode);


RLEList RLEListCreate(){
    RLEList newList=malloc(sizeof(*newList));
    if(!newList){
        return NULL;
    }
    newList->next=NULL;
    newList->letter=0;
    newList->repetitions=0;
    return newList;
}

void RLEListDestroy(RLEList list)
{
    while(list)
    {
        RLEList toDelete = list;
        list = list->next;
        free(toDelete);
    }
}

RLEListResult RLEListAppend(RLEList list, char value){
    if(!list || !value){
        return RLE_LIST_NULL_ARGUMENT;
    }
    //If the list's first node has 0 repetitions, it's a new list => puts the value in the first node
    if(list->repetitions==0){
        assert(!list->next);
        list->letter=value;
        list->repetitions++;
        return RLE_LIST_SUCCESS;
    }
    while(list->next){ //Moves to the last node in the list
        list=list->next;
    }
    if(list->letter==value){ //Adds the letter to the current repetitions if it's the same as in the last node
        list->repetitions++;
        return RLE_LIST_SUCCESS;
    }
    RLEList nextNode=RLEListCreate(); //Makes a new node to put the letter in
    if(!nextNode){
        return RLE_LIST_OUT_OF_MEMORY;
    }
    list->next=nextNode;
    nextNode->letter=value;
    nextNode->repetitions++;
    return RLE_LIST_SUCCESS;
}

int RLEListSize(RLEList list)
{
    if (list == NULL)
    {
        return -1;
    }
    int sum = 0;
    while(list)
    {
        sum += list->repetitions;
        list = list->next;
    }
    return sum;
}

RLEListResult RLEListRemove(RLEList list, int index){
    if(!list){
        return RLE_LIST_NULL_ARGUMENT;
    }
    if(index<0||index>RLEListSize(list)-1){
        return RLE_LIST_INDEX_OUT_OF_BOUNDS;
    }
    while(true){
        //moves up the list until it's on the right index
        //stops when the next node contains the required index (or if the first one does)
        if(list->repetitions < index+1 && list->next->repetitions < index-list->repetitions+1){
            assert(list->next);
            index-=list->repetitions;
            list=list->next;
        }
        else{
            if(list->repetitions >= index+1) { //If the index is in the first node
                if(list->repetitions <= 1) //If the first node contains one letter (or empty list)
                {
                    if (!list->next) { //if the list contains one node
                        list->letter = 0; //Resets the list to its default values (it's now an empty list)
                        list->repetitions = 0;
                        return RLE_LIST_SUCCESS;
                    }
                    else{ //if the list contains more nodes
                        RLEListRemoveSingle(list, true);
                        return RLE_LIST_SUCCESS;
                    }
                }
                else{ //if the first node contains more than one letter
                    list->repetitions--; //simply reduces repetitions by 1
                    return RLE_LIST_SUCCESS;
                }
            }
            else {//otherwise, the index is in the next node to the point reached
                assert(list->next);
                if (list->next->repetitions == 1) {//If the next node contains one letter
                    RLEListRemoveSingle(list, false);
                } else {
                    list->next->repetitions--; //if the next node contains more than one letter, simply reduces one from it.
                }
                return RLE_LIST_SUCCESS;
            }
        }
    }
}

char RLEListGet(RLEList list, int index, RLEListResult *result)
{
    if(!list)
    {
        RESULT_CHECK(result, RLE_LIST_NULL_ARGUMENT);
        return 0;
    }
    if (index < 0 || index > RLEListSize(list) - 1)
    {
        RESULT_CHECK(result, RLE_LIST_INDEX_OUT_OF_BOUNDS);
        return 0;
    }
    while (index >= 0)
    {
        index = index - list->repetitions;
        if (index < 0)
        {
            break;
        }
        list = list->next;
    }
    RESULT_CHECK(result, RLE_LIST_SUCCESS);
    return list->letter;
}

RLEListResult RLEListMap(RLEList list, MapFunction map_function){
    if(!list || !map_function){
        return RLE_LIST_NULL_ARGUMENT;
    }
    while(list){
        list->letter=map_function(list->letter);
        list=list->next;
    }
    return RLE_LIST_SUCCESS;
}

char* RLEListExportToString(RLEList list, RLEListResult* result)
{
    if(!list)
    {
        RESULT_CHECK(result, RLE_LIST_NULL_ARGUMENT);
        return NULL;
    }
    if(list->letter==0){//list is empty and nothing needs to be done
        RESULT_CHECK(result, RLE_LIST_SUCCESS);
        return NULL;
    }
    int maxRepetitionsChars= RLEListGetMaxRepetitionChars(list);
    int listLen= RLEListGetLen(list);
    char* str = malloc(((2 + maxRepetitionsChars) * listLen + 1) * sizeof(str));
    char* tmp = malloc((maxRepetitionsChars+1) * sizeof(tmp));
    if(!str || !tmp){
        free(str);
        free(tmp);
        RESULT_CHECK(result, RLE_LIST_OUT_OF_MEMORY);
        return NULL;
    }
    strcpy(str,""); //initializes the string
    while (list)
    {
        //copies the letter into the row
        sprintf(tmp, "%c", list->letter);
        strcat(str, tmp);

        //copies the repetitions into the row
        sprintf(tmp, "%d", list->repetitions);
        strcat(str, tmp);

        //goes down a row
        strcat(str, "\n");
        list = list->next;
    }
    RESULT_CHECK(result, RLE_LIST_SUCCESS);
    free(tmp);
    return str;

}

static int RLEListGetMaxRepetitionChars(RLEList list){
    int maxRepetitions=0;
    while(list){
        maxRepetitions= list->repetitions>maxRepetitions? list->repetitions:maxRepetitions;
        list=list->next;
    }
    int charCount=0;
    while(maxRepetitions>0){
        charCount++;
        maxRepetitions/=10;
    }
    return charCount;
}

static int RLEListGetLen(RLEList list){
    int len=0;
    while(list){
        len++;
        list=list->next;
    }
    return len;
}

static void RLEListRemoveSingle(RLEList list, bool firstNode){
    RLEList tmp = list->next; //makes a temp pointer to the next node
    list->next = tmp->next; //connects the current node to the next-next node
    if(firstNode) { //if it's the first node, fully changes it into the second node
        list->repetitions = tmp->repetitions;
        list->letter = tmp->letter;
    }
    //isolates and destroys the next node:
    tmp->next=NULL;
    RLEListDestroy(tmp);
    //if the next node in the new list has the same letter as the first node, connects the two nodes:
    if(list->next && list->letter == list->next->letter){
        list->repetitions+=list->next->repetitions; //combines the repetitions
        RLEListRemoveSingle(list, false); //removes the next node
    }
}
