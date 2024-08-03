/**
 * @file base32.c
 * @author Jake Donovan (jmpatte8)
 * this file is responsible for calculating values in base32 and returning as base10 values
 * it is also able to print the transferred values back to base32 at the end of a calculation via
 * printNumber()
*/

#include "base.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * This method is responsible for reading a number in base 32 and converting
 * to base10 so we can use the value within our polish.c file
 * @return our base32 value in base10
*/
long readNumber(){
    long value = 0;

    int ch = getchar();

    while(ch == ' '){
        ch = getchar();
    }

    int count = 0;

    if(ch == '-'){
        count++;
        ch = getchar();
    }

    while(ch >= '0' && ch <= 'V') {
        long d = 0;

        if(ch >= '0' && ch <= '9'){
            d = ch - '0';
        }

        else {
            d = ch - 'A' + LETTER;
        }

        // Check if we can multiply by 32
        checkMul(value, BASE_32);

        value *= BASE_32;

        checkAdd(value, d);

        value += d;

        ch = getchar();
    }

    if(ch > 'V'){
        exit(FAIL_INPUT);
    }

    ungetc(ch, stdin);

    if(count > 0){
        checkMul(value, -1);
        value *= -1;
    }

    return value;
}

/**
 * This function is responsible for printing a passed long value
 * in base32
 * @param val the value we want to print in base32
*/
void printNumber( long val ){
    int ch = 0;
    long check = val;

    if(check < 0){
        check *= -1;
    }

    if(val == 0){
        //
    }

    else {
        ch = 0;

        if(check % BASE_32 >= LETTER){
            ch = (check % BASE_32) - LETTER + 'A';
        }

        else {
            if(val < 0 && check < LETTER && check > 0){
                putchar('-');
                ch = (check % BASE_32) + '0';
            }

            else if(val < 0 && check > LETTER && check < BASE_32){
                putchar('-');
                ch = (check % BASE_32) - LETTER + 'A';
            }
            
            else {
                ch = (check % BASE_32) + '0';
            }
        }

        val = val / BASE_32;
        printNumber(val);
        putchar(ch);
    }
}