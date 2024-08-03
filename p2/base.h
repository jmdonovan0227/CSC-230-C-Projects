/**
 * @file bounds.h
 * @author Jake Donovan (jmpatte8)
 * This file is responsible for declaring our function prototypes
 * so that they can be used in other files
*/

#include "bounds.h"

/** Exit status indicating that the program was given invalid input. */
#define FAIL_INPUT 102

/** We need this to determine our values in base32 */
#define BASE_32 32

/** We have a value greater than 9, which in base 32, A starts at 10 */
#define LETTER 10

/**
  Read a number from standard input in the current base, terminating unsuccessfully
  if it's not in the right format or if there's an overflow.
  @return the number read in.
  */ 
long readNumber();

/**
  Print the given value to standard output in the current base.
  @param val the value to print.
  */ 
void printNumber( long val );

