/**
 * @file base10.c
 * @author Jake Donovan (jmpatte8)
 * This file is responsible for reading and printing values in base10
*/

#include "base.h"
#include "bounds.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * This function is responsible for reading and returning
 * values in base10
 * @return val our value in base10
*/
long readNumber()
{
     long val = 0;
     
     int matches = scanf("%ld", &val);
     
     if(matches == 1) {
        return val;
     }
     
     else {
          return -1;
     }
}

/**
 * This function is responsible for printing our base10 values
 * @param val our passed value that we will print in base10
*/
void printNumber( long val ) 
{
     printf("%ld", val);
}