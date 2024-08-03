/**
 * @file hash.c
 * @author Jake Donovan (jmpatte8)
 * This is the main component. It contains the main function and uses the other components
 * to read the input file and to perform the RIPEMD computation.
*/

#include "byteBuffer.h"
#include "ripeMD.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * This function calls byteBuffer.c functions and ripeMD.c functions to
 * read an input file and perform RipeMD computation
 * @param argc the number of command-line arguments
 * @param argv an array of pointers to command line arguments as strings
 * @return program exit status
*/
int main( int argc, char *argv[] )
{
  // the hash program expects one command line argument
  // if we don't have one command line, print to stderr and
  // terminate the program.
  if( argc != MIN_ARGS ){
    fprintf( stderr, "usage: hash <input-file>\n" );
    exit( EXIT_FAILURE );
  }

  // pass filename to readFile() in byteBuffer.c to construct a new ByteBuffer
  ByteBuffer *buffer = readFile( argv[1] );

  // if readFile() returns null, that means the file could not be opened so we need to throw an error using perror() and exit unsuccessfully
  if( buffer == NULL ){
     perror( argv[1] );
     exit( EXIT_FAILURE );
  }

  // pad our buffer with spaces by calling padBuffer() until we have a multiple of 
  padBuffer( buffer );

  // construct a new HashState
  HashState *state = ( HashState * )malloc( sizeof( HashState ) );
  // initialize the newly constructed HashState fields
  initState( state );

  // file length variable so we can loop through each 64 byte chunks in our buffer
  int allByte = 0;

  while( allByte < buffer->len ){
      // use memcpy to get the first 64 bytes from our buffer
      // construct a byte array
      byte cpy[ BLOCK_BYTES ] = {};
      // copy 64 bytes use allByte to shift to the correct spot in our buffer byte array
      // so we can pass the correct 64-byte chunk of our buffer's byte array (this is in the case)
      // that our buffer is larger than 64 bytes
      memcpy( cpy, buffer->data + allByte, BLOCK_BYTES * sizeof( byte ) );
      // pass our constructed and initialized state and our 64 byte array to hashBlock()
      hashBlock( state, cpy );
      // increment allByte by chunks of 64 as we know our buffer has a length that is a multiple of 64
      allByte += BLOCK_BYTES;
  }

  // print final state by call printHash()
  printHash( state );

  // free our buffer memory
  freeBuffer( buffer );

  // free initial state
  free( state );

  // return successfully
  return EXIT_SUCCESS;
}
