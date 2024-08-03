/**
 * @file bounds.c
 * @author Jake Donovan (jmpatte8)
 * This file is responsible for checking mathematical operations to determine if they are valid
 * aka to make sure they don't overflow
*/

#include <limits.h>
#include "bounds.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * This function is responsible for checking our addition operations to determine if they overflow
 * @param a the first long we will add
 * @param b the second long we will add
*/
void checkAdd( long a, long b)
{
    long result = a + b;

    if(a > 0 && b > 0 && result < 0){
        exit(OVERFLOW);
    }

    if(a < 0 && b < 0 && result > 0){
        exit(OVERFLOW);
    }

    if(result > LONG_MAX){
        exit(OVERFLOW);
    }

    if(result < LONG_MIN){
        exit(OVERFLOW);
    }
}

/**
 * This function is responsible for checking our subtraction operations
 * to make sure they do not overflow
 * @param a the first long that will be subtracted
 * @param b the second long that will be subtracted
*/
void checkSub( long a, long b)
{
    long result = a - b;

    if(a > 0 && b > 0 && result < 0 && result < LONG_MIN){
        exit(OVERFLOW);
    }

    if(a < 0 && b < 0 && result > LONG_MAX){
        exit(OVERFLOW);
    }

    if(a > 0 && b < 0 && result > LONG_MAX){
        exit(OVERFLOW);
    }

    if(a < 0 && b > 0 && result < LONG_MIN){
        exit(OVERFLOW);
    }

    if(result < LONG_MIN){
        exit(OVERFLOW);
    }

    if(result > LONG_MAX){
        exit(OVERFLOW);
    }
}

/**
 * This function is responsible for checking our multiplication operations to make sure they do not overflow
 * @param a the first long we will multiply
 * @param b the second long we will multiply
*/
void checkMul( long a, long b) 
{
    long x = 0;
    x = LONG_MAX / b;

    if(a < 0 && b < 0){
        b = b * -1;
        x = LONG_MIN / b;

        if(a <= x){
            exit(OVERFLOW);
        }
    }

    long result = a * b;

    if(a > x && a > 0 && b > 0){
        exit(OVERFLOW);
    }

    else if(result > LONG_MAX) {
        exit(OVERFLOW);
    }

    else if(result < LONG_MIN){
        exit(OVERFLOW);
    }
}

/**
 * This function is responsible for checking our division operations to make sure they do not overflow
 * and that we do not try to divide by zero
 * @param a the first long we will divide by
 * @param b the second long we will divide by
*/
void checkDiv( long a, long b )
{
    if(b == 0 || b == 0){
        exit(DIVIDE_BY_ZERO);
    }

    if((a / b) < LONG_MIN){
        exit(OVERFLOW);
    }

    if((a / b) > LONG_MAX){
        exit(OVERFLOW);
    }
}