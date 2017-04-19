#include "expr.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

//////////////////////////////////////////////////////////////////////
// Context

/** Representation for a variable anme and its value. */
typedef struct {
  char name[ MAX_IDENT_LEN + 1 ];
  char *val;
} VarRec;

/** Hidden implementation of the context.  Really just a wrapper
    around a resizable array of VarRec structs. */
struct ContextTag {
  // List of all varname/value pairs.
  VarRec *vlist;

  // Number of name/value pairs.
  int len;

  // Capacity of the name/value list.
  int capacity;
};

Context *makeContext()
{
  // You need to implement this.
  return NULL;
}

char const *getVariable( Context *ctxt, char const *name )
{
  // You need to implement this.
  return NULL;
}

void setVariable( Context *ctxt, char const *name, char *value )
{
  // You need to implement this.
}

void freeContext( Context *ctxt )
{
  // You need to implement this.
}

//////////////////////////////////////////////////////////////////////
// Literal

// Representation for a Literal expression, derived from Expr.
typedef struct {
  char *(*eval)( Expr *oper, Context *ctxt );
  void (*destroy)( Expr *oper );

  /** Literal value of this expression. */
  char *val;
} LiteralExpr;

// Function to evaluate a literal expression.
static char *evalLiteral( Expr *expr, Context *ctxt )
{
  // Cast the this pointer to a more specific type.
  LiteralExpr *this = (LiteralExpr *)expr;

  // Make and return a copy of the value we contain.
  char *result = (char *) malloc( strlen( this->val ) + 1 );
  strcpy( result, this->val );
  return result;
}

// Function to free a literal expression.
static void destroyLiteral( Expr *expr )
{
  // Cast the this pointer to a more specific type.
  LiteralExpr *this = (LiteralExpr *)expr;

  // Free the value we contain and the literal object itself.
  free( this->val );
  free( this );
}

Expr *makeLiteral( char *val )
{
  // Allocate space for the LiteralExpr object
  LiteralExpr *this = (LiteralExpr *) malloc( sizeof( LiteralExpr ) );

  // Remember our virutal functions.
  this->eval = evalLiteral;
  this->destroy = destroyLiteral;

  // Remember the literal string we contain.
  this->val = val;

  // Return the result, as an instance of the base.
  return (Expr *) this;
}

// For double values, this should be the longest representation that could
// get printed with %f, a large positive exponent and some fractional digits.
#define MAX_NUMBER 400

//////////////////////////////////////////////////////////////////////
// Sum expressions

/** Representation for a sum expression.  This struct could probably
    be used to represent lots of different binary expressions. */
typedef struct {
  char *(*eval)( Expr *oper, Context *ctxt );
  void (*destroy)( Expr *oper );

  // Two sub-expressions.
  Expr *leftExpr, *rightExpr;
} SumExpr;

// Destroy function for sum expression.
static void destroySum( Expr *expr )
{
  SumExpr *this = (SumExpr *)expr;

  // Free our operand subexpressions.
  this->leftExpr->destroy( this->leftExpr );
  this->rightExpr->destroy( this->rightExpr );

  // Then the struct itself.
  free( this );
}


// Eval function for a sum expression.
static char *evalSum( Expr *expr, Context *ctxt )
{
  // Get a pointer to the more specific type this function works with.
  SumExpr *this = (SumExpr *)expr;

  // Evaluate our two operands
  char *left = this->leftExpr->eval( this->leftExpr, ctxt );
  char *right = this->rightExpr->eval( this->rightExpr, ctxt );

  // Parse the left and right operands as doubles.  Set them
  // to zero if they don't parse correctly.
  double a, b;
  if ( sscanf( left, "%lf", &a ) != 1 )
    a = 0.0;

  if ( sscanf( right, "%lf", &b ) != 1 )
    b = 0.0;

  // We're done with the values returned by our two subexpressions,
  // We just needed to get them as doubles
  free( left );
  free( right );

  // Compute the result, store it in a dynamically allocated string
  // and return it to the caller.
  char *result = (char *)malloc( MAX_NUMBER + 1 );
  sprintf( result, "%f", a + b );
  return result;
}

Expr *makeSum( Expr *leftExpr, Expr *rightExpr )
{
  // Make an instance of SumExpr
  SumExpr *this = (SumExpr *) malloc( sizeof( SumExpr ) );
  this->destroy = destroySum;
  this->eval = evalSum;

  // Remember the two sub-expressions.
  this->leftExpr = leftExpr;
  this->rightExpr = rightExpr;

  // Return the instance as if it's an Expr (which it sort of is)
  return (Expr *) this;
}

//////////////////////////////////////////////////////////////////////
// Sub expressions

/** Representation for a sub expression.  This struct could probably
    be used to represent lots of different binary expressions. */
typedef struct {
  char *(*eval)( Expr *oper, Context *ctxt );
  void (*destroy)( Expr *oper );

  // Two sub-expressions.
  Expr *leftExpr, *rightExpr;
} SubExpr;

// Destroy function for sum expression.
static void destroySub( Expr *expr )
{
  SubExpr *this = (SubExpr *)expr;

  // Free our operand subexpressions.
  this->leftExpr->destroy( this->leftExpr );
  this->rightExpr->destroy( this->rightExpr );

  // Then the struct itself.
  free( this );
}


// Eval function for a sub expression.
static char *evalSub( Expr *expr, Context *ctxt )
{
  // Get a pointer to the more specific type this function works with.
  SubExpr *this = (SubExpr *)expr;

  // Evaluate our two operands
  char *left = this->leftExpr->eval( this->leftExpr, ctxt );
  char *right = this->rightExpr->eval( this->rightExpr, ctxt );

  // Parse the left and right operands as doubles.  Set them
  // to zero if they don't parse correctly.
  double a, b;
  if ( sscanf( left, "%lf", &a ) != 1 )
    a = 0.0;

  if ( sscanf( right, "%lf", &b ) != 1 )
    b = 0.0;

  // We're done with the values returned by our two subexpressions,
  // We just needed to get them as doubles
  free( left );
  free( right );

  // Compute the result, store it in a dynamically allocated string
  // and return it to the caller.
  char *result = (char *)malloc( MAX_NUMBER + 1 );
  sprintf( result, "%f", a - b );
  return result;
}

Expr *makeSub( Expr *leftExpr, Expr *rightExpr )
{
  // Make an instance of SubExpr
  SubExpr *this = (SubExpr *) malloc( sizeof( SubExpr ) );
  this->destroy = destroySub;
  this->eval = evalSub;

  // Remember the two sub-expressions.
  this->leftExpr = leftExpr;
  this->rightExpr = rightExpr;

  // Return the instance as if it's an Expr (which it sort of is)
  return (Expr *) this;
}
