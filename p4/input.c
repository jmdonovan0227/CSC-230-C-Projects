/**
 * @file input.c
 * @author Jake Donovan (jmpatte8)
 * This implementation file is responsible for reading
 * a single line of input from a given input stream and returning
 * as a string inside a block of dynamically allocated memory
*/


#include "input.h"

/**
 * This function is responsible for reading a single line input
 * from the passed FILE stream and returning a string inside a block
 * of dynamically allocated memory. This is useful for reading
 * commands from user and read menu item descriptions from a menu file
 * @param fp a pointer to a FILE input stream
 * @return a string inside a block of dynamically allocated memory
*/
char * readLine( FILE *fp )
{
    int capacity = INITIAL_CAPACITY;
    char * list = calloc(capacity, sizeof(char *));
    int ch = fgetc(fp);

    if(ch == EOF || ch == '\n'){
        if(ch == '\n'){
            ch = fgetc(fp);
        }

        else {
            free(list);
            return NULL;
        }
    }

    else {
        int len = 0;
        while((ch >= 'a' && ch <= 'z') || (ch == ' ') || (ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'Z')){
            if(len >= capacity){
                capacity = capacity * DOUBLE_CAPACITY;
                list = realloc(list, capacity * sizeof(char));
            }
            
            *(len + list) = (char)ch;
            len++;
            ch = fgetc(fp);
        }

        if(len > 0){
            return list;
        }
    }

    //free(list);
    return NULL;
}