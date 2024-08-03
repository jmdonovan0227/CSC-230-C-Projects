/**
 * @file ripeMD.c
 * @author Jake Donovan (jmpatte8)
 * This file is responsible for holding a struct that stores
 * five element states used during hash calculation and functions to compute
 * various parts of the RIPEMD algorithm.
*/

#include "ripeMD.h"
#include "byteBuffer.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * Given the address of a HashState instance, this funciton intializes its fields, filling them in
 * with the five constant values given in the RIPEMD algorithm description
 * @param state the current state
*/
void initState( HashState *state )
{
  state->A = 0x67452301;
  state->B = 0xEFCDAB89;
  state->C = 0x98BADCFE;
  state->D = 0x10325476;
  state->E = 0xC3D2E1F0;
}

/**
 * This function pads the given buffer, bringing its length up to a multiple of 64 bytes,
 * adding byte values as described in he RIPEMD algorithm
 * @param buffer the passed buffer
*/
void padBuffer( ByteBuffer *buffer )
{
   // save our original total
  int original = buffer->len;

  // add Ox80 byte
  addByte(buffer, 0x80);

  // calculate the total length we need to get a multiple of 64
  int total = buffer->len;

  // increment total until we have a multiple of 64
  while( (total % BLOCK_BYTES ) != 0 ){
    total++;
  }

  // set total to be 8 less than our target value because we need to leave eight bytes at the end
  total = total - BBITS;

  while( buffer->len < total ){
    addByte( buffer, 0x00 );
  }

  // calculate our value to add at the end
  // this is an unsigned char
  unsigned long x = original * BBITS;

  byte mask = 0xFF;

  for(int i = 0; i < BLOCK_BYTES; i += BBITS) {
    if( i > 0 ) {
      addByte( buffer, ( ( x >> i ) & mask ) );
    }
    
    else {
      addByte( buffer, ( x & mask ) );
    }
  }
}

/**
 * This function prints out the final hash value stored in the given state. It prints it as a 160 bit
 * number in hexadecimal.
 * @param state the current state
*/
void printHash( HashState *state )
{
  // use lecture 19 slides swapBytes4 to complete this method
  // a
  char * aSwap = (char *)(&state->A);
  char aTemp = aSwap[ SW_INDEX_0 ];
  aSwap[ SW_INDEX_0 ] = aSwap[ SW_INDEX_3 ];
  aSwap[ SW_INDEX_3 ] = aTemp;
  aTemp = aSwap[ SW_INDEX_1 ];
  aSwap[ SW_INDEX_1 ] = aSwap[ SW_INDEX_2 ];
  aSwap[ SW_INDEX_2 ] = aTemp;

  // b
  char * bSwap = (char *)(&state->B);
  char bTemp = bSwap[ SW_INDEX_0 ];
  bSwap[ SW_INDEX_0 ] = bSwap[ SW_INDEX_3 ];
  bSwap[ SW_INDEX_3 ] = bTemp;
  bTemp = bSwap[ SW_INDEX_1 ];
  bSwap[ SW_INDEX_1 ] = bSwap[ SW_INDEX_2 ];
  bSwap[ SW_INDEX_2 ] = bTemp;

  // c
  char * cSwap = (char *)(&state->C);
  char cTemp = cSwap[ SW_INDEX_0 ];
  cSwap[ SW_INDEX_0 ] = cSwap[ SW_INDEX_3 ];
  cSwap[ SW_INDEX_3 ] = cTemp;
  cTemp = cSwap[ SW_INDEX_1 ];
  cSwap[ SW_INDEX_1 ] = cSwap[ SW_INDEX_2 ];
  cSwap[ SW_INDEX_2 ] = cTemp;

  // d
  char * dSwap = (char *)(&state->D);
  char dTemp = dSwap[ SW_INDEX_0 ];
  dSwap[ SW_INDEX_0 ] = dSwap[ SW_INDEX_3 ];
  dSwap[ SW_INDEX_3 ] = dTemp;
  dTemp = dSwap[ SW_INDEX_1 ];
  dSwap[ SW_INDEX_1 ] = dSwap[ SW_INDEX_2 ];
  dSwap[ SW_INDEX_2 ] = dTemp;

  //
  char * eSwap = (char *)(&state->E);
  char eTemp = eSwap[ SW_INDEX_0 ];
  eSwap[ SW_INDEX_0 ] = eSwap[ SW_INDEX_3 ];
  eSwap[ SW_INDEX_3 ] = eTemp;
  eTemp = eSwap[ SW_INDEX_1 ];
  eSwap[ SW_INDEX_1 ] = eSwap[ SW_INDEX_2 ];
  eSwap[ SW_INDEX_2 ] = eTemp;

  // print all values
  fprintf(stdout, "%08x%08x%08x%08x%08x\n", state->A, state->B, state->C, state->D, state->E );
}

/**
 * Version 0 of bitwise function f for combining longwords a, b, and c
 * @param a the first longword
 * @param b the second longword
 * @param c the third longword
 * @return the combined longword
*/
static longword bitwiseF0( longword a, longword b, longword c )
{
  return a ^ b ^ c;
}

/**
 * Version 1 of bitwise function f for combining longwords a, b, and c
 * @param a the first longword
 * @param b the second longword
 * @param c the third longword
 * @return the combined longword
*/
static longword bitwiseF1( longword a, longword b, longword c )
{
  return ( a & b ) | ( ~a & c );
}

/**
 * Version 2 of bitwise function f for combining longwords a, b, and c
 * @param a the first longword
 * @param b the second longword
 * @param c the third longword
 * @return the combined longword
*/
static longword bitwiseF2( longword a, longword b, longword c )
{
  return ( a | ~b ) ^ c;
}

/**
 * Version 3 of bitwise function f for combinining longwords a, b, and c
 * @param a the first longword
 * @param b the second longword
 * @param c the third longword
 * @return the combined longword
*/
static longword bitwiseF3( longword a, longword b, longword c )
{
  return ( a & c ) | ( b & ~c);
}

/**
 * Version 4 of bitwise function f for combining longwords a, b, and c
 * @param a the first longword
 * @param b the second longword
 * @param c the third longword
 * @return the combined longword
*/
static longword bitwiseF4( longword a, longword b, longword c )
{
  return a ^ ( b | ~c );
}

/**
 * This function implements the rotate left operation from RIPEMD algorithm, 
 * shifting the given value left by s bits, with wraparound. It returns the resulting
 * value.
 * @param value the longword we want to rotate left
 * @param s the amount we want to shift our longword left by
 * @return the modified longword
*/
static longword rotateLeft( longword value, int s )
{
  longword x = value;
  longword mask = 0xFFFF;
  longword lostBits = x >> (LW_BITS - s) & mask;
  x = x << s;
  x |= lostBits;
  return x;
}

/**
 * This function implements an iteration of the RIPEMD algorithm. The first parameter is the input state,
 * and it should be set to the resulting state at the end of iteration. The datum and shift parameters are the element
 * of the data and shift arrays for the iteration (the particular values for this iteration have already been chosen by the caller)
 * the iteration function doesn't need to look them up in the data and shift arrays. The noise and f values are the noise
 * and bitwise function to use for this iteration.
 * @param state the current state
 * @param datum an element from the data array for iteration
 * @param shift an element from the shift array for iteration
 * @param noise the noise used for this iteration
 * @param f the bitwise function to use for this iteration
*/
static void hashIteration( HashState *state, longword datum, int shift, longword noise, BitwiseFunction f )
{
  // calculate value for STATE_B
  longword bVal = rotateLeft( state->A + f( state->B, state->C, state->D ) + datum + noise, shift) + state->E;
  // c val
  longword cVal = state->B;
  // d val
  longword dVal = rotateLeft( state->C, C_ROTATION );
  // e val
  longword eVal = state->D;
  // a val
  longword aVal = state->E;

  // calculate A
  state->A = aVal;

  // calculate C
  state->C = cVal;
  
  // calculate D
  state->D = dVal;

  // calculate E
  state->E = eVal;

  // calculate B
  state->B = bVal;
}

/**
 * This function implements a round of the RIPEMD algorithm. It calls hashIteration() to actually perform each of the 16 iterations in the round.
 * The first parameter is the input state, and it should be set to the resulting state at the end of the round. The data array is a 64-byte block, represented
 * as 16 longwords. The perm and shift arrays give the sequence of data elements and shift values to use in each iteration in the round. These let the funciton choose the right
 * element of data and right shift value to pass to each call of hashIteration(). The noise and f values are the noise and bitwise function to use for every iteration in the round.
 * @param state the input state
 * @param data a 64-byte block represented as 16 longwords
 * @param perm an array with a sequence of data elements to use in each iteration in the round
 * @param shift an array with a sequence of shift values to use in each iteration in the round
 * @param noise the noise function to use for every iteration in the round
 * @param f the bitwise function to use for every iteration in the round
*/
static void hashRound( HashState *state, longword data[ BLOCK_LONGWORDS ], int perm[ RIPE_ITERATIONS ], int shift[ RIPE_ITERATIONS ], longword noise, BitwiseFunction f )
{
  for(int i = 0; i < RIPE_ITERATIONS; i++) {
    hashIteration( state, data[ perm[ i ] ], shift[ i ], noise, f );
  }
}

/**
 * This function processes the given block of 64-bytes. The given state is the input state for processing the block, and it's used as the output state for returning
 * the resulting A, B, C, D, and E values after the block is processed. This function converts the given block from an array of 64-bytes to an array of 16 longwords,
 * then calls hashRound five times to perform the left side iterations, and five more times to perform all the right-side iterations. At the end, it combines the resulting
 * state values with original input state values to produce the output state
 * @param state the input state
 * @param block the passed block of 64-bytes
*/
void hashBlock( HashState *state, byte block[ BLOCK_BYTES ] )
{
  // we know that our buffer has a number of elements that is a multiple of 64
  // so we can use memcpy to pick each 64 byte array in our buffer until we hit len
  // then we know we are done, we have already passed the first 64 bytes when we called this method

  // define our static local arrays
  static int leftPerm0[ RIPE_ITERATIONS ] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
  static int leftPerm1[ RIPE_ITERATIONS ] = {	7, 4, 13, 1, 10, 6, 15, 3, 12, 0, 9, 5, 2, 14, 11, 8 };
  static int leftPerm2[ RIPE_ITERATIONS ] = { 3, 10, 14, 4, 9, 15, 8, 1, 2, 7, 0, 6, 13, 11, 5, 12 };
  static int leftPerm3[ RIPE_ITERATIONS ] = { 1, 9, 11, 10, 0, 8, 12, 4, 13, 3, 7, 15, 14, 5, 6, 2 };
  static int leftPerm4[ RIPE_ITERATIONS ] = { 4, 0, 5, 9, 7, 12, 2, 10, 14, 1, 3, 8, 11, 6, 15, 13 };
  static int rightPerm0[ RIPE_ITERATIONS ] = { 5, 14, 7, 0, 9, 2, 11, 4, 13, 6, 15, 8, 1, 10, 3, 12 };
  static int rightPerm1[ RIPE_ITERATIONS ] = { 6, 11, 3, 7, 0, 13, 5, 10, 14, 15, 8, 12, 4, 9, 1, 2 };
  static int rightPerm2[ RIPE_ITERATIONS ] = { 15, 5, 1, 3, 7, 14, 6, 9, 11, 8, 12, 2, 10, 0, 4, 13 };
  static int rightPerm3[ RIPE_ITERATIONS ] = { 8, 6, 4, 1, 3, 11, 15, 0, 5, 12, 2, 13, 9, 7, 10, 14 };
  static int rightPerm4[ RIPE_ITERATIONS ] = { 12, 15, 10, 4, 1, 5, 8, 7, 6, 2, 13, 14, 0, 3, 9, 11 };
  static int leftShift0[ RIPE_ITERATIONS ] = { 11, 14, 15, 12, 5, 8, 7, 9, 11, 13, 14, 15, 6, 7, 9, 8 };
  static int leftShift1[ RIPE_ITERATIONS ] = { 7, 6, 8, 13, 11, 9, 7, 15, 7, 12, 15, 9, 11, 7, 13, 12 };
  static int leftShift2[ RIPE_ITERATIONS ] = { 11, 13, 6, 7, 14, 9, 13, 15, 14, 8, 13, 6, 5, 12, 7, 5 };
  static int leftShift3[ RIPE_ITERATIONS ] = { 11, 12, 14, 15, 14, 15, 9, 8, 9, 14, 5, 6, 8, 6, 5, 12 };
  static int leftShift4[ RIPE_ITERATIONS ] = { 9, 15, 5, 11, 6, 8, 13, 12, 5, 12, 13, 14, 11, 8, 5, 6 };
  static int rightShift0[ RIPE_ITERATIONS ] = { 8, 9, 9, 11, 13, 15, 15, 5, 7, 7, 8, 11, 14, 14, 12, 6 };
  static int rightShift1[ RIPE_ITERATIONS ] = { 9, 13, 15, 7, 12, 8, 9, 11, 7, 7, 12, 7, 6, 15, 13, 11 };
  static int rightShift2[ RIPE_ITERATIONS ] = { 9, 7, 15, 11, 8, 6, 6, 14, 12, 13, 5, 14, 13, 13, 7, 5 };
  static int rightShift3[ RIPE_ITERATIONS ] = { 15, 5, 8, 11, 14, 14, 6, 14, 6, 9, 12, 9, 12, 5, 15, 8 };
  static int rightShift4[ RIPE_ITERATIONS ] = { 8, 5, 12, 9, 12, 5, 14, 6, 8, 13, 6, 5, 15, 13, 11, 11 };
  static longword noiseLeft0 = 0x00000000;
  static longword noiseLeft1 = 0x5A827999;
  static longword noiseLeft2 = 0x6ED9EBA1;
  static longword noiseLeft3 = 0x8F1BBCDC;
  static longword noiseLeft4 = 0xA953FD4E;
  static longword noiseRight0 = 0x50A28BE6;
  static longword noiseRight1 = 0x5C4DD124;
  static longword noiseRight2 = 0x6D703EF3;
  static longword noiseRight3 = 0x7A6D76E9;
  static longword noiseRight4 = 0x00000000;

  // REMEMBER FOR BITWISE FUNCTIONS ON RIGHT SIDE ROUNDS, BITWISE FUNCTIONS ARE CALLED IN REVERSE
  // MEANING ON RIGHT SIDE ROUND 0, BITWISEF4 IS CALLED

  // index calculation

  longword lWords[ RIPE_ITERATIONS ] = {};

  for( int i = 0; i < RIPE_ITERATIONS; i++ ) {
    for( int j = 0; j < LW_BYTES; j++ ){
      // this index calculation is the same as saying
      // 4 * i + 0, 1, 2, and 3,
      // I want to give credit to Piazza Post #778 by Haoze Du for figuring the insertion index out
      longword x = block[ LW_BYTES * i + j];
      // add each 8 bit byte to our longword at index at i
      // shift by our index j * 8 bits so we can add 4 bytes to each
      // longword = a 32 bit unsigned int
      lWords[ i ] |= x << ( j * BBITS );
    }
  }

  // create pointers to left, and right states after performing
  // left and right side iterations so we can combine resulting state
  // values at the end of this function
  // for testing
  HashState leftRoundsState = *state;
  HashState rightRoundsState = *state;
  HashState startingState = *state;

  // perform 5 left round rotations
  
  // iteration 0 left side
  hashRound( &leftRoundsState, lWords, leftPerm0, leftShift0, noiseLeft0, bitwiseF0 );

  // iteration 1 left side
  hashRound( &leftRoundsState, lWords, leftPerm1, leftShift1, noiseLeft1, bitwiseF1 );

  // iteration 2 left side
  hashRound( &leftRoundsState, lWords, leftPerm2, leftShift2, noiseLeft2, bitwiseF2 );

  // iteration 3 left side
  hashRound( &leftRoundsState, lWords, leftPerm3, leftShift3, noiseLeft3, bitwiseF3 );

  // iteration 4 left side
  hashRound( &leftRoundsState, lWords, leftPerm4, leftShift4, noiseLeft4, bitwiseF4 );

  // perform 5 right round rotations

  // iteration 0 right side
  hashRound( &rightRoundsState, lWords, rightPerm0, rightShift0, noiseRight0, bitwiseF4 );

  // iteration 1 right side
  hashRound( &rightRoundsState, lWords, rightPerm1, rightShift1, noiseRight1, bitwiseF3 );

  // iteration 2 right side
  hashRound( &rightRoundsState, lWords, rightPerm2, rightShift2, noiseRight2, bitwiseF2 );

  // iteration 3 right side
  hashRound( &rightRoundsState, lWords, rightPerm3, rightShift3, noiseRight3, bitwiseF1 );

  // iteration 4 right side
  hashRound( &rightRoundsState, lWords, rightPerm4, rightShift4, noiseRight4, bitwiseF0 );

  // calculate final states and print
  // A's final state
  state->A = startingState.B + leftRoundsState.C + rightRoundsState.D;
  // B's final state
  state->B = startingState.C + leftRoundsState.D + rightRoundsState.E;
  // C's final state
  state->C = startingState.D + leftRoundsState.E + rightRoundsState.A;
  // D's final state
  state->D = startingState.E + leftRoundsState.A + rightRoundsState.B;
  // E's final state
  state->E = startingState.A + leftRoundsState.B + rightRoundsState.C;
}

// Put the following at the end of your implementation file.
// If we're compiling for unit tests, create wrappers for the otherwise
// private functions we'd like to be able to test.

#ifdef TESTABLE

longword bitwiseF0Wrapper( longword b, longword c, longword d )
{
  return bitwiseF0( b, c, d );
}

longword bitwiseF1Wrapper( longword b, longword c, longword d )
{
  return bitwiseF1( b, c, d );
}

longword bitwiseF2Wrapper( longword b, longword c, longword d )
{
  return bitwiseF2( b, c, d );
}

longword bitwiseF3Wrapper( longword b, longword c, longword d )
{
  return bitwiseF3( b, c, d );
}

longword bitwiseF4Wrapper( longword b, longword c, longword d )
{
  return bitwiseF4( b, c, d );
}

longword rotateLeftWrapper( longword value, int s )
{
  return rotateLeft( value, s );
}

void hashIterationWrapper( HashState *state,
                           longword datum,
                           int shift,
                           longword noise,
                           BitwiseFunction f )
{
  hashIteration( state, datum, shift, noise, f );
}

void hashRoundWrapper( HashState *state,
                       longword *block,
                       int perm[ RIPE_ITERATIONS ],
                       int shift[ RIPE_ITERATIONS ],
                       longword noise,
                       BitwiseFunction f )
{
  hashRound( state, block, perm, shift, noise, f );
}

#endif
