/**
 * @file syntax.c
 * @author Jake Donovan (jmpatte8)
 * This file is responsible for constructing all required structs, and necessary functions which can include eval and destroy to manage
 * each of our file's structs
*/

#include "syntax.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//////////////////////////////////////////////////////////////////////
// Error-reporting functions

/** Report an error for a program with bad types, then exit. */
static int reportTypeMismatch()
{
  fprintf( stderr, "Type mismatch\n" );
  exit( EXIT_FAILURE );
}

/** Require a given value to be an IntType value.  Exit with an error
    message if not.
    @param v value to check, passed by address.
 */
static void requireIntType( Value const *v )
{
  if ( v->vtype != IntType )
    reportTypeMismatch();
}

/**
 * This function is responsible for checking a passed value to verify that it is a sequence, 
 * exits unsucessfully if the value is not a sequence
 * @param v the value we want to examine
*/
static void requireSequence( Value const *v ){
  if(v->vtype != SeqType)
    reportTypeMismatch();
}

//////////////////////////////////////////////////////////////////////
// LiteralInt

/** Representation for a LiteralInt expression, a subclass of Expr that
    evaluates to a constant value. */
typedef struct {
  Value (*eval)( Expr *expr, Environment *env );
  void (*destroy)( Expr *expr );

  /** Integer value this expression evaluates to. */
  int val;
} LiteralInt;

/** Implementation of eval for LiteralInt expressions. */
static Value evalLiteralInt( Expr *expr, Environment *env )
{
  // If this function gets called, expr must really be a LiteralInt.
  LiteralInt *this = (LiteralInt *)expr;

  // Return an int value containing a copy of the value we represent.
  return (Value){ IntType, .ival = this->val };
}

/** Implementation of destroy for LiteralInt expressions. */
static void destroyLiteralInt( Expr *expr )
{
  // This object is just one block of memory.  We can free it without
  // even having to type-cast its pointer.
  free( expr );
}

/** Implementation of makeLiteralInt to constuct a new LiteralInt */
Expr *makeLiteralInt( int val )
{
  // Allocate space for the LiteralInt object
  LiteralInt *this = (LiteralInt *) malloc( sizeof( LiteralInt ) );

  // Remember the pointers to functions for evaluating and destroying ourself.
  this->eval = evalLiteralInt;
  this->destroy = destroyLiteralInt;

  // Remember the integer value we contain.
  this->val = val;

  // Return the result, as an instance of the Expr superclass.
  return (Expr *) this;
}

//////////////////////////////////////////////////////////////////////
// SimpleExpr Struct

/** Representation for an expression with either one or two
    sub-expressionts.  With the right eval funciton, this struct should
    be able to help implement any expression with either one or two
    sub-expressiosn. */
typedef struct {
  Value (*eval)( Expr *expr, Environment *env );
  void (*destroy)( Expr *oper );

  /** The first sub-expression */
  Expr *expr1;
  
  /** The second sub-expression, or NULL if it's not needed. */
  Expr *expr2;
} SimpleExpr;

/** General-purpose function for freeing an expression represented by
    SimpleExpr.  It frees the two sub-expressions, then frees the strucct
    itself. */
static void destroySimpleExpr( Expr *expr )
{
  // If this function gets called, expr must really be a SimpleExpr.
  SimpleExpr *this = (SimpleExpr *)expr;

  // Free the first sub-expression.
  this->expr1->destroy( this->expr1 );

  // Free the second one, if it exists.
  if ( this->expr2 )
    this->expr2->destroy( this->expr2 );

  // Then the SimpleExpr struct itself.
  free( this );
}

/** Helper funciton to construct a SimpleExpr representation and fill
    in the fields.
    @param first sub-expression in the expression.
    @param second sub-expression in the expression, or null if it only
    has one sub-expression.
    @param eval function implementing the eval mehod for this expression.
    @return new expression, as a poiner to Expr.
*/
static Expr *buildSimpleExpr( Expr *expr1, Expr *expr2,
                              Value (*eval)( Expr *, Environment * ) )
{
  // Allocate space for a new SimpleExpr and fill in the pointer for
  // its destroy function.
  SimpleExpr *this = (SimpleExpr *) malloc( sizeof( SimpleExpr ) );
  this->destroy = destroySimpleExpr;

  // Fill in the two parameters and the eval funciton.
  this->eval = eval;
  this->expr1 = expr1;
  this->expr2 = expr2;

  return (Expr *) this;
}

//////////////////////////////////////////////////////////////////////
// Integer addition

/** Implementation of the eval function for integer addition. */
static Value evalAdd( Expr *expr, Environment *env )
{
  // If this function gets called, expr must really be a SimpleExpr.
  SimpleExpr *this = (SimpleExpr *)expr;

  // Evaluate our left and right operands. 
  Value v1 = this->expr1->eval( this->expr1, env );
  Value v2 = this->expr2->eval( this->expr2, env );

  // Make sure the operands are both integers.
  requireIntType( &v1 );
  requireIntType( &v2 );

  // Return the sum of the two expression values.
  return (Value){ IntType, .ival = v1.ival + v2.ival };
}

/** Implementation of makeAdd to construct a new add expression */
Expr *makeAdd( Expr *left, Expr *right )
{
  // Use the convenience function to build a SimpleExpr for addition
  return buildSimpleExpr( left, right, evalAdd );
}

/** Implementation of evalLen to evaluate the length of a passed expression if it is a sequence */
static Value evalLen(Expr *expr, Environment *env) {
  SimpleExpr *this = (SimpleExpr *)expr;

  // evaluate a sequence 
  Value v = this->expr1->eval(this->expr1, env);

  // require a sequence
  requireSequence(&v);

  // grab sequence
  grabSequence(v.sval);

  // release sequence
  releaseSequence(v.sval);

  // return the sequence's length aka number of elements in the sequence
  return (Value){ IntType, v.sval->len };
}

/** Implementation of makeLenExpr to construct a new len expr by creating expr as a SimpleExpr */
Expr *makeLenExpr( Expr *expr ){
  return buildSimpleExpr(expr, NULL, evalLen);
}

//////////////////////////////////////////////////////////////////////
// Integer subtracton

/** Implementation of the eval function for integer subtraction. */
static Value evalSub( Expr *expr, Environment *env )
{
  // If this function gets called, expr must really be a SimpleExpr.
  SimpleExpr *this = (SimpleExpr *)expr;

  // Evaluate our left and right operands. 
  Value v1 = this->expr1->eval( this->expr1, env );
  Value v2 = this->expr2->eval( this->expr2, env );

  // Make sure the operands are both integers.
  requireIntType( &v1 );
  requireIntType( &v2 );

  // Return the difference of the two expression values.
  return (Value){ IntType, .ival = v1.ival - v2.ival };
}

/** Implementation of makeSub which creates a new SimpleExpr for subtraction */
Expr *makeSub( Expr *left, Expr *right )
{
  // Use the convenience function to build a SimpleExpr for subtraction.
  return buildSimpleExpr( left, right, evalSub );
}

//////////////////////////////////////////////////////////////////////
// Integer multiplication

/** Implementation of the eval function for integer multiplication. */
static Value evalMul( Expr *expr, Environment *env )
{
  // If this function gets called, expr must really be a SimpleExpr.
  SimpleExpr *this = (SimpleExpr *)expr;

  // Evaluate our left and right operands. 
  Value v1 = this->expr1->eval( this->expr1, env );
  Value v2 = this->expr2->eval( this->expr2, env );

  // Make sure the operands are both integers.
  requireIntType( &v1 );
  requireIntType( &v2 );

  // Return the product of the two expression.
  return (Value){ IntType, .ival = v1.ival * v2.ival };
}

/** Implementation of makeMul to construct a new SimpleExpr for multiplying expressions */
Expr *makeMul( Expr *left, Expr *right )
{
  // Use the convenience function to build a SimpleExpr for multiplication.
  return buildSimpleExpr( left, right, evalMul );
}

//////////////////////////////////////////////////////////////////////
// Integer division

/** Implementation of the eval function for integer division. */
static Value evalDiv( Expr *expr, Environment *env )
{
  // If this function gets called, expr must really be a SimpleExpr.
  SimpleExpr *this = (SimpleExpr *)expr;

  // Evaluate our left and right operands. 
  Value v1 = this->expr1->eval( this->expr1, env );
  Value v2 = this->expr2->eval( this->expr2, env );

  // Make sure the operands are both integers.
  requireIntType( &v1 );
  requireIntType( &v2 );

  // Catch it if we try to divide by zero.
  if ( v2.ival == 0 ) {
    fprintf( stderr, "Divide by zero\n" );
    exit( EXIT_FAILURE );
  }

  // Return the quotient of the two expression.
  return (Value){ IntType, .ival = v1.ival / v2.ival };
}

/** Implementation of makeDiv which creates a new SimpleExpr for dividing expressions */
Expr *makeDiv( Expr *left, Expr *right )
{
  // Use the convenience function to build a SimpleExpr for division.
  return buildSimpleExpr( left, right, evalDiv );
}

//////////////////////////////////////////////////////////////////////
// Logical and
/** Implementation of evalAdd which adds two expressions and returns the result */
static Value evalAnd( Expr *expr, Environment *env )
{
  // If this function gets called, expr must really be a SimpleExpr.
  SimpleExpr *this = (SimpleExpr *)expr;

  // Evaluate the left operand; return immediately if it's false.
  Value v1 = this->expr1->eval( this->expr1, env );
  requireIntType( &v1 );
  if ( v1.ival == 0 )
    return v1;
  
  // Evaluate the right operand.
  Value v2 = this->expr2->eval( this->expr2, env );
  requireIntType( &v2 );

  // Return true if the right-hand operand is true.
  return v2;
}

/** Implementation of makeAnd which constructs a new SimpleExpr for comparing expressions */
Expr *makeAnd( Expr *left, Expr *right )
{
  // Use the convenience function to build a SimpleExpr for the logical and.
  return buildSimpleExpr( left, right, evalAnd );
}

//////////////////////////////////////////////////////////////////////
// Logical or
/** Impelmentation of evalOr which evaluates two expressions with logical or */
static Value evalOr( Expr *expr, Environment *env )
{
  // If this function gets called, expr must really be a SimpleExpr.
  SimpleExpr *this = (SimpleExpr *)expr;

  // Evaluate the left operand; return immediately if it's true.
  Value v1 = this->expr1->eval( this->expr1, env );
  requireIntType( &v1 );
  if ( v1.ival )
    return v1;
  
  // Evaluate the right operand
  Value v2 = this->expr2->eval( this->expr2, env );
  requireIntType( &v2 );

  // Return true if the right-hand operand is true.
  return v2;
}

/** Implementation of makeOr which constructs a new SimpleExpr for logical or */
Expr *makeOr( Expr *left, Expr *right )
{
  // Use the convenience function to build a SimpleExpr for the logical or
  return buildSimpleExpr( left, right, evalOr );
}

//////////////////////////////////////////////////////////////////////
// Less-than comparison

/** Implementation of eval for the less than operator. */
static Value evalLess( Expr *expr, Environment *env )
{
  // If this function gets called, expr must really be a SimpleExpr.
  SimpleExpr *this = (SimpleExpr *)expr;

  // Evaluate our left and right operands. 
  Value v1 = this->expr1->eval( this->expr1, env );
  Value v2 = this->expr2->eval( this->expr2, env );

  // Make sure the operands are both the same type.
  if ( v1.vtype != v2.vtype )
    reportTypeMismatch();

  if ( v1.vtype == IntType ) {
    // Is v1 less than v2
    return (Value){ IntType, .ival = v1.ival < v2.ival ? true : false };
  } else {
    // Replace with code to compare sequences.

    // first check the len of both sequences
    // v1 len is the same as v2
    if( v1.sval->len == v2.sval->len ){
      bool v1IsLess = false;
      bool v2IsLess = false;
      int idx = 0;

      while(!v1IsLess && !v2IsLess && idx < v1.sval->len){
        if(v1.sval->data[idx] < v2.sval->data[idx]){
          v1IsLess = true;
        }

        else if(v1.sval->data[idx] > v2.sval->data[idx]){
          v2IsLess = true;
        }

        idx++;
      }

      if(v1IsLess){
        // return true
        return (Value){IntType, .ival = 1};
      }

      else if(v2IsLess){
        // return false
        return (Value){IntType, .ival = 0};
      }

      else {
        // return false if they are equal
        return (Value){IntType, .ival = 0};
      }
    }

    // v1 len is less than v2
    else if ( v1.sval->len < v2.sval->len) {
      bool v1IsLess = false;
      bool v2IsLess = false;
      int idx = 0;

      while(!v1IsLess && !v2IsLess && idx < v1.sval->len){
        if(v1.sval->data[idx] < v2.sval->data[idx]){
          v1IsLess = true;
        }

        else if(v1.sval->data[idx] > v2.sval->data[idx]){
          v2IsLess = true;
        }

        idx++;
      }

      if(v1IsLess){
        // return true
        return (Value){IntType, .ival = 1};
      }

      else if(v2IsLess){
        // return false
        return (Value){IntType, .ival = 0};
      }

      else {
        // return true, v2 is longer and all other elements
        // are equall
        return (Value){IntType, .ival = 1};
      }
    }

    // v1 len is greater than v2
    else {
      bool v1IsLess = false;
      bool v2IsLess = false;
      int idx = 0;

      while(!v1IsLess && !v2IsLess && idx < v2.sval->len){
        if(v1.sval->data[idx] < v2.sval->data[idx]){
          v1IsLess = true;
        }

        else if(v1.sval->data[idx] > v2.sval->data[idx]){
          v2IsLess = true;
        }

        idx++;
      }

      if(v1IsLess){
        // return true
        return (Value){IntType, .ival = 1};
      }

      else if(v2IsLess){
        // return false
        return (Value){IntType, .ival = 0};
      }

      else {
        // return false v1 is longer and we are equal
        // when comparing against all elements in v2 from
        // 0-> v2.sval.len - 1
        return (Value){IntType, .ival = 0};
      }
    }
  }
}

/** Implementation of makeLess which constructs a new SimpleExpr for verifying expressions our less than eachother or not */
Expr *makeLess( Expr *left, Expr *right )
{
  // Use the convenience function to build a SimpleExpr for the less-than
  // comparison.
  return buildSimpleExpr( left, right, evalLess );
}

//////////////////////////////////////////////////////////////////////
// Equality comparison

/** Eval function for an equality test. */
static Value evalEquals( Expr *expr, Environment *env )
{
  // If this function gets called, expr must really be a SimpleExpr.
  SimpleExpr *this = (SimpleExpr *)expr;

  // Evaluate our left and right operands. 
  Value v1 = this->expr1->eval( this->expr1, env );
  Value v2 = this->expr2->eval( this->expr2, env );

  // Make sure the same type.
  if ( v1.vtype == IntType && v2.vtype == IntType ) {
    return (Value){ IntType, .ival = ( v1.ival == v2.ival ) };
  } else {
    // Replace with code to permit sequence-sequence comparison.
    // A sequence can also be compared to an int, but they should
    // never be considered equal.
    if(v1.vtype == SeqType && v2.vtype == SeqType){
      if(v1.sval->len == v2.sval->len){
        bool notSame = false;
        int idx = 0;

        while(!notSame && idx < v1.sval->len){
          if(v1.sval->data[idx] != v2.sval->data[idx]){
            notSame = true;
          }

          idx++;
        }

        if(!notSame){
          // Return true
          return (Value){ IntType, .ival = 1 };
        }

        else {
          // Return false
          return (Value){ IntType, .ival = 0};
        }
      }
      
      else {
        // Return false
        return (Value){ IntType, .ival = 0};
      }
    }

    // if they are not the same type return false
    else {
      return (Value){ IntType, .ival = 0};
    }
  }
}

/** Implementation of makeEquals which constructs a new SimpleExpr for using equal to operator */
Expr *makeEquals( Expr *left, Expr *right )
{
  // Use the convenience function to build a SimpleExpr for the equals test.
  return buildSimpleExpr( left, right, evalEquals );
}

//////////////////////////////////////////////////////////////////////
// Variable in an expression

/** Representation for an expression representing an occurrence of a
    variable, subclass of Expr. */
typedef struct {
  Value (*eval)( Expr *expr, Environment *env );
  void (*destroy)( Expr *expr );

  /** Name of the variable. */
  char name[ MAX_VAR_NAME + 1 ];
} VariableExpr;

/** Implementation of evalVariable for checking if two expressions are equal to eachother */
static Value evalVariable( Expr *expr, Environment *env )
{
  // If this function gets called, expr must really be a VariableExpr
  VariableExpr *this = (VariableExpr *) expr;

  // Get the value of this variable.
  Value val = lookupVariable( env, this->name );

  return val;
}

/** Implementation of destroy for Variable. */
static void destroyVariable( Expr *expr )
{
  free( expr );
}

/** Implementation of makeVariable which creates a new Variable to use in our files */
Expr *makeVariable( char const *name )
{
  // Allocate space for the Variable statement, and fill in its function
  // pointers and a copy of the variable name.
  VariableExpr *this = (VariableExpr *) malloc( sizeof( VariableExpr ) );
  this->eval = evalVariable;
  this->destroy = destroyVariable;
  strcpy( this->name, name );

  return (Expr *) this;
}

//////////////////////////////////////////////////////////////////////
// SimpleStmt Struct

/** Generic representation for a statement that contains one or two
    expressions.  With different execute methods, this same struct
    can be used to represent print and push statements. */
typedef struct {
  void (*execute)( Stmt *stmt, Environment *env );
  void (*destroy)( Stmt *stmt );

  /** First (or only) expression used by this statement. */
  Expr *expr1;
  /** Second expression used by this statement, or null */
  Expr *expr2;
} SimpleStmt;

/** Generic destroy function for SimpleStmt, with either one
    or two sub-expressions. */
static void destroySimpleStmt( Stmt *stmt )
{
  // If this function gets called, stmt must really be a SimpleStmt.
  SimpleStmt *this = (SimpleStmt *)stmt;

  // Free our subexpression then the SimpleStmt object itself.
  this->expr1->destroy( this->expr1 );
  if ( this->expr2 )
    this->expr2->destroy( this->expr2 );
  free( this );
}

//////////////////////////////////////////////////////////////////////
// Print Statement

/** Implementation of execute for a print statement */
static void executePrint( Stmt *stmt, Environment *env )
{
  // If this function gets called, stmt must really be a SimpleStmt.
  SimpleStmt *this = (SimpleStmt *)stmt;

  // Evaluate our argument.
  Value v = this->expr1->eval( this->expr1, env );

  // Print the value of our expression appropriately, based on its type.
  if ( v.vtype == IntType ) {
    printf( "%d", v.ival );
  } else {
    grabSequence(v.sval);
    // Replace with code to permit print a sequence as a string of
    // ASCII character codes.
    for(int i = 0; i < v.sval->len; i++){
      putchar(v.sval->data[i]);
    }

    releaseSequence(v.sval);
  }
}

/** Implementation of makePrint which creats a new SimpleStmt for printing */
Stmt *makePrint( Expr *expr )
{
  // Allocate space for the SimpleStmt object
  SimpleStmt *this = (SimpleStmt *) malloc( sizeof( SimpleStmt ) );

  // Remember the pointers to execute and destroy this statement.
  this->execute = executePrint;
  this->destroy = destroySimpleStmt;

  // Remember the expression for the thing we're supposed to print.
  this->expr1 = expr;
  this->expr2 = NULL;

  // Return the SimpleStmt object, as an instance of the Stmt interface.
  return (Stmt *) this;
}

//////////////////////////////////////////////////////////////////////
// Compound Statement

/** Representation for a compound statement, derived from Stmt. */
typedef struct {
  void (*execute)( Stmt *stmt, Environment *env );
  void (*destroy)( Stmt *stmt );

  /** Number of statements in the compound. */
  int len;
  
  /** List of statements in the compound. */
  Stmt **stmtList;
} CompoundStmt;

/** Implementation of execute for CompountStmt */
static void executeCompound( Stmt *stmt, Environment *env )
{
  // If this function gets called, stmt must really be a CompoundStmt.
  CompoundStmt *this = (CompoundStmt *)stmt;

  // Execute the sequence of statements in this compound
  for ( int i = 0; i < this->len; i++ )
    this->stmtList[ i ]->execute( this->stmtList[ i ], env );
}

/** Implementation of destroy for CompountStmt */
static void destroyCompound( Stmt *stmt )
{
  // If this function gets called, stmt must really be a CompoundStmt.
  CompoundStmt *this = (CompoundStmt *)stmt;

  // Free the list of statements inside this compond.
  for ( int i = 0; i < this->len; i++ )
    this->stmtList[ i ]->destroy( this->stmtList[ i ] );

  // Then, free the array of pointers and the compund statement itself.
  free( this->stmtList );
  free( this );
}

/** Implementation of makeCompound which constructs a new CompoundStmt in our files */
Stmt *makeCompound( int len, Stmt **stmtList )
{
  // Allocate space for the CompoundStmt object
  CompoundStmt *this = (CompoundStmt *) malloc( sizeof( CompoundStmt ) );

  // Remember the pointers to execute and destroy this statement.
  this->execute = executeCompound;
  this->destroy = destroyCompound;

  // Remember the list of statements in the compound.
  this->len = len;
  this->stmtList = stmtList;

  // Return the result, as an instance of the Stmt interface.
  return (Stmt *) this;
}

///////////////////////////////////////////////////////////////////////
// ConditioanlStatement (for while/if)

/** Representation for either a while or if statement, subclass of Stmt. */
typedef struct {
  void (*execute)( Stmt *stmt, Environment *env );
  void (*destroy)( Stmt *stmt );

  // Condition to be checked before running the body.
  Expr *cond;

  // Body to execute if / while cond is true.
  Stmt *body;
} ConditionalStmt;

/** Implementation of destroy for either while of if statements. */
static void destroyConditional( Stmt *stmt )
{
  // If this function gets called, stmt must really be a ConditionalStmt.
  ConditionalStmt *this = (ConditionalStmt *)stmt;

  // Destroy the condition expression and the statement in the body.
  this->cond->destroy( this->cond );
  this->body->destroy( this->body );

  // Then, free the ConditionalStmt struct.
  free( this );
}

///////////////////////////////////////////////////////////////////////
// if statement

/** Implementation of th execute function for an if statement. */
static void executeIf( Stmt *stmt, Environment *env )
{
  // If this function gets called, stmt must really be a ConditionalStmt.
  ConditionalStmt *this = (ConditionalStmt *)stmt;

  // Evaluate our operand and see if it's true.
  Value result = this->cond->eval( this->cond, env );
  requireIntType( &result );

  // Execute the body if the condition evaluated to true.
  if ( result.ival )
    this->body->execute( this->body, env );
}

/** Implementation of makeIf which constructs a new if conditional statement for comparing expressions */
Stmt *makeIf( Expr *cond, Stmt *body )
{
  // Allocate an instance of ConditionalStmt
  ConditionalStmt *this =
    (ConditionalStmt *) malloc( sizeof( ConditionalStmt ) );

  // Functions to execute and destroy an if statement.
  this->execute = executeIf;
  this->destroy = destroyConditional;

  // Fill in the condition and the body of the if.
  this->cond = cond;
  this->body = body;

  // Return the result, as an instance of the Stmt interface.
  return (Stmt *) this;
}

///////////////////////////////////////////////////////////////////////
// while statement

/** Implementation of th execute function for a while statement. */
static void executeWhile( Stmt *stmt, Environment *env )
{
  // If this function gets called, stmt must really be a ConditionalStmt.
  ConditionalStmt *this = (ConditionalStmt *)stmt;

  // Evaluate our condition and see if it's true.
  Value result = this->cond->eval( this->cond, env );
  requireIntType( &result );
  
  // Execute the body while the condition evaluates to true.
  while ( result.ival ) {
    this->body->execute( this->body, env );
    
    // Get the value of the condition for the next iteration.
    result = this->cond->eval( this->cond, env );
    requireIntType( &result );
  }
}

/** Constructs a new ConditionalStmt for a while loop conditional */
Stmt *makeWhile( Expr *cond, Stmt *body )
{
  // Allocate an instance of ConditionalStmt
  ConditionalStmt *this =
    (ConditionalStmt *) malloc( sizeof( ConditionalStmt ) );

  // Functions to execute and destroy a while statement.
  this->execute = executeWhile;
  this->destroy = destroyConditional;

  // Fill in the condition and the body of the while.
  this->cond = cond;
  this->body = body;

  // Return the result, as an instance of the Stmt interface.
  return (Stmt *) this;
}

///////////////////////////////////////////////////////////////////////
// assignment statement

/** Representation of an assignment statement, a subclass of
    Stmt. This representation should be suitable for assigning to a
    variable or an element of a sequence.  */
typedef struct {
  void (*execute)( Stmt *stmt, Environment *env );
  void (*destroy)( Stmt *stmt );

  /** Name of the variable we're assigning to. */
  char name[ MAX_VAR_NAME + 1 ];
  
  /** If we're assigning to an element of a sequence, this is the index
      expression. Otherwise, it's zero. */
  Expr *iexpr;

  /** Expression for the right-hand side of the assignment (the source). */
  Expr *expr;
} AssignmentStmt;

/** Implementation of destroy for assignment Statements. */
static void destroyAssignment( Stmt *stmt )
{
  AssignmentStmt *this = (AssignmentStmt *)stmt;

  // Destroy the source expression and the index (if there is one)
  this->expr->destroy( this->expr );
  if ( this->iexpr )
    this->iexpr->destroy( this->iexpr );
  free( this );
}

/** Implementation of execute for assignment Statements. */
static void executeAssignment( Stmt *stmt, Environment *env )
{
  // If we get to this function, stmt must be an AssignmentStmt.
  AssignmentStmt *this = (AssignmentStmt *) stmt;

  // Evaluate the right-hand side of the equals.
  Value result = this->expr->eval( this->expr, env );
  
  if ( this->iexpr ) {
    Value idx = this->iexpr->eval( this->iexpr, env );
    // Replace with code to permit assigning to a sequence element.
    Value ret = lookupVariable( env, this->name);
    ret.sval->data[idx.ival] = result.ival;
  } else {
    if(result.vtype == SeqType){
      grabSequence(result.sval);
    }

    // It's a variable, change its value
    setVariable( env, this->name, result );
  }
}

/** Implementation of makeAssignment to create a new assigment statement */
Stmt *makeAssignment( char const *name, Expr *iexpr, Expr *expr )
{
  // Allocate the AssignmentStmt representations.
  AssignmentStmt *this =
    (AssignmentStmt *) malloc( sizeof( AssignmentStmt ) );

  // Fill in functions to execute or destory this statement.
  this->execute = executeAssignment;
  this->destroy = destroyAssignment;

  // Get a copy of the destination variable name, the source
  // expression and the sequence index (if it's non-null).
  strcpy( this->name, name );
  this->iexpr = iexpr;
  this->expr = expr;

  // Return this object, as an instance of Stmt.
  return (Stmt *) this;
}

/** Implementation of execute for push Statements. */
static void executePush( Stmt *stmt, Environment *env )
{
  // If we get to this function, stmt must be an AssignmentStmt.
  SimpleStmt *this = (SimpleStmt *) stmt;

  Value sequence = this->expr1->eval(this->expr1, env );

  requireSequence(&sequence);

  Value val = this->expr2->eval(this->expr2, env );

  requireIntType(&val);

  if(sequence.sval->len >= sequence.sval->cap){
    sequence.sval->cap *= 2;
    sequence.sval->data = realloc(sequence.sval->data, sequence.sval->cap * sizeof( int ));  
  }

  sequence.sval->data[sequence.sval->len++] = val.ival;
}

/** Implementation of makePush for making push statements */
Stmt *makePush( Expr *sexpr, Expr *vexpr )
{
  // Allocate the AssignmentStmt representations.
  SimpleStmt * this = ( SimpleStmt * )malloc( sizeof( SimpleStmt ) );

  this->execute = executePush;

  this->destroy = destroySimpleStmt;

  this->expr1 = sexpr;

  this->expr2 = vexpr;

  return (Stmt *)this;
}

/**
 * Construct a SequenceInitializer struct for creating sequences of values
*/
typedef struct {
    /** 
     * Our eval function
    */
    Value (*eval)(Expr *expr, Environment *env);
    /**
     * Our destroy function
    */
    void (*destroy)(Expr *expr );
    /**
     * The number of expressions in our SequenceInitializer
    */
    int len;
    /**
     * Our list of pointers to Expression pointers
    */
    Expr **exprList;
} SequenceInitializer;

/** Implentation of eval method for SequenceInitializer */
static Value evalSeqInti( Expr *expr, Environment *env ){
  SequenceInitializer *this = (SequenceInitializer *)expr;

  Sequence * ret = makeSequence();

  for(int i = 0; i < this->len; i++){
    if(ret->len >= ret->cap){
      ret->cap *= 2;
      ret->data = realloc(ret->data, ret->cap * sizeof( int ));
    }

    Value v = this->exprList[i]->eval(this->exprList[i], env);

    requireIntType(&v);

    ret->data[ret->len++] = (&v)->ival;
  }

  return (Value){SeqType, .sval = ret};
}

/** Implementation of destroy for our SequenceInitializer */
static void destroySeqInti( Expr *expr ){
  SequenceInitializer * this = (SequenceInitializer *)expr;

  for(int i = 0; i < this->len; i++){
    this->exprList[i]->destroy(this->exprList[i]);
  }

  free(this->exprList);

  free(this);
}

/** Implementation of makeSequenceInitializer to create a new SequenceInitializer */
Expr *makeSequenceInitializer( int len, Expr * eList[] ) {
  SequenceInitializer * this = ( SequenceInitializer * )malloc( sizeof( SequenceInitializer ) );

  this->eval = evalSeqInti;

  this->destroy = destroySeqInti;

  this->len = len;

  this->exprList = malloc(len * sizeof(Expr *));

  for(int i = 0; i < len; i++){
    this->exprList[i] = eList[i];
  }

  return (Expr *)this;
}

/** Implementation of eval method for SequenceIndexExpression */
static Value evalSeqIdx( Expr *expr, Environment *env ){
  SimpleExpr *this = (SimpleExpr *)expr;

  Value seq = this->expr1->eval(this->expr1, env);

  requireSequence(&seq);

  Value idx = this->expr2->eval(this->expr2, env);

  requireIntType(&idx);

  if(idx.ival >= seq.sval->len){
    // invalid index
    // print to stderr
    fprintf(stderr, "Index out of bounds\n");
    exit( EXIT_FAILURE );
  }

  return (Value){IntType, seq.sval->data[idx.ival]};
}

/** Implementation of makeSequenceIndex which makes a new SequenceIndexExpression */
Expr *makeSequenceIndex( Expr * aexpr, Expr * iexpr ){
  return buildSimpleExpr( aexpr, iexpr, evalSeqIdx);
}