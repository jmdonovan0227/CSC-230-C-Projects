/**
   @file textbox.c
   @author Jake Donovan (jmpatte8)
   This program is responsible for creating formatted textboxes around text
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/** Width of each line of text in the box. */
#define LINE_WIDTH 60
/** Our max line length in characters with our border '*' on each end */
#define TOTAL_LENGTH 62
/** Symbol used to draw the border around the box. */
#define BORDER '*'
/** Our space character for padding lines */
#define SPACE ' '
/** Our newline character which indicates we have hit a newline */
#define NEWLINE '\n'

/**
   This method is about to check and modify lines so that they maintain a line
   length of 60 characters so that the textbox around each line of text is uniform
   @return true if we have more input or false if we have no more input to process
*/
bool paddedLine()
{
     // keeps track of number of characters processed
     int chars = 0;
     // gets first char
     int ch = getchar();
     // keeps track of whether we have more input to process or not
     bool check = true;
 
     // if we have no more input to process, return false
     if ( ch == - 1 ) {
         return false;
     }
 
     // otherwise...
     else {
         // enclose text in border (this places a '*' on left side)
         putchar( BORDER );
      
         // keep going until we either hit the end of file, we hit a new line, or have
         // a line of length 60 characters
         while ( ch != EOF && ch != NEWLINE && chars < LINE_WIDTH ) {
               putchar( ch );
               ch = getchar();
               chars++;
         }
     
         // this checks for the end of file 
         if ( ch == EOF ) {
             // does line have 60 characters?
             if ( chars < LINE_WIDTH ) {
                 // if not pad with spaces until we do
                 while ( chars < LINE_WIDTH ) {
                       putchar( SPACE );
                       chars++;
                 }
         
                 // we have hit EOF so we have no more information to process
                 check = false;
             }
       
             // if we have 60 characters just set check to false
             else {
                 check = false;
             }
         }
     
         // this checks for the end of a line
         else if ( ch == NEWLINE ) {
                  if ( chars < LINE_WIDTH ) {
                       while ( chars < LINE_WIDTH ) {
                             putchar( SPACE );
                             chars++;
                       }
                       // We have more input to process
                       check = true;
             
                  }
            
                  else {
                      check = true;
                  }  
         }
     
         // We have a line longer than 60 characters if we get here
         else {
     
             if ( chars < LINE_WIDTH ) {
                 while ( chars < LINE_WIDTH ) {
                       putchar( SPACE );
                 }
             }
          
             // discard input until we hit EOF or a newline
             while ( ch != NEWLINE && ch != EOF ) {
                   ch = getchar();
             }
         
             if ( ch == NEWLINE ) {
                 check = true;
             }
         
             if ( ch == EOF ) {
                 check = false;
             }
         }
     
         putchar( BORDER );
         putchar( NEWLINE );
     }
     
     // Return true or false based on whether we have more input to read or not
     return check;
}

/**
   Constructs a line of a characters with a passed character
   ch and a specified length in param count
   @param ch the specified character we will use to print a line of chars
   @param count the length of the line of chars we will create here
*/
void lineOfChars( char ch, int count )
{
     int x = 0;
     
     while ( x < count ){
           putchar( ch );
           x++;
     }
     
     putchar( NEWLINE );
}

/**
   Program starting point, passes to other functions in this C file to create
   our textbox around our text
   @return sucessfull exit status
*/
int main()
{
    lineOfChars( BORDER, TOTAL_LENGTH );
 
    bool lines = paddedLine();
 
    while ( lines != false ) {
          lines = paddedLine();
    }
 
    lineOfChars( BORDER, TOTAL_LENGTH );
 
    return EXIT_SUCCESS;
}
