/**
 * @file line.c
 * @author Jake Donovan (jmpatte8)
 * This file is responsible for measuring the number
 * of characters in a line of input and able to read a line
 * of input and store in a char array
 */

#include "line.h"

/**
 * This function counts and stores the number of characters
 * in the next line of input from a given file. The count includes
 * the newline at the end. If there is no more input to process,
 * this function will return 0. After reading a line of input, this
 * function should seek the input file back to the position it was before
 * this function was called so the caller should be able to read the next line
 * after measuring how long it is with this function.
 * @param fp a pointer to a file
 * @return the number of characters in the next input line
*/
int measureLine(FILE *fp)
{
  // create a variable to hold the total number of chars in our input line
  int length = 0;
  // create a variable to hold our chars
  int ch;
  // get starting position in file stream
  long start = ftell(fp);

  // get first input char from file
  ch = fgetc(fp);

  // count chars in input line
  while(ch != EOF && ch != '\n'){
    length++;
    ch = fgetc(fp);
  }

  if(ch == '\n'){
    length++;
  }

  // Check this to make sure it works
  // sets file stream to position before this function was called
  fseek(fp, start, SEEK_SET);

  // return length
  return length;
}

/**
 * This function is responsible for reading the next input line from
 * fp and storing the input in a char array called str. With the help of
 * measureLine(), the caller is responsible for making sure str is long enough
 * to contain the next line of input from the given file.
 * @param fp a pointer to a file
 * @param str a char array of characters also known as a string 
*/
void readLine(FILE *fp, char str[])
{
  // get the first character
  int ch = fgetc(fp);
  // keep track or our index in our str array
  int index = 0;

  // keep looping until we hit a newline or EOF
  while(ch != EOF && ch != '\n') {
    str[index] = ch;
    index++;
    ch = fgetc(fp);
  }

  // if we have EOF put a null terminator at the end of the line
  // this is helpful as some tests had garbage at the end of the str array
  // without putting this null terminator here
  if(ch == EOF){
    str[index] = '\0';
    index++;
  }

  // if we have a newline or null terminator meaning we are at the end of the current line
  else if(ch == '\n' || ch == '\0'){
    // add the character to our str array
    str[index] = ch;
    index++;
    ch = fgetc(fp);

    // if we don't have a null terminator to end our line then add one
    // this is also to counteract our issue with garbage at the end of our str arrays
    // when reading some lines of input
    if(ch != '\0'){
      str[index] = '\0';
      index++;
      ungetc(ch, fp);
    }

    // otherwise just put our null terminator at the last index in our str array
    else {
      str[index] = ch;
      index++;
    }
  }
}