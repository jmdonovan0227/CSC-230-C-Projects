/**
 * @file expand.h
 * @author Jake Donovan (jmpatte8)
 * This header file is responsible for including stdbool.h
 * library header file so we can utilize the bool name in our files
 * that we include this header file
*/

#include <stdbool.h>

/**
 * Checks if a given character code evaluates to a word character
 * @param ch the passed character code
 * @return true if character code is a valid word character or false otherwise
*/
extern bool wordChar( int ch );

/**
 * Computes upper bound on all words to determine how long the src
 * string will be after replacing all target words
 * @param src a pointer to the beginning of a string
 * @param maxRep the maximum length of replacement strings
 * @return the length of string after replacing words
*/
extern int expansionBound( char *src, int maxRep);

/**
 * This function creates a copy of source string in dest with all
 * replacement words included
 * @param src the source string pointer
 * @param dest the destination string pointer
 * @param tList the target word array pointer
 * @param rList the replacement word array pointer
 * @param pairs the number of target words + replacement words
 * 
*/
extern void expand( char *src, char *dest, char *tList[], char *rList[], int pairs);