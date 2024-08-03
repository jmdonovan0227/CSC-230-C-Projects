/**
 * @file byteBuffer.h
 * @author Jake Donovan (jmpatte8)
 * This is the header file for byteBuffer.c which holds
 * all required structs and necessary functions to read and
 * store contents of binary files and text files in memory
*/

#ifndef _BYTE_BUFFER_H_
/** Define our byte buffer */
#define _BYTE_BUFFER_H_

/** Number of bits in a byte */
#define BBITS 8

/** Minimum number of command line arguments */
#define MIN_ARGS 2

/** Value for resizing our capacity for our resizable byte array in our buffer */
#define RESIZE 2

/** Initial capacity for buffer */
#define INITIAL_CAPACITY 5

/** Type used as a byte. */
typedef unsigned char byte;

/** Representation for a file copied to memory, with some padding
    at the end. */
typedef struct {
  /** Array of bytes from the file (no null termination, so it's not a
      string). */
  byte *data;

  /** Number of currently used bytes in the data array. */
  unsigned int len;

  /** Capacity of the data array (it's typically over-allocated. */
  unsigned int cap;
} ByteBuffer;

/** createBuffer() function prototype */
ByteBuffer *createBuffer();
/** addByte() function prototype */
void addByte( ByteBuffer *buffer, byte b );
/** freeBuffer() function prototype */
void freeBuffer( ByteBuffer *buffer );
/** readFile() function prototype */
ByteBuffer *readFile( const char * filename );

#endif
