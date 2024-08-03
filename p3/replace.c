/**
 * @file replace.c
 * @author Jake Donovan (jmpatte8)
 * This file is responsible for calling our other file's functions
 * in order to successfully replace all target words with replacement strings
 * and return the output.
*/

#include "line.h"
#include "expand.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/** Number of required arguments at the end of the command line. */
#define REQUIRED_ARGS 2
/** Number that represents the input file, output file, and command which we want to subtract to determine our target and replacement commands*/
#define TARGETS_AND_REPLACEMENTS 3
/**
 * Program starting point, calls all necessary files with their
 * included functions to process strings and replace target strings
 * with replacement strings and return those changes
 * @return program exit status
*/
int main(int argc, char *argv[])
{
  if(argc <= REQUIRED_ARGS){
    printf("Not enough arguments\n");
  }

  else {
    FILE *input = fopen(argv[argc - REQUIRED_ARGS], "r");

    if(input == NULL){
      FILE *error = fopen("stderr.txt", "w");
      fprintf(error, "Can't open file: %s\n", argv[argc - REQUIRED_ARGS]);
      exit(1);
    }

    FILE *output = fopen(argv[argc - 1], "w");

    if(output == NULL){
      FILE *error = fopen("stderr.txt", "w");
      fprintf(error, "Can't open file: %s\n", argv[argc - 1]);
      exit(1);
    }

    // determine the size needed for each replacement and target word arrays
    // factor out the first argument and last two which are the input and output file
    // then divide by 2 as both rWord and tWord should have an equal size
    int size = (argc - TARGETS_AND_REPLACEMENTS) / REQUIRED_ARGS;
    // construct our target array pointer
    char *tWord[size];
    // construct our replacement array pointer
    char *rWord[size];
    // initialize a variable to keep track of the longest replacement word
    int maxRep = 0;
    // intialize a variable to keep track of the number of pairs we have between
    // our tWord array and rWord array
    int pairs = 0;

    // Make sure that argc is greater than 3 meaning we have a valid number
    // of arguments and we also possibly have an unknown number of target words and
    // replacements, they should be in pairs so check that argc - 1 % 2 == 0 because
    // our first argument does not need to be examined
    if(argc > TARGETS_AND_REPLACEMENTS && (argc - 1) % REQUIRED_ARGS == 0) {
      // current index for rWord and tWord arrays
      int idx = 0;

      // start at 1 to skip past the first argument
      // check to see if we can get the next argument
      // (i + 1) in argv without reaching our input file name
      // which is the second to last argument for a valid number of arguments
      for(int i = 1; (i + 1) < argc - REQUIRED_ARGS; i = i + REQUIRED_ARGS){
         // get target word and add to tWord
         tWord[idx] = argv[i];

         // get replacement word's length that we will add
         // to rWord and keep checking if the length is greater
         // than the current maxRep
         if(strlen(argv[i + 1]) > maxRep){
          maxRep = strlen(argv[i + 1]);
         }

         // get replacement word and put in rWord
         rWord[idx] = argv[i + 1];
         // update our array index
         idx++;
         // update the number of pairs we have
         pairs++;
      }

      bool duplicate = false;
      bool invalidWord = false;
      int invalid;

      // Check for invalid replacement words
      for(int i = 0; i < pairs; i++){
        int len = strlen(*(i + rWord));
        char * line = *(i + rWord);
        for(int j = 0; j < len; j++){
          char ch = *(j + line);
          if(wordChar((int)ch) == false){
            invalidWord = true;
            invalid = i;
            break;
          }
        }
      }

      // if we found one send an error to our file
      if(invalidWord == true){
        char * line = *(invalid + rWord);
        FILE *error = fopen("stderr.txt", "w");
        fprintf(error, "Invalid word: %s\n", line);
        exit(1);
      }

      // otherwise we also need to check for duplicate target values
      else {
        int duplicateIdx;
        // make sure we have more than one pair
        if(pairs > 1){
          // check all target values
          for(int i = 0; i < pairs; i++){
            for(int j = 0; j < pairs; j++)
            if(strcmp(*(i + tWord), *(j + tWord)) == 0 && i != j){
              duplicateIdx = j;
              duplicate = true;
              break;
            }
          }
        }

        // if we have a duplicate
        if(duplicate == true){
          char * line = *(duplicateIdx + tWord);
          FILE *error = fopen("stderr.txt", "w");
          fprintf(error, "Duplicate target: %s\n", line);
          exit(1);
        }
      }
    }

    else {
      // do something here
    }

    // Keep calling our functions until measureLine() returns 0
    // indicating we have no more input to process
    // first call
    int process = 0;

    // Keep checking if we have a line with a length greater than 0
    // if measureLine() returns 0 we are done processing
    while((process = measureLine(input)) != 0){
      // create a new array with size determined by measureLine() add 1 for null terminator
      char str[process + 1];
      // read the input on the current line
      readLine(input, str);
      // create a pointer to beginning of str array which has been modified in readLine()
      char *src = NULL;
      // assign the pointer to the front
      src = str;
      //char ch = *(str); this returns a pointer to first char
      
      // determine the size of the dest array which will hold current line with replacement words using expansionBound()
      int destSize = expansionBound(src, maxRep);
      // create the array, add 1 for null terminator
      char dest[destSize + 1];
      // create a pointer to the front of the array
      char *destP = NULL;
      // assign the pointer to the front
      destP = dest;
      // call expand to modify the contents of dest array with pointer destP
      expand(src, destP, tWord, rWord, pairs);
      // print contents to output file
      fprintf(output, "%s", dest);
      // check to see if we have more information to process
      process = measureLine(input);
    }

  // close input file
  fclose(input);
  // close output file
  fclose(output);
  }
  
  // return successfully
  return EXIT_SUCCESS;
}