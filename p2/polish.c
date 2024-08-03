/**
 * @file polish.c
 * @author Jake Donovan (jmpatte8)
 * This file is responsible for parsing expressions in polish notation
 * while passing to other files to get numbers and to determine if any of the
 * operations performed when parsing overflows
*/

#include "base.h"
#include "bounds.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * Read a series of characters and parse them in polish notation
 * @return long the calculated result of our polish notation expression
*/
static long parseExpression()
{
    // ~ + 35 / 135 -47 / * 62 81 34 will be our reference expression when coding this method as it
    // appears to be the most complicated
    // Get first character input
    int ch = getchar();

    // get rid of ws
    while(ch == ' ' || ch == '\n'){
        ch = getchar();
    }

    if(ch == '+'){
        long expressionOne = parseExpression();
        long expressionTwo = parseExpression();
        checkAdd(expressionOne, expressionTwo);
        return expressionOne + expressionTwo;
    }

    else if(ch == '~'){
        // We have an operator, check for next character
        // get next two expressions, can be numbers or more complicated expressions
        long expressionOne = parseExpression();
        long expressionTwo = parseExpression();
        checkSub(expressionOne, expressionTwo);
        return expressionOne - expressionTwo;
    }

    else if(ch == '/'){
        long expressionOne = parseExpression();
        long expressionTwo = parseExpression();
        checkDiv(expressionOne, expressionTwo);
        return expressionOne / expressionTwo;
    }

    else if(ch == '*'){
        long expressionOne = parseExpression();
        long expressionTwo = parseExpression();
        checkMul(expressionOne, expressionTwo);
        return expressionOne * expressionTwo;
    }

    else {
        if((ch >= '0' && ch <= '9') || ch == '-' || (ch >= 'A' && ch <= 'V')){
            ungetc(ch, stdin);
            return readNumber();
        }

        else {
            exit(FAIL_INPUT);
        } 
    }

    // we don't actually ever want to get HERE
    return EXIT_SUCCESS;
}

/**
 * File starting point, removes whitespace at the beginning of the polish notation expression
 * and then loops the expression until all expressions have been read, this function will
 * also handle EOF and newlines as well
 * @return exit status
*/
int main()
{
    // Get the first character in an expression
    int ch = getchar();

    // Handle EOF, ignore whitespace at the beginning of the expression, and  ignore newline
    while(ch != EOF){
        // Once we hit a non-whitespace character (which is not a newline or ws) then call parseExpression()
        // use ungetc to put the first non-whitespace character back in stdin so we can read it in parseExpression()
        if(ch != '\n' && ch != ' '){
            ungetc(ch, stdin);
            long result = parseExpression();

            if(result == 0){
                putchar('0');
            }

            else {
                printNumber(result);
            }
        }

        // Move ch to the next char and go through loop again, this can be handy
        // as well when we have another expression on a newline
        if(ch != EOF){
            if(ch == '\n'){
                putchar(ch);
                ch = getchar();
            }
            else {
                ch = getchar();
            }
        }
    }

    // Return successfully
    return EXIT_SUCCESS;
}