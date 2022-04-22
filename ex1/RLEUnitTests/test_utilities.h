//
// Created by Assaf on 13/03/2022.
//

#ifndef MAIN_C_TEST_UTILITIES_H
#define MAIN_C_TEST_UTILITIES_H

#include <stdbool.h>
#include <stdio.h>

/**
 * These macros are here to help you create tests more easily and keep them
 * clear.
 *
 * The basic idea with unit-testing is create a test function for every real
 * function and inside the test function declare some variables and execute the
 * function under test.
 *
 * Use the ASSERT_TEST and ASSERT_TEST_WITH_FREE to verify correctness of
 * values.
 */


/**
 * Evaluates expr and continues if expr is true.
 * If expr is false, ends the test by returning false and prints a detailed
 * message about the failure.
 */
#define ASSERT_TEST(expr, goto_label)                                                         \
     do {                                                                          \
         if (!(expr)) {                                                            \
             printf("\nAssertion failed at %s:%d %s ", __FILE__, __LINE__, #expr); \
             result = false;                                                       \
             goto goto_label;                                                         \
         }                                                                         \
     } while (0)

/**
 * Macro used for running a test from the main function
 */
#define RUN_TEST(test, name)                  \
    do {                                 \
      printf("Running %s ... ", name);   \
        if (test()) {                    \
            printf("[OK]\n");            \
        } else {                         \
            printf("[Failed]\n");        \
        }                                \
    } while (0)


#endif //MAIN_C_TEST_UTILITIES_H
