/**
 * @file expand.c
 * @author Jake Donovan (jmpatte8)
 * This file is responsible for processing characters
 * in a char array to determine if they are valid words and determining
 * how long the char array will be after replacing the specified
 * characters or words and then returning a new char array that contains
 * the replacement words
*/

#include "expand.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * This function is responsible for checking input characters
 * to determine if they are valid word characters if they are
 * valid then this function will return true otherwise this function
 * will return false
 * @param ch a passed character code for a character
 * @return true if the the input character code is a valid word character
 * or false otherwise
*/
bool wordChar( int ch )
{
  // check for a valid word char if true return here
  if((ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || ch == '_'){
    return true;
  }

  // we don't have one if we get here so return false
  else {
    return false;
  }
}

/**
 * This function is responsible for determining the upper bound
 * on how long the given string src will be after all target words
 * are replaced by the replacement strings. For the second param,
 * you should pass this function the maximum length of the replacement
 * strings.
 * @param src the source string
 * @param maxRep the max length of replacement strings
*/
int expansionBound( char *src, int maxRep )
{
  // our new length which we will return
  int newLength = 0;
  // each processed word's length
  int wordLength = 0;
  // the first character held by our src pointer
  char ch = *(src);

  // check all characters by utilizing our src pointer
  while(ch != '\0' && ch != '\n'){
    // if we have an invalid character check to see if we have a word we can compare against
    // to see if it shorter than our max replacement (length wise) word
    if(!wordChar((int) ch)){
      // we have a word and it is shorter than our maxRep word
      if(wordLength > 0 && wordLength <= maxRep){
        // add maxRep to our newLength
        newLength += maxRep;
        // add 1 for space
        newLength++;
        // reset word counter
        wordLength = 0;
        // move file pointer
        ch = *(++src);
      }

      // we have a word but it is longer than maxRep
      else if(wordLength > 0 && wordLength > maxRep){
        newLength += wordLength;
        // add 1 for space
        newLength++;
        // reset word counter
        wordLength = 0;
        // move file pointer
        ch = *(++src);
      }

      // else if we have not processed a word just increment for each non-word character
      else {
        // increment new length by 1 for space
        newLength++;
        // move file pointer
        ch = *(++src);
      }
    }

    // otherwise if we have a valid word character increment the word length counter
    else {
      // increment word length
      wordLength++;
      // move to next character
      ch = *(++src);
    }
  }

  // add one to newLength for newline char if ch was a newline when our loop broke
  // othwerise we hit a null terminator indicating we are done reading the current string
  // this is mainly to get the last word
  if(ch == '\n' || ch == '\0'){
    // if we had a new line when our loop broke

    // our word is valid and it is longer than maxRep
    if(wordLength > 0 && wordLength > maxRep){
      newLength += wordLength;
      wordLength = 0;
    }

    // our word is valid but it is shorter than maxRep
    else if(wordLength > 0 && wordLength < maxRep){
      newLength += maxRep;
      wordLength = 0;
    }

    else if(ch == '\n'){
      // add one for newline
      newLength++;
    }
  }

  // return the updated length
  return newLength;
}

/**
 * This function creates a copy of the src string, src, in dest
 * but with every occurence of a target string replaced by its replacement
 * tList array is the list of target strings given on the command line and
 * rList is the list of corresponding replacement strings. For ex, if tList[0]
 * = "less" and rList[0] = "more" then it means the user asked for the occurences
 * of less to be replaced with more, the pairs param gives the length of tList an rList.
 * With the help of expansionBound() function, the caller is responsible
 * for making sure dest is long enough to contain the resulting string after all replacements
 * have been performed.
 * @param src the source string
 * @param dest the destination string
 * @param tList the list of target strings given on the command line
 * @param rList the list of replacement strings
 * @param pairs the length of tList and rList
*/
void expand( char *src, char *dest, char *tList[], char *rList[], int pairs )
{
  // create a char array for comparisons, make the char array the sizeof(src) so we know we can't go out of bounds
  // when examining src
  char check[strlen(src) + 1];

  // keep track of the current index we are adding to in check
  int checkIndex = 0;

  // keep track of the current index we are in our dest array when modifiying our values with our pointer
  int destIndex = 0;

  // keep track of the wordLength of the current word we are adding to check to compare
  int wordLength = 0;

  // keep track of our matches when comparing for target words
  bool match = false;

  // keep track of the matched index of a target word
  int matchedIndex = 0;

  // declare a char variable to look at each character in source
  char ch = *(src);

  // our current point in our char array str which we access it's contents through our src pointer
  int iterator = 0;
  // the total length of our current char array or String
  int len = strlen(src) + 1;

  // read all the way up to end of our current char array or line
  while(iterator < len){
    // we have an invalid word character
    if(!wordChar((int) ch)){
      // if we have a word we can compare
      if(wordLength > 0){
        // first compare our check array contents against the target words in tList to see if we have a match
        // use pairs which is the number of target and replacements words we have as our loop end condition
        // compare up to our wordLength to see if we have matching target value

        // we have some targets to compare against
        if(pairs > 0){
          for(int i = 0; i < pairs; i++){
          // since we are overwriting our array contents and comparing against our target values
          // we need to use strncmp so we can only compare characters up to our current word's length
          // we also need a check to make sure when we match, that we have matched against all characters
          // in target string. For ex: if we have 2 and our target value is 28 it will match using strncmp
          // against the 2 because our wordLength will only be 1 so we need to check that we have actually matched
          // against our target value correctly so we need to check that our wordLength is the same as the target value.
          if(strncmp(check, *(i + tList), wordLength) == 0 && wordLength == strlen(*(i + tList))) {
            match = true;
            break;
          }

          // check the next target
          else {
            // increment matchedIndex so we can use this later for accessing our rList val
            matchedIndex++;
          }
        }
        }

        // now check if we have a matching target word
        if(match){
          // the char array of characters
          char * line = *(matchedIndex + rList);
          // it's length
          int len = strlen(*(rList + matchedIndex));
          // get each char in the rList that was matched to tList
          char ch;

          // add all characters from replacement string to dest
          for(int i = 0; i < len; i++){
            // get each individual char
            ch = *(line + i);
            // add at the correct destIndex
            *(dest + destIndex) = ch;
            // increment destIndex so we can add the next char correctly
            destIndex++;
          }
        }

        // if we don't just add the current word we read from src up to wordLength
        else {
          for(int i = 0; i < wordLength; i++){
            *(dest + destIndex) = check[i];
            destIndex++;
          }
        }

        // reset our matchedIndex!
        matchedIndex = 0;
        // set wordLength back to 0 for our next word
        wordLength = 0;
        // set match back to false
        match = false;
        // put the current space char in dest
        *(dest + destIndex) = ch;
        // increment destIndex for next char to add
        destIndex++;
        // set checkIndex back to zero so we can overwrite and compare check's contents to compare against with new words
        checkIndex = 0;
        // get the next char
        ch = *(++src);
        iterator++;
      }

      // if we don't just add the space at the current index in dest using our pointer and destIndex val
      else {
        // assign the current dest index with a space
        *(dest + destIndex) = ch;
        // increment our destIndex
        destIndex++;
        // get the next char
        ch = *(++src);
        iterator++;
      }
    }

    else {
      // assign the current word char to our check char array
      check[checkIndex] = ch;
      // increase checkIndex for next add
      checkIndex++;
      // increase wordLength to keep track of our word length
      wordLength++;
      // move to the next char with our src pointer
      ch = *(++src);
      // increment our iterator variable to move to the next character
      iterator++;
    }
  }
}