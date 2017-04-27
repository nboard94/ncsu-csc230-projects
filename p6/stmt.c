#include "stmt.h"
#include "expr.h"
#include <stdlib.h>
#include <string.h>

//////////////////////////////////////////////////////////////////////
// Print

// Representation for a print statement, derived from Stmt.
typedef struct {
  void (*execute)( Stmt *stmt, Context *ctxt );
  void (*destroy)( Stmt *stmt );

  /** Argument expression we're supposed to evaluate and print. */
  Expr *arg;
} PrintStmt;

// Function to execute a print statement.
static void executePrint( Stmt *stmt, Context *ctxt )
{
  // Cast the this pointer to a more specific type.
  PrintStmt *this = (PrintStmt *)stmt;

  // Evaluate our argument, print the result, then free it.
  char *result = this->arg->eval( this->arg, ctxt );
  printf( "%s", result );
  free( result );
}

// Function to free a print statement.
static void destroyPrint( Stmt *stmt )
{
  // Cast the this pointer to a more specific type.
  PrintStmt *this = (PrintStmt *)stmt;

  // Free our subexpression then the print statement itself.
  this->arg->destroy( this->arg );
  free( this );
}

Stmt *makePrint( Expr *arg )
{
  // Allocate space for the PrintStmt object
  PrintStmt *this = (PrintStmt *) malloc( sizeof( PrintStmt ) );

  // Remember our virutal functions.
  this->execute = executePrint;
  this->destroy = destroyPrint;

  // Remember our argument subexpression.
  this->arg = arg;

  // Return the result, as an instance of the Stmt interface.
  return (Stmt *) this;
}

//////////////////////////////////////////////////////////////////////
// Compound

// Representation for a compound statement, derived from Stmt.
typedef struct {
  void (*execute)( Stmt *stmt, Context *ctxt );
  void (*destroy)( Stmt *stmt );

  /** List of statements in the compound. */
  Stmt **stmtList;

  /** Number of statements in the compound. */
  int len;
} CompoundStmt;

// Function to evaluate a compound statement.
static void executeCompound( Stmt *stmt, Context *ctxt )
{
  // Cast the this pointer to a more specific type.
  CompoundStmt *this = (CompoundStmt *)stmt;

  // Execute the sequence of statements in this compound
  for ( int i = 0; i < this->len; i++ )
    this->stmtList[ i ]->execute( this->stmtList[ i ], ctxt );
}

// Function to free a compound statement.
static void destroyCompound( Stmt *stmt )
{
  // Cast the this pointer to a more specific type.
  CompoundStmt *this = (CompoundStmt *)stmt;

  // Free the list of statements inside this compond.
  for ( int i = 0; i < this->len; i++ )
    this->stmtList[ i ]->destroy( this->stmtList[ i ] );

  // Then, free the compound statement itself.
  free( this->stmtList );
  free( this );
}

Stmt *makeCompound( Stmt **stmtList, int len )
{
  // Allocate space for the CompoundStmt object
  CompoundStmt *this = (CompoundStmt *) malloc( sizeof( CompoundStmt ) );

  // Remember our virutal functions.
  this->execute = executeCompound;
  this->destroy = destroyCompound;

  // Remember the list of statements in the compound.
  this->stmtList = stmtList;
  this->len = len;

  // Return the result, as an instance of the Stmt interface.
  return (Stmt *) this;
}

//////////////////////////////////////////////////////////////////////
// Assignment

// Representation for an assignment statement, derived from Stmt.
typedef struct {
  void (*execute)( Stmt *stmt, Context *ctxt );
  void (*destroy)( Stmt *stmt );

  /** Name of the variable. */
  //char *vname;
  char vname[MAX_IDENT_LEN + 1];

  /** Expression that the variable will evaluate to. */
  Expr *expr;
} AssignmentStmt;

// Function to evaluate an assignment statement.
static void executeAssignment( Stmt *stmt, Context *ctxt )
{
  // Cast the this pointer to a more specific type.
  AssignmentStmt *this = (AssignmentStmt *)stmt;

  // Evaluate the expression
  //char *result = (char *) malloc( sizeof(MAX_IDENT_LEN + 1) );
  //result = ( this->expr ) -> eval( this->expr, ctxt );
  
  // Store the result in context.
  setVariable( ctxt, this->vname, this->expr->eval(this->expr, ctxt) );
}

// Function to free an assignment statement.
static void destroyAssignment( Stmt *stmt )
{
  // Cast the this pointer to a more specific type.
  AssignmentStmt *this = (AssignmentStmt *)stmt;

  this->expr->destroy(this->expr);
  free( this );
}

Stmt *makeAssignment( char const *vname, Expr *expr )
{

  // Allocate space for the AssignmentStmt object
  AssignmentStmt *this = (AssignmentStmt *) malloc( sizeof( AssignmentStmt ) );

  // Remember our virutal functions.
  this->execute = executeAssignment;
  this->destroy = destroyAssignment;
  //this->vname = (char *) malloc( sizeof(MAX_IDENT_LEN + 1 ) );
  //this->expr = (Expr *) malloc( sizeof(Expr) );

  // Remember the vname and expression in the assignment.
  strcpy( this->vname, vname );
  
  this->expr = expr;
  
  // Return the result, as an instance of the Stmt interface.
  return (Stmt *) this;
}

//////////////////////////////////////////////////////////////////////
// If

// Representation for an if statement, derived from Stmt.
typedef struct {
  void (*execute)( Stmt *stmt, Context *ctxt );
  void (*destroy)( Stmt *stmt );

  /** Condition to evaluate. */
  Expr *cond;

  /** Body to execute. */
  Stmt *body;
} IfStmt;

// Function to evaluate an if statement.
static void executeIf( Stmt *stmt, Context *ctxt )
{
  // Cast the this pointer to a more specific type.
  IfStmt *this = (IfStmt *)stmt;

  // Evaluate the the truth value of the expression.
  //char *result = (char *) malloc( sizeof(MAX_IDENT_LEN + 1) );
  //result = ( this->cond )->eval( this->cond, ctxt );
  
  if ( strcmp( ( this->cond )->eval( this->cond, ctxt ), "" ) != 0 )
    this->body->execute( this->body, ctxt );

  //free( result );
}

// Function to free an if statement.
static void destroyIf( Stmt *stmt )
{
  // Cast the this pointer to a more specific type.
  IfStmt *this = (IfStmt *)stmt;

  // Free the condition and body, and then the if itself.
  this->cond->destroy( this->cond );
  this->body->destroy( this->body );
  free( this );
}

Stmt *makeIf( Expr *cond, Stmt *body )
{

  // Allocate space for the IfStmt object
  IfStmt *this = (IfStmt *) malloc( sizeof( IfStmt ) );

  // Remember our virutal functions.
  this->execute = executeIf;
  this->destroy = destroyIf;

  // Remember the condition and body.
  this->cond = cond;
  this->body = body;

  // Return the result, as an instance of the Stmt interface.
  return (Stmt *) this;
}

//////////////////////////////////////////////////////////////////////
// While

// Representation for a while statement, derived from Stmt.
typedef struct {
  void (*execute)( Stmt *stmt, Context *ctxt );
  void (*destroy)( Stmt *stmt );

  /** Condition to evaluate. */
  Expr *cond;

  /** Body to execute. */
  Stmt *body;
} WhileStmt;

// Function to evaluate a while statement.
static void executeWhile( Stmt *stmt, Context *ctxt )
{
  // Cast the this pointer to a more specific type.
  WhileStmt *this = (WhileStmt *)stmt;

  // Evaluate the the truth value of the expression.
  //char *result = (char *) malloc( sizeof(MAX_IDENT_LEN + 1) );
  
  
  while ( strcmp( ( this->cond )->eval( this->cond, ctxt ), "" ) != 0 ) {

    this->body->execute( this->body, ctxt );
  }
  
  //free( result );
}

// Function to free a while statement.
static void destroyWhile( Stmt *stmt )
{
  // Cast the this pointer to a more specific type.
  WhileStmt *this = (WhileStmt *)stmt;

  // Free the condition and body, and then the while itself.
  this->cond->destroy( this->cond );
  this->body->destroy( this->body );
  free( this );
}

Stmt *makeWhile( Expr *cond, Stmt *body )
{
  // Allocate space for the WhileStmt object
  WhileStmt *this = (WhileStmt *) malloc( sizeof( WhileStmt ) );

  // Remember our virutal functions.
  this->execute = executeWhile;
  this->destroy = destroyWhile;

  // Remember the condition and body.
  this->cond = cond;
  this->body = body;

  // Return the result, as an instance of the Stmt interface.
  return (Stmt *) this;
}
