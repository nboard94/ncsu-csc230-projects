#include "expr.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

//////////////////////////////////////////////////////////////////////
// Context

/** Representation for a variable name and its value. */
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

// Dynamically allocate a context struct, and returns a pointer to it.
Context *makeContext()
{
  Context *c = (Context *)malloc(sizeof(Context));
  c->len = 0;
  c->capacity = 2;
  
  c->vlist = (VarRec *) malloc( (c->capacity) * sizeof( VarRec ) );
  
  return c;
}

// Looks through the context's list of variables by name,
// returning the value of the variable when found.  Otherwise null.
char const *getVariable( Context *ctxt, char const *name )
{

  for ( int i = 0; i < (ctxt -> len); i++ ) {

    if ( strcmp( name, (ctxt->vlist)[i].name ) == 0 ) {

      return (ctxt->vlist)[i].val;
      
    }
  }
  return "";
  
  return NULL;
}

// Creates a VarRec, sets the name and value.
// Places the VarRec in the last position of the Context's vlist.
// Increments Context's vlist.
void setVariable( Context *ctxt, char const *name, char *value )
{
  VarRec *variable = (VarRec*)malloc(sizeof(VarRec));

  strcpy( ( variable -> name ), name );
  ( variable -> val ) = value;
  
  
  // Reassign a variable if it already exists.
  bool found = 0;
  for ( int i = 0; i < ctxt->len; i++) {

    if ( strcmp( (ctxt->vlist)[i].name, name ) == 0 ) {

     (ctxt->vlist[i]).val = variable->val;
     
      //printf("Current: %s\n", (ctxt->vlist[i]).val );
      found = 1;
    }

  }
  
  if ( ctxt->len >= ctxt->capacity ) {

    ctxt->capacity *= ctxt->len;
    ctxt->vlist = (VarRec *) realloc( ctxt->vlist, (ctxt->capacity) * sizeof( VarRec ) );
  }

  // Otherwise, add a new one.
  if (!found) {

    ( ctxt -> vlist )[(ctxt -> len)] = *variable;
    ( ctxt -> len )++;
  }
}

// Go through the Context's vlist, freeing each var.
// The free the Context itself.
void freeContext( Context *ctxt )
{
  //for( int i = 0; i < (ctxt -> len); i++ )
  //free( &( ( ctxt -> vlist )[i] ) );

  free( ctxt );
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
// Var expression

/** Representation for a varname.  This struct is probably based on a
    different one that could represents lots of different binary expressions. */
typedef struct {
  char *(*eval)( Expr *oper, Context *ctxt );
  void (*destroy)( Expr *oper );
  
  /** Value of this variable. */
  //char *val;
  char val[ MAX_IDENT_LEN + 1];
} VarExpr;

//Function to evaluate a var expression.
static char *evalVar( Expr *expr, Context *ctxt )
{
  // Cast the this pointer to a more specific type.
  VarExpr *this = (VarExpr *)expr;
  
  // Find in ctxt and return a copy of the value we contain.
  char *result = (char *) malloc( strlen( getVariable( ctxt, this->val ) ) + 1 );
  strcpy( result, getVariable(ctxt, this->val) );
  return result;
}


// Destroy function for a Var
static void destroyVar( Expr *expr )
{
  // Cast the this pointer to a more specific type.
  VarExpr *this = (VarExpr *)expr;
  
  // Free the value we contain the literal object itself.
  //free( this->val ); keep?
  free( this );
}

Expr *makeVar( char const *name )
{
  // Allocate space for the VarExpr object
  VarExpr *this = (VarExpr *) malloc( sizeof( VarExpr ) );

  // Remember our virtual functions.
  this->eval = evalVar;
  this->destroy = destroyVar;
  //this->val = (char *) malloc( sizeof( MAX_IDENT_LEN + 1) );

  // Remember the literal string we contain.
  strcpy( this->val, name );

  // Return the result, as an instance of the base.
  return (Expr *) this;
}


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
  //free( left );
  //free( right );

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
// Difference expressions

/** Representation for a difference expression.  This struct could probably
    be used to represent lots of different binary expressions. */
typedef struct {
  char *(*eval)( Expr *oper, Context *ctxt );
  void (*destroy)( Expr *oper );

  // Two sub-expressions.
  Expr *leftExpr, *rightExpr;
} DifferenceExpr;

// Destroy function for sum expression.
static void destroyDifference( Expr *expr )
{
  DifferenceExpr *this = (DifferenceExpr *)expr;

  // Free our operand subexpressions.
  this->leftExpr->destroy( this->leftExpr );
  this->rightExpr->destroy( this->rightExpr );

  // Then the struct itself.
  free( this );
}


// Eval function for a sub expression.
static char *evalDifference( Expr *expr, Context *ctxt )
{
  // Get a pointer to the more specific type this function works with.
  DifferenceExpr *this = (DifferenceExpr *)expr;

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
  //free( left );
  //free( right );

  // Compute the result, store it in a dynamically allocated string
  // and return it to the caller.
  char *result = (char *)malloc( MAX_NUMBER + 1 );
  sprintf( result, "%f", a - b );
  return result;
}

Expr *makeDifference( Expr *leftExpr, Expr *rightExpr )
{
  // Make an instance of DifferenceExpr
  DifferenceExpr *this = (DifferenceExpr *) malloc( sizeof( DifferenceExpr ) );
  this->destroy = destroyDifference;
  this->eval = evalDifference;

  // Remember the two sub-expressions.
  this->leftExpr = leftExpr;
  this->rightExpr = rightExpr;

  // Return the instance as if it's an Expr (which it sort of is)
  return (Expr *) this;
}

//////////////////////////////////////////////////////////////////////
// Product expressions

/** Representation for a product expression.  This struct could probably
    be used to represent lots of different binary expressions. */
typedef struct {
  char *(*eval)( Expr *oper, Context *ctxt );
  void (*destroy)( Expr *oper );

  // Two sub-expressions.
  Expr *leftExpr, *rightExpr;
} ProductExpr;

// Destroy function for Product expression.
static void destroyProduct( Expr *expr )
{
  ProductExpr *this = (ProductExpr *)expr;

  // Free our operand subexpressions.
  //this->leftExpr->destroy( this->leftExpr );
  //this->rightExpr->destroy( this->rightExpr );

  // Then the struct itself.
  free( this );
}


// Eval function for a Product expression.
static char *evalProduct( Expr *expr, Context *ctxt )
{
  // Get a pointer to the more specific type this function works with.
  ProductExpr *this = (ProductExpr *)expr;

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
  sprintf( result, "%f", a * b );
  return result;
}

Expr *makeProduct( Expr *leftExpr, Expr *rightExpr )
{
  // Make an instance of ProductExpr
  ProductExpr *this = (ProductExpr *) malloc( sizeof( ProductExpr ) );
  this->destroy = destroyProduct;
  this->eval = evalProduct;

  // Remember the two sub-expressions.
  this->leftExpr = leftExpr;
  this->rightExpr = rightExpr;

  // Return the instance as if it's an Expr (which it sort of is)
  return (Expr *) this;
}

//////////////////////////////////////////////////////////////////////
// Quotient expressions

/** Representation for a quotient expression.  This struct could probably
    be used to represent lots of different binary expressions. */
typedef struct {
  char *(*eval)( Expr *oper, Context *ctxt );
  void (*destroy)( Expr *oper );

  // Two sub-expressions.
  Expr *leftExpr, *rightExpr;
} QuotientExpr;

// Destroy function for Quotient expression.
static void destroyQuotient( Expr *expr )
{
  QuotientExpr *this = (QuotientExpr *)expr;

  // Free our operand subexpressions.
  this->leftExpr->destroy( this->leftExpr );
  this->rightExpr->destroy( this->rightExpr );

  // Then the struct itself.
  free( this );
}


// Eval function for a Quotient expression.
static char *evalQuotient( Expr *expr, Context *ctxt )
{
  // Get a pointer to the more specific type this function works with.
  QuotientExpr *this = (QuotientExpr *)expr;

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
  //free( left );
  //free( right );

  // Compute the result, store it in a dynamically allocated string
  // and return it to the caller.
  char *result = (char *)malloc( MAX_NUMBER + 1 );
  
  // If both a and b are zero, we need to return the "not-a-number" value.
  if ( a == 0 && b == 0 ) {

    strcpy( result, "not-a-number" );
    return result;
  }
  
  // If b is zero, and a is some other integer, we need to return the "inf" value.
  if ( a != 0 && b == 0 ) {

    strcpy( result, "inf" );
    return result;
  }
  
  // Otherwise, do the division and return the result.
  sprintf( result, "%f", a / b );
  return result;
}

Expr *makeQuotient( Expr *leftExpr, Expr *rightExpr )
{
  // Make an instance of QuotientExpr
  QuotientExpr *this = (QuotientExpr *) malloc( sizeof( QuotientExpr ) );
  this->destroy = destroyQuotient;
  this->eval = evalQuotient;

  // Remember the two sub-expressions.
  this->leftExpr = leftExpr;
  this->rightExpr = rightExpr;

  // Return the instance as if it's an Expr (which it sort of is)
  return (Expr *) this;
}

//////////////////////////////////////////////////////////////////////
// Equals expressions

/** Representation for an equals expression.  This struct could probably
    be used to represent lots of different binary expressions. */
typedef struct {
  char *(*eval)( Expr *oper, Context *ctxt );
  void (*destroy)( Expr *oper );

  // Two sub-expressions.
  Expr *leftExpr, *rightExpr;
} EqualsExpr;

// Destroy function for Equals expression.
static void destroyEquals( Expr *expr )
{
  EqualsExpr *this = (EqualsExpr *)expr;

  // Free our operand subexpressions.
  this->leftExpr->destroy( this->leftExpr );
  this->rightExpr->destroy( this->rightExpr );

  // Then the struct itself.
  free( this );
}

// Eval function for an Equals expression.
static char *evalEquals( Expr *expr, Context *ctxt )
{
  // Get a pointer to the more specific type this function works with.
  EqualsExpr *this = (EqualsExpr *)expr;

  // Evaluate our two operands
  char *left = this->leftExpr->eval( this->leftExpr, ctxt );
  char *right = this->rightExpr->eval( this->rightExpr, ctxt );

  // Compute the result, store it in a dynamically allocated string.
  char *result = (char *)malloc( MAX_NUMBER + 1 );
  
  if ( strcmp( left, right ) == 0 )
      strcpy( result, "t" );
  else
      strcpy( result, "" );

  // We're done with the values returned by our two subexpressions,
  // We just needed to get them as doubles
  free( left );
  free( right );

  // Return result to the caller.
  return result;
}

Expr *makeEquals( Expr *leftExpr, Expr *rightExpr )
{
  // Make an instance of EqualsExpr
  EqualsExpr *this = (EqualsExpr *) malloc( sizeof( EqualsExpr ) );
  this->destroy = destroyEquals;
  this->eval = evalEquals;

  // Remember the two sub-expressions.
  this->leftExpr = leftExpr;
  this->rightExpr = rightExpr;

  // Return the instance as if it's an Expr (which it sort of is)
  return (Expr *) this;
}

//////////////////////////////////////////////////////////////////////
// Less expressions

/** Representation for a less expression.  This struct could probably
    be used to represent lots of different binary expressions. */
typedef struct {
  char *(*eval)( Expr *oper, Context *ctxt );
  void (*destroy)( Expr *oper );

  // Two sub-expressions.
  Expr *leftExpr, *rightExpr;
} LessExpr;

// Destroy function for Less expression.
static void destroyLess( Expr *expr )
{
  LessExpr *this = (LessExpr *)expr;

  // Free our operand subexpressions.
  this->leftExpr->destroy( this->leftExpr );
  this->rightExpr->destroy( this->rightExpr );

  // Then the struct itself.
  free( this );
}

// Eval function for a Less expression.
static char *evalLess( Expr *expr, Context *ctxt )
{
  // Get a pointer to the more specific type this function works with.
  LessExpr *this = (LessExpr *)expr;

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
  
  // If a is less than b, result is the true string.
  // Otherwise, result is the empty string.
  if ( a < b )
    strcpy( result, "t" );
  else
    strcpy( result, "" );

  // Return the result.
  return result;
}

Expr *makeLess( Expr *leftExpr, Expr *rightExpr )
{
  // Make an instance of LessExpr
  LessExpr *this = (LessExpr *) malloc( sizeof( LessExpr ) );
  this->destroy = destroyLess;
  this->eval = evalLess;

  // Remember the two sub-expressions.
  this->leftExpr = leftExpr;
  this->rightExpr = rightExpr;

  // Return the instance as if it's an Expr (which it sort of is)
  return (Expr *) this;
}

//////////////////////////////////////////////////////////////////////
// And expressions

/** Representation for an and expression.  This struct could probably
    be used to represent lots of different binary expressions. */
typedef struct {
  char *(*eval)( Expr *oper, Context *ctxt );
  void (*destroy)( Expr *oper );

  // Two sub-expressions.
  Expr *leftExpr, *rightExpr;
} AndExpr;

// Destroy function for And expression.
static void destroyAnd( Expr *expr )
{
  AndExpr *this = (AndExpr *)expr;

  // Free our operand subexpressions.
  this->leftExpr->destroy( this->leftExpr );
  this->rightExpr->destroy( this->rightExpr );

  // Then the struct itself.
  free( this );
}

// Eval function for an And expression.
static char *evalAnd( Expr *expr, Context *ctxt )
{
  // Get a pointer to the more specific type this function works with.
  AndExpr *this = (AndExpr *)expr;

  // Evaluate our two operands
  char *left = this->leftExpr->eval( this->leftExpr, ctxt );
  char *right = this->rightExpr->eval( this->rightExpr, ctxt );

  // Allocate the result
  char *result = (char *)malloc( MAX_NUMBER + 1 );
  
  // Check if they're both not equal to the false empty string.
  if ( strcmp( left, "" ) != 0 && strcmp( right, "" ) != 0 )
    strcpy( result, "t" );
  else
    strcpy( result, "" );

  // We're done with the values returned by our two subexpressions.
  free( left );
  free( right );
  
  // Return the result.
  return result;
}

Expr *makeAnd( Expr *leftExpr, Expr *rightExpr )
{
  // Make an instance of AndExpr
  AndExpr *this = (AndExpr *) malloc( sizeof( AndExpr ) );
  this->destroy = destroyAnd;
  this->eval = evalAnd;

  // Remember the two sub-expressions.
  this->leftExpr = leftExpr;
  this->rightExpr = rightExpr;

  // Return the instance as if it's an Expr (which it sort of is)
  return (Expr *) this;
}

//////////////////////////////////////////////////////////////////////
// Or expressions

/** Representation for an or expression.  This struct could probably
    be used to represent lots of different binary expressions. */
typedef struct {
  char *(*eval)( Expr *oper, Context *ctxt );
  void (*destroy)( Expr *oper );

  // Two sub-expressions.
  Expr *leftExpr, *rightExpr;
} OrExpr;

// Destroy function for Or expression.
static void destroyOr( Expr *expr )
{
  OrExpr *this = (OrExpr *)expr;

  // Free our operand subexpressions.
  this->leftExpr->destroy( this->leftExpr );
  this->rightExpr->destroy( this->rightExpr );

  // Then the struct itself.
  free( this );
}

// Eval function for an Or expression.
static char *evalOr( Expr *expr, Context *ctxt )
{
  // Get a pointer to the more specific type this function works with.
  OrExpr *this = (OrExpr *)expr;

  // Evaluate our two operands
  char *left = this->leftExpr->eval( this->leftExpr, ctxt );
  char *right = this->rightExpr->eval( this->rightExpr, ctxt );

  // Allocate the result
  char *result = (char *)malloc( MAX_NUMBER + 1 );
  
  // Check if eithe of them are not equal to the false empty string.
  if ( strcmp( left, "" ) != 0 || strcmp( right, "" ) != 0 )
    strcpy( result, "t" );
  else
    strcpy( result, "" );

  // We're done with the values returned by our two subexpressions.
  free( left );
  free( right );
  
  // Return the result.
  return result;
}

Expr *makeOr( Expr *leftExpr, Expr *rightExpr )
{
  // Make an instance of OrExpr
  OrExpr *this = (OrExpr *) malloc( sizeof( OrExpr ) );
  this->destroy = destroyOr;
  this->eval = evalOr;

  // Remember the two sub-expressions.
  this->leftExpr = leftExpr;
  this->rightExpr = rightExpr;

  // Return the instance as if it's an Expr (which it sort of is)
  return (Expr *) this;
}
