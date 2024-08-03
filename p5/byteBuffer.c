/**
 * @file byteBuffer.h
 * @author Jake Donovan (jmpatte8)
 * This file is responsible for reading and storing contents
 * of binary files and text files in memory
*/

#include "byteBuffer.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * This function dynamically allocates a ByteBuffer struct and intializes
 * it's fields to represent an empty resizable array of bytes
 * @return a new ByteBuffer struct
*/
ByteBuffer *createBuffer()
{
    ByteBuffer *buffer = ( ByteBuffer * )malloc( sizeof( ByteBuffer ) );
    buffer->data = ( byte * )malloc( INITIAL_CAPACITY * sizeof( byte ) );
    buffer->cap = INITIAL_CAPACITY;
    buffer-> len = 0;
    return buffer;
}

/**
 * This function adds a singly byte to the end of the given buffer, enlarging
 * the data array if necessary
 * @param buffer the passed pointer to a ByteBuffer struct
 * @param b the byte we want to add
*/
void addByte( ByteBuffer *buffer, byte b )
{
    if( buffer->len >= buffer->cap ){
        buffer->cap *= RESIZE;
        buffer->data = ( byte * )realloc( buffer->data, buffer->cap * sizeof( byte ) );
    }

    buffer->data[ buffer->len++ ] = b;
}

/**
 * This function is responsible for freeing all memory for the passed
 * ByteBuffer struct
 * @param buffer the passed ByteBuffer struct
*/
void freeBuffer( ByteBuffer *buffer )
{
    free( buffer->data );
    free( buffer );
}

/**
 * This function creates a new ByteBuffer. It initializes the contents
 * of the data array with the contents of the given file. The file should be read in binary.
 * If the file cannot be opened, this function will just return NULL. After creating
 * the buffer from the input file, the buffer's capacity can be any (non-zero) value that is at least
 * as large as the file length. The buffer needs to be padded with additional bytes after it is created,
 * so you need to be sure addByte() works after reading a ByteBuffer from a file.
 * @param filename the binary file we want to read from
 * @return our ByteBuffer that we filled with the contents from the param binary file
*/
ByteBuffer *readFile( const char *filename )
{
    // construct a file pointer to read in binary mode
    FILE *fp = fopen( filename, "rb" );

    // if we could not open that passed filename, return NULL
    if( !fp ){
        return NULL;
    }

    // construct our ByteBuffer
    ByteBuffer * buffer = createBuffer();

    // seek to the end of the file
    fseek( fp, 0, SEEK_END );

    // get the current location in the file = file length
    long length = ftell( fp );

    // rewind to the start of the file
    rewind( fp );

    // create an array of bytes with size of length + 1
    byte bufferArray[ length ];

    // read file contents in to buffer using fread()
    fread( bufferArray, sizeof( byte ), sizeof( bufferArray ) / sizeof( byte ), fp);

    // save the length of the bufferArray for our for loop
    int byteArrayLen = sizeof( bufferArray ) / sizeof( byte );

    // copy contents of bufferArray in to our buffer by passing to addByte()
    for( int i = 0; i < byteArrayLen; i++ ) {
        addByte( buffer, bufferArray[ i ] );
    }

    // close fp
    fclose( fp );

    // return buffer
    return buffer;
}
