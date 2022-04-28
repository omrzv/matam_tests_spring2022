#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../RLEList.h"
#include "test_utilities.h"

typedef bool (*testFunc)(void);

void copyStrHelper(char* source, char* dest);
void removeFromStringHelper(char* string, int index);
int weirdHaserHelper(int number);
int tenPower(int n);
int logTenRoundUp(int num);
char* simpleStrToExportedHelper(char* source);
bool strCompareHelper(char* string1, char* string2);

bool basicTest();
bool basicTestMacros(); 
bool RLEListCreateTest(); 
bool RLEListDestroyTest(); 
bool RLEListAppendTest();
bool RLEListSizeTest();
bool RLEListRemoveTest(); 
bool RLEListGetTest();
bool RLEListExportToStringTest(); 
bool RLEListMapTest();
bool RLEListMapAndExportTest();

#define TESTS_NAMES             \
    X(basicTest)               \
    X(basicTestMacros) \
    X(RLEListCreateTest) \
    X(RLEListDestroyTest) \
    X(RLEListAppendTest) \
    X(RLEListSizeTest) \
    X(RLEListRemoveTest) \
    X(RLEListGetTest) \
    X(RLEListExportToStringTest) \
    X(RLEListMapTest) \
    X(RLEListMapAndExportTest)

testFunc tests[] = {
#define X(name) name,
    TESTS_NAMES
#undef X
};

const char *tests_names[] = {
#define X(name) #name,
    TESTS_NAMES
#undef X
};

#define MAKE_LIST_WITH_ASSERT(__list, __string_raw, __goto_label) \
    do { \
        char* __string = (__string_raw); \
        (__list) = RLEListCreate(); \
        ASSERT_TEST((__list) != NULL, __goto_label); \
        while (*__string){ \
            ASSERT_TEST(RLEListAppend((__list), *(__string++)) == RLE_LIST_SUCCESS, __goto_label); \
        } \
    } while (0)

#define ASSERT_TEST_FULL_LIST(__list, __string_raw, __goto_label) \
    do { \
        char* __string = (__string_raw); \
        ASSERT_TEST((__list) != NULL, __goto_label); \
        RLEListResult __result; \
        int __i = 0; \
        while (*__string) \
        { \
            char __currentChar = RLEListGet((__list), __i, &__result); \
            if(__result != RLE_LIST_SUCCESS){ \
                printf("\nAssertion failed at %s:%d %s ", __FILE__, __LINE__, "__result != RLE_LIST_SUCCESS"); \
                printf("\nList get failed on index=%d in list, expected=%c, result was=%d", __i, *__string, __result); \
                result = false; \
                goto __goto_label; \
            } \
            if(__currentChar != *(__string)){ \
                printf("\nAssertion failed at %s:%d %s ", __FILE__, __LINE__, "__currentChar == *(__string++)"); \
                printf("\nString cmp failed on index=%d in list, got=%c and expected=%c", \
                       __i, __currentChar, *(__string)); \
                result = false; \
                goto __goto_label; \
            } \
            ++__string, ++__i; \
        } \
        ASSERT_TEST(RLEListGet(__list, __i, &__result) == 0, __goto_label); \
        ASSERT_TEST(__result == RLE_LIST_INDEX_OUT_OF_BOUNDS, __goto_label); \
    } while (0)

static int number_of_tests = sizeof(tests) / sizeof(tests[0]);

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        for (int test_idx = 0; test_idx < number_of_tests; test_idx++)
        {
            printf("%d ", test_idx + 1);
            RUN_TEST(tests[test_idx], tests_names[test_idx]);
        }
        return 0;
    }
    if (argc != 2)
    {
        fprintf(stdout, "Usage: tests <test index>\n");
        return 0;
    }

    int test_idx = strtol(argv[1], NULL, 10);
    if (test_idx < 1 || test_idx > number_of_tests)
    {
        fprintf(stderr, "Invalid test index %d\n", test_idx);
        return 0;
    }

    RUN_TEST(tests[test_idx - 1], tests_names[test_idx - 1]);
    printf("%d ", test_idx);
    return 0;
}

void copyStrHelper(char* source, char* dest)
{
    // This is NOT a safe function, it just assumes dest is writeable and large enough
    while (*source)
    {
        *(dest++) = *(source++);
    }
    *dest = '\0';
}

void removeFromStringHelper(char* string, int index)
{
    // This is NOT a safe function, it just assumes dest is writeable and large enough
    string += index + 1;
    while (*string)
    {
        *(string - 1) = *string;
        string++;
    }
    *(string - 1) = '\0';
}

// Returns a number which is between 0 and 4, and is very chaotic in relation to number
int weirdHaserHelper(int number)
{
    int hash = 3;
    while (number > 0)
    {
        hash = ((hash << 5) + hash + (number % 11)) % 5;
        number = number / 11;
    }
    return hash;
}

int tenPower(int n)
{
    int res = 1;
    while (n--)
    {
        res *= 10;
    }
    return res;
}


int logTenRoundUp(int num)
{
    int logTenOfNum = 0;
    while (num > 0)
    {
        logTenOfNum++;
        num /= 10;
    }
    return logTenOfNum;
}

char* simpleStrToExportedHelper(char* source)
{
    int stringSize = 0;
    int count = 0;
    char last = '\0';
    char* runner = source;
    while (*runner){
        if (last != *runner){
            stringSize += 2 + logTenRoundUp(count);
            last = *runner;
            count = 1;
        }
        else
        {
            count++;
        }
        ++runner;
    }
    stringSize += logTenRoundUp(count);

    char* toReturn = malloc((stringSize + 1) * sizeof(char));
    char* string = toReturn;

    last = '\0';
    runner = source;
    while (*runner){
        if (last != *runner){
            if (last != '\0'){
                *(string++) = last;
                int logTenAmount = logTenRoundUp(count);
                for (int powerOfTen = logTenAmount - 1; powerOfTen >= 0; --powerOfTen)
                {
                    int digit = ((count / tenPower(powerOfTen)) % 10);
                    *(string++) = '0' + digit;
                }
                *(string++) = '\n';
            }
            last = *runner;
            count = 1;
        }
        else
        {
            count++;
        }
        ++runner;
    }

    *(string++) = last;
    int logTenAmount = logTenRoundUp(count);
    for (int powerOfTen = logTenAmount - 1; powerOfTen >= 0; --powerOfTen)
    {
        int digit = ((count / tenPower(powerOfTen)) % 10);
        *(string++) = '0' + digit;
    }
    *(string++) = '\n';
    *(string++) = '\0';

    return toReturn;
}

bool strCompareHelper(char* string1, char* string2)
{
    while (*string1 && *string2)
    {
        if (*(string1++) != *(string2++))
        {
            return false;
        }
    }
    return *(string1) == *(string2);
}


bool basicTest()
{
    RLEList list = RLEListCreate();
    bool result=true;
    ASSERT_TEST(list != NULL, destroy);

    //adding elements to the list
    ASSERT_TEST(RLEListAppend(list, 'a') == RLE_LIST_SUCCESS, destroy);    // a
    ASSERT_TEST(RLEListAppend(list, 'c') == RLE_LIST_SUCCESS, destroy);    // ac
    ASSERT_TEST(RLEListAppend(list, 'b') == RLE_LIST_SUCCESS, destroy);    // acb
    ASSERT_TEST(RLEListAppend(list, 'a') == RLE_LIST_SUCCESS, destroy);    // acba
    ASSERT_TEST(RLEListAppend(list, 'b') == RLE_LIST_SUCCESS, destroy);    // acbab
    ASSERT_TEST(RLEListAppend(list, 'a') == RLE_LIST_SUCCESS, destroy);    // acbaba
    ASSERT_TEST(RLEListAppend(list, 'b') == RLE_LIST_SUCCESS, destroy);    // acbabab
    ASSERT_TEST(RLEListAppend(list, 'a') == RLE_LIST_SUCCESS, destroy);    // acbababa
    ASSERT_TEST(RLEListAppend(list, 'a') == RLE_LIST_SUCCESS, destroy);    // acbababaa
    ASSERT_TEST(RLEListAppend(list, 'a') == RLE_LIST_SUCCESS, destroy);    // acbababaaa

    ASSERT_TEST(RLEListRemove(list, 1) == RLE_LIST_SUCCESS, destroy); // abababaaa

    // check if the represented string is "abababaaa"
    const char *s = "abababaaa";
    char it;
    for(int i=0; i<RLEListSize(list); i++)
    {
        it=RLEListGet(list, i, NULL);
        ASSERT_TEST(it == s[i++], destroy);
    }
    //check if the length's are equal
    ASSERT_TEST(RLEListSize(list)==strlen(s), destroy);
    destroy:
    RLEListDestroy(list);
    return result;
}

bool basicTestMacros()
{
    bool result = true;
    RLEList list;
    
    char* testString = "Hello\nI love you\nwon't you tell me your name?\n\n\n\n\n\nuuuuusssss!!!.^^^    ---";
    MAKE_LIST_WITH_ASSERT(list, testString, destroy);
    ASSERT_TEST_FULL_LIST(list, testString, destroy);
    RLEListDestroy(list);

    testString = "totaly didn't code this after the seder... im a normal person who doesnt do such things XOXO";
    MAKE_LIST_WITH_ASSERT(list, testString, destroy);
    ASSERT_TEST_FULL_LIST(list, testString, destroy);

    destroy:
    RLEListDestroy(list);
    return result;
}

bool RLEListCreateTest()
{
    bool result = true;

    RLEList list = RLEListCreate();
    RLEList list2 = RLEListCreate();

    ASSERT_TEST(list != NULL, destory);
    ASSERT_TEST(RLEListSize(list) == 0, destory);

    ASSERT_TEST(list2 != NULL, destory);
    ASSERT_TEST(RLEListSize(list2) == 0, destory);

    ASSERT_TEST(list != list2, destory);

    destory:
    RLEListDestroy(list);
    RLEListDestroy(list2);
    return result;
}


/**
* RLEListDestroy: Deallocates an existing RLE list.
*
* @param list - RLE list to be deallocated. If RLE list is NULL nothing will be done
*/
//void RLEListDestroy(RLEList list);
bool RLEListDestroyTest()
{
    return true; // There is literaly no way to test if a pointer was freed. I DUNNO MAN
}


/**
*   RLEListAppend: add a specified character at the end of the list.
*
* @param list - The RLE list for which to add the character
* @param value - The character which needs to be added.
* @return
* 	RLE_LIST_NULL_ARGUMENT if a NULL was sent as one of the parameters
* 	RLE_LIST_OUT_OF_MEMORY if an allocation failed
* 	RLE_LIST_SUCCESS if the character has been inserted successfully
*/
//RLEListResult RLEListAppend(RLEList list, char value);
bool RLEListAppendTest()
{
    bool result = true;
    RLEList list = RLEListCreate();
    ASSERT_TEST(RLEListAppend(NULL, 'a') == RLE_LIST_NULL_ARGUMENT, destroy);
    ASSERT_TEST(RLEListAppend(list, 'a') == RLE_LIST_SUCCESS, destroy);
    RLEListResult getResult;
    ASSERT_TEST(RLEListGet(list, 0, &getResult) == 'a', destroy);
    RLEListGet(list, 1, &getResult);
    ASSERT_TEST(getResult == RLE_LIST_INDEX_OUT_OF_BOUNDS, destroy);
    
    ASSERT_TEST(RLEListAppend(list, 'a') == RLE_LIST_SUCCESS, destroy);
    ASSERT_TEST(RLEListGet(list, 0, &getResult) == 'a', destroy);
    ASSERT_TEST(RLEListGet(list, 1, &getResult) == 'a', destroy);
    RLEListGet(list, 2, &getResult);
    ASSERT_TEST(getResult == RLE_LIST_INDEX_OUT_OF_BOUNDS, destroy);

    ASSERT_TEST(RLEListAppend(list, 'b') == RLE_LIST_SUCCESS, destroy);
    ASSERT_TEST(RLEListGet(list, 0, &getResult) == 'a', destroy);
    ASSERT_TEST(RLEListGet(list, 1, &getResult) == 'a', destroy);
    ASSERT_TEST(RLEListGet(list, 2, &getResult) == 'b', destroy);
    RLEListGet(list, 3, &getResult);
    ASSERT_TEST(getResult == RLE_LIST_INDEX_OUT_OF_BOUNDS, destroy);

    ASSERT_TEST(RLEListAppend(list, 'a') == RLE_LIST_SUCCESS, destroy);
    ASSERT_TEST(RLEListGet(list, 0, &getResult) == 'a', destroy);
    ASSERT_TEST(RLEListGet(list, 1, &getResult) == 'a', destroy);
    ASSERT_TEST(RLEListGet(list, 2, &getResult) == 'b', destroy);
    ASSERT_TEST(RLEListGet(list, 3, &getResult) == 'a', destroy);
    RLEListGet(list, 4, &getResult);
    ASSERT_TEST(getResult == RLE_LIST_INDEX_OUT_OF_BOUNDS, destroy);

    destroy:
    RLEListDestroy(list);
    return result;
}

/**
* RLEListSize: Returns the total number of characters in an RLE list.
* @param list - The RLE list whose size is requested
* @return
* 	-1 if a NULL pointer was sent.
* 	Otherwise the total number of characters in the list.
*/
//int RLEListSize(RLEList list);
bool RLEListSizeTest()
{
    bool result = true;
    RLEList list;
    MAKE_LIST_WITH_ASSERT(list, "123456789", destroy);


    ASSERT_TEST(RLEListSize(NULL) == -1, destroy);
    
    ASSERT_TEST(RLEListSize(list) == 9, destroy);

    RLEListDestroy(list);

    MAKE_LIST_WITH_ASSERT(list, "aaaaabaaabsssaadddddssssaaaa,,,,asd,f.ld,f;,b3pij4i4343oofdjbvc,..,.,.,,amdsmlkvmalfdmbioadhbpxchaeb\\-asdfajeijjvh0230jzkjxcvoxji09``12", destroy);
    ASSERT_TEST(RLEListSize(list) == 135, destroy);

    for (int i = 0; i < 20; ++i)
    {
        ASSERT_TEST(RLEListRemove(list, i) == RLE_LIST_SUCCESS, destroy);
        ASSERT_TEST(RLEListSize(list) == 135 - i - 1, destroy);
    }

    for (int i = 0; i < 20; ++i)
    {
        ASSERT_TEST(RLEListAppend(list, 'a' + (i/ 2)) == RLE_LIST_SUCCESS, destroy);
        ASSERT_TEST(RLEListSize(list) == 115 + i + 1, destroy);
    }

    destroy:
    RLEListDestroy(list);
    return result;
}

/**
*   RLEListRemove: Removes a character found at a specified index in an RLE list.
*
* @param list - The RLE list to remove the character from.
* @param index - The index at which the character to be removed is found.
* @return
* 	RLE_LIST_NULL_ARGUMENT if a NULL was sent to the function.
* 	RLE_LIST_INDEX_OUT_OF_BOUNDS if given index is not withing the list's bounds.
* 	RLE_LIST_SUCCESS the character found at index has been removed successfully.
*/
//RLEListResult RLEListRemove(RLEList list, int index);
bool RLEListRemoveTest()
{
    bool result = true;
    RLEList list = RLEListCreate();
    char *testString = (char *) malloc(54 * sizeof(char));

    ASSERT_TEST(RLEListRemove(NULL, 0) == RLE_LIST_NULL_ARGUMENT, destroy);

    for (int i = -1000; i < 1000; i++)
    {
        ASSERT_TEST(RLEListRemove(list, i * i * i * i * i) == RLE_LIST_INDEX_OUT_OF_BOUNDS, destroy);
    }

    RLEListDestroy(list);
    copyStrHelper("11145678901234567890125556789012345678901234567000", testString);
    MAKE_LIST_WITH_ASSERT(list, testString, destroy); // 50 chars
    for (int i = 49; i >= 0; --i)
    {
        ASSERT_TEST(RLEListRemove(list, 0) == RLE_LIST_SUCCESS, destroy);
        removeFromStringHelper(testString, 0);
        ASSERT_TEST_FULL_LIST(list, testString, destroy);
    }

    
    RLEListDestroy(list);
    copyStrHelper("199999999999999999999999999999999999999999999999990", testString);
    MAKE_LIST_WITH_ASSERT(list, testString, destroy); // 51 chars
    for (int i = 50; i >= 0; --i)
    {
        ASSERT_TEST(RLEListRemove(list, i) == RLE_LIST_SUCCESS, destroy);
        removeFromStringHelper(testString, i);
        ASSERT_TEST_FULL_LIST(list, testString, destroy);
    }

    RLEListDestroy(list);
    copyStrHelper("1999999999999999999__99999999999999999999999999999KKK", testString);
    MAKE_LIST_WITH_ASSERT(list, testString, destroy); // 53 chars
    for (int i = 52; i >= 0; --i)
    {
        ASSERT_TEST(RLEListRemove(list, i > 10? 10 : i) == RLE_LIST_SUCCESS, destroy);
        removeFromStringHelper(testString, i > 10? 10 : i);
        ASSERT_TEST_FULL_LIST(list, testString, destroy);
    }

    destroy:
    free(testString);
    RLEListDestroy(list);
    return result;
}

/**
*   RLEListGet: Returns the character found at a specified index in an RLE list.
*
* @param list - The RLE list to retrieve the character from.
* @param index - The index at which the character to be retrieved is found.
* @param result - Pointer to be used to store the result of the operation, if it is NULL, the result will not be saved.
* 	RLE_LIST_NULL_ARGUMENT if a NULL was sent to the function as list.
* 	RLE_LIST_INDEX_OUT_OF_BOUNDS if given index is not withing the list's bounds.
* 	RLE_LIST_SUCCESS the character found at index has been retrieved successfully.
* @return
* 	0 if result is not RLE_LIST_SUCCESS.
* 	The character found at given index in case of success.   
*/
//char RLEListGet(RLEList list, int index, RLEListResult *result);
bool RLEListGetTest()
{
    bool result = true;
    RLEList list = RLEListCreate();
    RLEListResult getResult;
    char testString[16384];

    ASSERT_TEST(RLEListGet(NULL, 0, &getResult) == 0, destroy);
    ASSERT_TEST(getResult == RLE_LIST_NULL_ARGUMENT, destroy);

    for (int i = -1000; i < 1000; i++)
    {
        ASSERT_TEST(RLEListGet(list, i * i * i * i * i, &getResult) == 0, destroy);
        ASSERT_TEST(getResult == RLE_LIST_INDEX_OUT_OF_BOUNDS, destroy);
    }

    int i = 0;
    for (int stringLen = 1; stringLen < 16384; stringLen = 2 * stringLen + 1)
    {
        // Setup the string
        for (int j = 0; j < stringLen; j++)
        {
            testString[j] = '0' + weirdHaserHelper(i++);
        }
        testString[stringLen] = '\0';

        // Setup the list
        RLEListDestroy(list);
        MAKE_LIST_WITH_ASSERT(list, testString, destroy);

        // Tests
        ASSERT_TEST(RLEListGet(list, -1, &getResult) == 0, destroy);
        ASSERT_TEST(getResult == RLE_LIST_INDEX_OUT_OF_BOUNDS, destroy);

        ASSERT_TEST(RLEListGet(list, stringLen, &getResult) == 0, destroy);
        ASSERT_TEST(getResult == RLE_LIST_INDEX_OUT_OF_BOUNDS, destroy);

        for (int j = 0; j < stringLen; j++)
        {
            ASSERT_TEST(RLEListGet(list, j, &getResult) == testString[j], destroy);
            ASSERT_TEST(getResult == RLE_LIST_SUCCESS, destroy);
        }
    }

    destroy:
    RLEListDestroy(list);
    return result;
}



/**
*   RLEListExportToString: Returns the characters found in an RLE list as a string.
*
* @param list - The RLE list to retrieve the characters from.
* @param result - Pointer to be used to store the result of the operation, if it is NULL, the result will not be saved.
* 	RLE_LIST_NULL_ARGUMENT if a NULL was sent to the function as list.
* 	RLE_LIST_INDEX_OUT_OF_BOUNDS if given index is not withing the list's bounds.
* 	RLE_LIST_SUCCESS the character found at index has been retrieved successfully.
* @return
* 	NULL if result is not RLE_LIST_SUCCESS.
* 	The string that correspondent to the received RLE list.   
*/
//char* RLEListExportToString(RLEList list, RLEListResult* result);
bool RLEListExportToStringTest()
{
    bool result = true;
    RLEList list = RLEListCreate();
    RLEListResult exportResult;
    char* actuallResult = NULL;
    char* comparer;

    ASSERT_TEST(RLEListExportToString(NULL, &exportResult) == NULL, destroy);
    ASSERT_TEST(exportResult == RLE_LIST_NULL_ARGUMENT, destroy);
    actuallResult = RLEListExportToString(list, NULL);
    ASSERT_TEST(*actuallResult == '\0', destroy);

    RLEListDestroy(list);
    MAKE_LIST_WITH_ASSERT(list, "ABBabb------------\n\na", destroy);
    char* expectedResult = "A1\nB2\na1\nb2\n-12\n\n2\na1\n";
    free(actuallResult);
    actuallResult = RLEListExportToString(list, &exportResult);
    
    ASSERT_TEST(exportResult == RLE_LIST_SUCCESS, destroy);
    comparer = actuallResult;
    while (*expectedResult)
    {
        ASSERT_TEST(*comparer != '\0', destroy);
        ASSERT_TEST(*(expectedResult++) == *(comparer++), destroy);
    }
    ASSERT_TEST(*comparer == '\0', destroy);

    ASSERT_TEST(RLEListRemove(list, 0) == RLE_LIST_SUCCESS, destroy);
    ASSERT_TEST(RLEListRemove(list, 2) == RLE_LIST_SUCCESS, destroy);
    ASSERT_TEST(RLEListRemove(list, 4) == RLE_LIST_SUCCESS, destroy);
    ASSERT_TEST(RLEListRemove(list, 6) == RLE_LIST_SUCCESS, destroy);
    ASSERT_TEST(RLEListRemove(list, 13) == RLE_LIST_SUCCESS, destroy);
    ASSERT_TEST(RLEListRemove(list, 15) == RLE_LIST_SUCCESS, destroy);
    ASSERT_TEST(RLEListAppend(list, 'a') == RLE_LIST_SUCCESS, destroy);
    ASSERT_TEST(RLEListAppend(list, '\n') == RLE_LIST_SUCCESS, destroy);
    ASSERT_TEST(RLEListRemove(list, 15) == RLE_LIST_SUCCESS, destroy);
    expectedResult = "B2\nb2\n-9\n\n3\n";
    free(actuallResult);
    actuallResult = RLEListExportToString(list, &exportResult);
    
    ASSERT_TEST(exportResult == RLE_LIST_SUCCESS, destroy);
    comparer = actuallResult;
    while (*expectedResult)
    {
        ASSERT_TEST(*comparer != '\0', destroy);
        ASSERT_TEST(*(expectedResult++) == *(comparer++), destroy);
    }
    ASSERT_TEST(*comparer == '\0', destroy);

    RLEListDestroy(list);
    MAKE_LIST_WITH_ASSERT(list, "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^", destroy);
    expectedResult = "^123\n";
    free(actuallResult);
    actuallResult = RLEListExportToString(list, &exportResult);
    
    ASSERT_TEST(exportResult == RLE_LIST_SUCCESS, destroy);
    comparer = actuallResult;
    while (*expectedResult)
    {
        ASSERT_TEST(*comparer != '\0', destroy);
        ASSERT_TEST(*(expectedResult++) == *(comparer++), destroy);
    }

    RLEListDestroy(list);
    list = RLEListCreate();
    expectedResult = "";
    free(actuallResult);
    actuallResult = RLEListExportToString(list, &exportResult);
    
    ASSERT_TEST(exportResult == RLE_LIST_SUCCESS, destroy);
    comparer = actuallResult;
    while (*expectedResult)
    {
        ASSERT_TEST(*comparer != '\0', destroy);
        ASSERT_TEST(*(expectedResult++) == *(comparer++), destroy);
    }

    destroy:
    RLEListDestroy(list);
    free(actuallResult);
    return result;
}

/**
*   RLEListMap: Change the given RLE list's characters according to the received mapping function.
*               This function replaces each character of the give RLE list with its mapped character.
*
* @param list - The RLE list to edit.
* @param MapFunction - Pointer to a function of type MapFunction. 
* @return
* 	RLE_LIST_NULL_ARGUMENT if a NULL was sent as a paramater.
* 	LIST_SUCCESS if the mapping is done successfully.
*/
char map1(char a){return 'a';}
char map2(char a){return (a - 'a' + 1) % 24 + 'a';}
char map3(char a){return a != ' '? (a != '@'? a : ' ') : '@';}
char map4(char a){return a;}
char map5(char a){return a == ' '? a: '\n';}

//RLEListResult RLEListMap(RLEList list, MapFunction map_function);
bool RLEListMapTest()
{
    bool result = true;
    const int FUNC_COUNT = 5;
    MapFunction mapFunctions[5] = {map1, map2, map3, map4, map5};
    RLEList list = RLEListCreate();

    ASSERT_TEST(RLEListMap(NULL, map1) == RLE_LIST_NULL_ARGUMENT, destroy);
    ASSERT_TEST(RLEListMap(list, NULL) == RLE_LIST_NULL_ARGUMENT, destroy);
    ASSERT_TEST(RLEListMap(NULL, NULL) == RLE_LIST_NULL_ARGUMENT, destroy);

    char* text;
    char mappedText[124] = {0};
    
    text = "ABBabb------------\n\na";
    for (int i = 0; i < FUNC_COUNT; i++)
    {
        RLEListDestroy(list);
        MAKE_LIST_WITH_ASSERT(list, text, destroy);
        ASSERT_TEST(RLEListMap(list, mapFunctions[i]) == RLE_LIST_SUCCESS, destroy);
        char* mappedRunner = mappedText;
        for (char* runner = text; *runner; runner++)
        {
            *(mappedRunner++) = mapFunctions[i](*runner);
        }

        ASSERT_TEST_FULL_LIST(list, mappedText, destroy);
    }

    text = "ABBabb------------\n\na";
    for (int i = 0; i < FUNC_COUNT; i++)
    {
        RLEListDestroy(list);
        MAKE_LIST_WITH_ASSERT(list, text, destroy);
        ASSERT_TEST(RLEListMap(list, mapFunctions[i]) == RLE_LIST_SUCCESS, destroy);
        char* mappedRunner = mappedText;
        for (char* runner = text; *runner; runner++)
        {
            *(mappedRunner++) = mapFunctions[i](*runner);
        }

        ASSERT_TEST_FULL_LIST(list, mappedText, destroy);
    }

    text = "__!@4 f g;dk\nfs m23mbf;erlk;lk;lkl;;\n;;\n \n     4l4ll1;23lk4;1lm.d, vdsm.vm.wlekm3k4";
    for (int i = 0; i < FUNC_COUNT; i++)
    {
        RLEListDestroy(list);
        MAKE_LIST_WITH_ASSERT(list, text, destroy);
        ASSERT_TEST(RLEListMap(list, mapFunctions[i]) == RLE_LIST_SUCCESS, destroy);
        char* mappedRunner = mappedText;
        for (char* runner = text; *runner; runner++)
        {
            *(mappedRunner++) = mapFunctions[i](*runner);
        }

        ASSERT_TEST_FULL_LIST(list, mappedText, destroy);
    }

    text = ")))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))";
    for (int i = 0; i < FUNC_COUNT; i++)
    {
        RLEListDestroy(list);
        MAKE_LIST_WITH_ASSERT(list, text, destroy);
        ASSERT_TEST(RLEListMap(list, mapFunctions[i]) == RLE_LIST_SUCCESS, destroy);
        char* mappedRunner = mappedText;
        for (char* runner = text; *runner; runner++)
        {
            *(mappedRunner++) = mapFunctions[i](*runner);
        }

        ASSERT_TEST_FULL_LIST(list, mappedText, destroy);
    }

    text = "";
    
    RLEListDestroy(list);
    MAKE_LIST_WITH_ASSERT(list, text, destroy);
    ASSERT_TEST(RLEListMap(list, map3) == RLE_LIST_SUCCESS, destroy);
    ASSERT_TEST_FULL_LIST(list, text, destroy);


    destroy:
    RLEListDestroy(list);
    return result;
}

bool RLEListMapAndExportTest()
{
    bool result = true;
    const int FUNC_COUNT = 5;
    MapFunction mapFunctions[5] = {map1, map2, map3, map4, map5};
    RLEList list = RLEListCreate();

    char* text;
    char* expectedString;
    char* resultString;
    char mappedText[124] = {0};
    
    text = "Yes if this fails for you after you submitted it sucks :D! \n \n \n ";
    for (int i = 0; i < FUNC_COUNT; i++)
    {
        RLEListDestroy(list);
        MAKE_LIST_WITH_ASSERT(list, text, destroy);
        ASSERT_TEST(RLEListMap(list, mapFunctions[i]) == RLE_LIST_SUCCESS, destroy);
        char* mappedRunner = mappedText;
        for (char* runner = text; *runner; runner++)
        {
            *(mappedRunner++) = mapFunctions[i](*runner);
        }

        expectedString = simpleStrToExportedHelper(mappedText);
        resultString = RLEListExportToString(list, NULL);
        if (!strCompareHelper(expectedString, resultString)){
            printf("\nAssertion failed at %s:%d %s ", __FILE__, __LINE__, "expectedString == resultString");
            printf("\nexpectedString:\n%s\nresultString:\n%s", expectedString, resultString);
            result = false;
            free(expectedString);
            free(resultString);
            goto destroy;
        }
        free(expectedString);
        free(resultString);
    }

    destroy:
    RLEListDestroy(list);
    return result;
}
