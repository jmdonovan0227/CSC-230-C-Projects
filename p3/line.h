/**
 * @file line.h
 * @author Jake Donovan (jmpatte8)
 * This header file is responsible for including stdio.h
 * header file and all files that implement this header file
 * so that all required library functions can be utilized when processing
 * input lines
*/

#include <stdio.h>

/** Counts characters in a line of input 
 * @param fp a pointer to a file
 * @return the length of the line of characters
*/
extern int measureLine( FILE *fp );

/** Reads next input line of characters
 * @param fp a pointer to a file
 * @param str a char array where we store our input from file
*/
extern void readLine( FILE *fp, char str[] );
