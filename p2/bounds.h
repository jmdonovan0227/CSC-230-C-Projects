/**
 * @file bounds.h
 * @author Jake Donovan
 * This file is responsible for declaring our bounds file function prototypes so that we can use
 * them in other files
*/

/** Our constant for an exit status of 101 which indicates we tried to divide by zero */
#define DIVIDE_BY_ZERO 101
/** Our constant for an exit status of 100 which indicates we had overflow when performing a mathematical operation */
#define OVERFLOW 100

/**
 * Check our subtraction operations to determine if there is overflow
 * when subtracting the two passed values
 * @param a the first long that will be subtracted
 * @param b the second long that will be subtracted
*/
void checkSub(long a, long b);

/**
 * Check our addition operations to determine if there was overflow
 * @param a the first long that will be added
 * @param b the second long that will be added
*/
void checkAdd(long a, long b);

/**
 * Check our multiplication operations to determine if there was overflow
 * @param a the first long that will be multiplied
 * @param b the second long that will be multipplied
*/
void checkMul(long a, long b);

/**
 * Check our division operations to determine if there was overflow
 * or divide by zero
 * @param a the first long that will be divided
 * @param b the second long that will be divided
*/
void checkDiv(long a, long b);