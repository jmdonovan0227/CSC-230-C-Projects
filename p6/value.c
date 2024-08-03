/**
 * @file value.c
 * @author Jake Donovan (jmpatte8)
 * Constructs our Sequence and Environment in this file and this file also contains our functions for freeing memory for the sequences and environment
 * and also methods to modify our ref count in our sequences
*/

#include "value.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

//////////////////////////////////////////////////////////////////////
// Sequence.
/**
 * Createas and returns a new Sequence
 * @return seq our new sequence
*/
Sequence *makeSequence()
{
  Sequence *seq = (Sequence *)malloc(sizeof( Sequence ));
  seq->len = 0;
  seq->cap = 5;
  seq->data = (int *)malloc(seq->cap * sizeof( int ));
  seq->ref = 0;
}

/**
 * Free the memory for the passed sequence
 * @param seq the sequence we want to free
*/
void freeSequence( Sequence *seq )
{
  free(seq->data);
  free(seq);
}

/**
 * Increment our passed sequence's count which indicates it has been assigned to a variable or is temporarily
 * being used by an expression
 * @param seq the sequence that we want to increment it's ref count
*/
void grabSequence( Sequence *seq )
{
  seq->ref += 1;
}

/**
 * Decrement reference count for passed sequence and free memory if ref count is now 0 or less
 * @param seq the passed seq we want to decrement it's reference count
*/
void releaseSequence( Sequence *seq )
{
  seq->ref -= 1;

  if ( seq->ref <= 0 ) {
    assert( seq->ref == 0 );

    // Once the reference count hits zero, we can free the sequence memory.
    freeSequence( seq );
  }
}

//////////////////////////////////////////////////////////////////////
// Environment.
/**
 * Our Environment struct
*/
typedef struct {
  char name[ MAX_VAR_NAME + 1 ];
  Value val;
} VarRec;

// Hidden implementation of the environment.
struct EnvironmentStruct {
  VarRec *vlist;

  // Number of name/value pairs.
  int len;

  // Capacity of the name/value list.
  int capacity;
};

/**
 * Create a new environment to hold our variables and sequences
 * @return env the newly constructed environment
*/
Environment *makeEnvironment()
{
  Environment *env = (Environment *) malloc( sizeof( Environment ) );
  env->capacity = 5;
  env->len = 0;
  env->vlist = (VarRec *) malloc( sizeof( VarRec ) * env->capacity );
  return env;
}

/**
 * See if a variable already exists
 * @param env the environment where our variable is held
 * @param name the name of our variable
 * @return the found value or null otherwise aka 0 which means it was not found
*/
Value lookupVariable( Environment *env, char const *name )
{
  // Linear search for a variable name, not too efficient.
  for ( int i = 0; i < env->len; i++ )
    if ( strcmp( env->vlist[ i ].name, name ) == 0 )
      return env->vlist[ i ].val;

  // Return zero for uninitialized variables.
  return (Value){ IntType, .ival = 0 };
}

/**
 * Assign a variable a passed value
 * @param env the environment where we want to place the variable
 * @param name the name of the variable
 * @param value the value we want to set to our variable
*/
void setVariable( Environment *env, char const *name, Value value )
{
  int pos = 0;
  while ( pos < env->len && strcmp( env->vlist[ pos ].name, name ) != 0 )
    pos++;

  if ( pos >= env->len ) {
    if ( env->len >= env->capacity ) {
      env->capacity *= 2;
      env->vlist = (VarRec *) realloc( env->vlist, sizeof( VarRec ) * env->capacity );
    }
  }

  // Is this a new variable, or one that already existed?
  if ( pos == env->len ) {
    pos = env->len++;
    strcpy( env->vlist[ pos ].name, name );
  }
  
  env->vlist[ pos ].val = value;
}

/**
 * Free environment, all elements, and handle all reference counts
 * @param env the passed environment
*/
void freeEnvironment( Environment *env )
{
  for(int i = 0; i < env->len; i++){
    if(env->vlist[i].val.vtype == SeqType){
      releaseSequence(env->vlist[i].val.sval);
    }
  }

  free( env->vlist );
  free( env );
}

