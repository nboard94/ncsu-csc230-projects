/** 
    @file scene.h
    @author Nick Board (ndboard)

    This head contains the functions in scene.c and
    the scene struct used throughout.
*/

#ifndef _SCENE_H_
#define _SCENE_H_

#include "model.h"
#include <stdbool.h>

/** Representation for a whole scene, a collection of models. */
typedef struct {
  /** Number of models in the scene. */
  int mCount;
  
  /** Capacity of the model list. */
  int mCap;

  /** List of pointers to models. */
  Model **mList;
} Scene;

#endif

/**
  Creates a scene, putting space aside for the model list.
*/
Scene *makeScene();

/**
  Frees a scene's allocated space.
  
  @param s The scene pointer to free.
*/
void freeScene ( Scene *s );

/**
    Applies a transformation to a given scene.
    
    @param s The scene pointer to apply the transformation to.
    @param name The name of the model to apply the transformation to.
    
    @return true or false if the transformation was applied.
*/
bool applyToScene( Scene *s, char const *name,
void (*f)( double pt[ 2 ], double a, double b ), double a, double b );

/**
  Sorts the models contained in a scene's mList.
  
  @param s The scene to sort the models in.
*/
void sortModels ( Scene *s );
