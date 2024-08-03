/**
 * @file input.h
 * @author Jake Donovan (jmpatte8)
 * This header file is responsible for including the required behavior
 * for our input.c implementing class to read input from a file steram
 * and return as a string inside a block of dynamically allocated memory
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** A preprocessor constant that holds the max size for our cmd array which is what we use to hold our command values such as list */
#define COMMAND_MAX_SIZE 8
/** A preprocessor constant that holds the max size for our name array which holds our names for MenuItems */
#define NAME_MAX_SIZE 25
/** A preprocessor constant that holds the max size for our category array which holds the categories of our MenuItems */
#define CATEGORY_MAX_SIZE 16
/** A preprocessor constant for the value we divide and mod by when printing our cost dollars and cents */
#define CONVERT_COST_INT 100
/** A preprocessor constant for the mininmum command line arguments for our program to process input, which is at least one file */
#define MIN_ARGS 2
/** A preprocessor constant for the initial capacity of our Menu and Order Lists */
#define INITIAL_CAPACITY 5
/** A preprocessor constant to keep track of when our cents value is less than ten which we would need to pad with a 0 in listOrderItems and menuOrderItems */
#define SINGLE_DIGIT_CENTS_CONV 10
/** A preprocessor constant for the max size for our id array which holds the categories of our MenuItems */
#define ID_ARRAY_MAX_SIZE 6
/** A preprocessor constant for the number of digits for a valid id */
#define MAX_ID_DIGITS 4
/** A preprocessor constant for the max size of our array that holds a line from readLine() */
#define MAX_LINE_LENGTH 50
/** A preprocessor constant for the invalid index we want to check in our name array */
#define NAME_ARRAY_INVALID_IDX 21
/** A preprocessor constant for doubling the capacity of our order lists and menu lists */
#define DOUBLE_CAPACITY 2

/**
 * This functions reads a single line of input
 * from a given input stream and returns as a string
 * inside a block of a dynamically allocated memory
 * this can read commands from the user and to read menu
 * item descriptions from a menu item file
 * @param fp the passed file input stream
 * @return a string inside a block of dynamically allocated memory
*/
char * readLine( FILE *fp );