/**
  @file expr.h
  @author

  Support for variable storage and expression syntax.
*/

#ifndef _EXPR_H_
#define _EXPR_H_

#include <stdio.h>
#include <stdbool.h>

//////////////////////////////////////////////////////////////////////
// Context

// Maximum length of an identifier (variable) name.
#define MAX_IDENT_LEN 20

/**
   Short typename for the Context structure.  It's definition is an
   implementation detail of the language, not visible to client code.
*/
typedef struct ContextTag Context;


/** Create and return a new, empty context object.
    @return new context object.  The caller must eventually free this
    with freeContext().
*/
Context *makeContext();


/** Return the value of the variable with the given name.  If the
    variable isn't defined, this function returns the empty string.
    @param ctxt context object in which to lookup the variable name.
    @param new value for the variable name.
    @return the variable's value.  This is a pointer into the
    context's representation and should not be directly freed or
    modified by the caller.
*/
char const *getVariable( Context *ctxt, char const *name );

/** In the given context, set the named variable to store the given
    value.  

    Finish commenting this function based on how your context works.

    @param ctxt context in which to store the variable name / value.
    @param name of the variable to set the value for.
    @param value new value for this variable.
*/
void setVariable( Context *ctxt, char const *name, char *value );

/** Free all the memory associated with this context.
    @param ctxt context to free memory for.
*/
void freeContext( Context *ctxt );

//////////////////////////////////////////////////////////////////////
// Expr

/** A short name to use for the expression interface. */
typedef struct ExprTag Expr;

/** Representation for an Expr interface.  Classes implementing this
    have these two fields as their first members.  They will set eval
    to point to appropriate functions to evaluate the type of
    expression their class represents, and they will set destroy to
    point to a function that frees memory for their type of expresson.
*/
struct ExprTag {
  /** Pointer to a function to evaluate the given expression and
      return the result as a dynamically allocated string.
      @param expr expression to be evaluated.
      @param ctxt current values of all variables.
      @return string respresentation of the result. The caller is responsible
      for freeing this.
   */
  char *(*eval)( Expr *expr, Context *ctxt );

  /** Free memory for this expression, including any subexpressions
      it contains.
      @param expr expression to free.
  */
  void (*destroy)( Expr *expr );
};

/** Make a literal expressin that evaluates to the given string.
    @param val value this expression evaluates to.  This should be a
    dynamically allocated string that the expression will be
    responsible for freeing.
    @return a new expression that evaluates to a copy of the given value.
 */
Expr *makeLiteral( char *val );

/** Make a variable expression that evaulates to the name of that variable.
    This should be a dynamically allocated string that the expression
    will be responsible for freeing.
    @param val value this expression evaluates to.
    @return a new expression that evaluates to a copy of a given value.
*/
Expr *makeVar( char const *name );

/** Make an expression that adds up the value of its two sub-expressions.
    This expression object will take ownership of the memory pointed to
    by its two sub-expressions and free them when it is destroyed.
    @param leftExpr left-hand expression to add.
    @param rightExpr right-hand expression to add.
    @return pointer to a new, dynamically allocated subclass of Expr.
 */
Expr *makeSum( Expr *leftExpr, Expr *rightExpr );

/** Make an expression that finds the difference of its two sub-expressions.
    This expression object will take ownership of the memory pointed to
    by its two sub-expressions and free them when it is destroyed.
    @param leftExpr left-hand expression to add.
    @param rightExpr right-hand expression to add.
    @return pointer to a new, dynamically allocated subclass of Expr.
 */
Expr *makeDifference( Expr *leftExpr, Expr *rightExpr );

/** Make an expression that finds the product of its two sub-expressions.
    This expression object will take ownership of the memory pointed to
    by its two sub-expressions and free them when it is destroyed.
    @param leftExpr left-hand expression to add.
    @param rightExpr right-hand expression to add.
    @return pointer to a new, dynamically allocated subclass of Expr.
 */
Expr *makeProduct( Expr *leftExpr, Expr *rightExpr );

/** Make an expression that finds the quotient of its two sub-expressions.
    This expression object will take ownership of the memory pointed to
    by its two sub-expressions and free them when it is destroyed.
    @param leftExpr left-hand expression to add.
    @param rightExpr right-hand expression to add.
    @return pointer to a new, dynamically allocated subclass of Expr.
 */
Expr *makeQuotient( Expr *leftExpr, Expr *rightExpr );

/** Make an expression that determines a truth-value depending on the
    truth values of its two sub-expressions.
    @param leftExpr left-hand expression to judge.
    @param rightExpr right-hand expression to judge.
    @return pointer to a new, dynamically allocated subclass of Expr.
*/
Expr *makeAnd( Expr *leftExpr, Expr *rightExpr );

/** Make an expression that determines a truth-value depending on the
    truth values of its two sub-expressions.
    @param leftExpr left-hand expression to judge.
    @param rightExpr right-hand expression to judge.
    @return pointer to a new, dynamically allocated subclass of Expr.
*/
Expr *makeOr( Expr *leftExpr, Expr *rightExpr );

/** Make an expression that determines the equality of its two sub-expressions.
    @param leftExpr left-hand expression to compare
    @param rightExpr right-hand expression to compare
    @return pointer to a new, dynamically allocated subclass of Expr.
*/
Expr *makeEquals( Expr *leftExpr, Expr *rightExpr );

/** Make an expression that determines which of the two sub-expressions is smaller.
    @param leftExpr left-hand expression to compare.
    @param rightExpr right-hand expression to compare.
    @return pointer to a new, dynamically allocated subclass of Expr.
*/
Expr *makeLess( Expr *leftExpr, Expr *rightExpr );

#endif
