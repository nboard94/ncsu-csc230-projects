/**
    @file model.h
    @author Nick Board (ndboard)

    This head contains the functions in model.c and
    the model struct used throughout.
*/

#ifndef _MODEL_H_
#define _MODEL_H_

#include <stdio.h>

/** Maximum length of name and filename strings. */
#define NAME_LIMIT 20

/** Representation for a model, a collection of line segments. */
typedef struct {
  /** Name of the model. */
  char name[ NAME_LIMIT + 1 ];
  
  /** File name it was loaded from. */
  char fname[ NAME_LIMIT + 1 ];
  
  /** Number of points in the model.  It has half this many line segments. */
  int pCount;

  /** List of points in the model, twice as long as the number
      of segments, since each segment has two points */
  double (*pList)[ 2 ];
} Model;

#endif

/**
    Creates a model by reading from the given filename.
    
    @param fname Name of the file to read a model from.
*/
Model *loadModel( char const *fname );

/**
    Frees a model's allocated space.
    
    @param m The model pointer to free.
*/
void freeModel ( Model *m );

/**
  Applies a transformation to a model.
  
  @param m The model pointer to apply the transformation at.
  @param f The function pointer that applies that transformation.
  @param a Used in the transformation.
  @param b Used in the transformation.
*/
void applyToModel( Model *m, void (*f)( double pt[ 2 ], double a, double b ), double a, double b );
