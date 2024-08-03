/**
 * @file interpret.c
 * @author Jake Donovan
 * This file is responsible for running all included files in this program to correctly parse and perform each stmt and conditional specified in the passed
 * file
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "value.h"
#include "syntax.h"
#include "parse.h"

/** Print a usage message then exit unsuccessfully. */
void usage()
{
  fprintf( stderr, "usage: interpret <program-file>\n" );
  exit( EXIT_FAILURE );
}

/**
 * Program starting point, calls all files to parse a file and correctly perform all specified statements and operations
 * @param argc the number of command line arguments
 * @param argv an array of char pointers which point to each command line argument
 * @return program exit status
*/
int main( int argc, char *argv[] )
{
  // Open the program's source.
  if ( argc != 2 )
    usage();
  
  FILE *fp = fopen( argv[ 1 ], "r" );
  if ( !fp ) {
    perror( argv[ 1 ] );
    exit( EXIT_FAILURE );
  }

  // Environment, for storing variable values.
  Environment *env = makeEnvironment();
  
  // Parse one statement at a time, then run each statement
  // using the same Environment.
  char tok[ MAX_TOKEN + 1 ];
  while ( parseToken( tok, fp ) ) {
    // Parse the next input statement.
    Stmt *stmt = parseStmt( tok, fp );

    // Run the statement.
    stmt->execute( stmt, env );

    // Delete the statement.
    stmt->destroy( stmt );
  }
  
  // We're done, close the input file and free the environment.
  fclose( fp );
  freeEnvironment( env );

  return EXIT_SUCCESS;
}
