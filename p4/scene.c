/**
    @file scene.c
    @author Nick Board (ndboard)

    This program creates scenes as requested, frees them,
    and passes on transformations.
*/

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include"model.h"
#include"scene.h"

/** The initial capacity of a scene's model list. */
#define SCENE_CAPACITY 100;

/**
  Creates a scene, putting space aside for the model list.
*/
Scene *makeScene()
{
  //Create a Scene* to a block of dynamically allocated memory of size Scene.
  Scene *newScene = ( Scene * )( malloc ( sizeof ( Scene ) ) );
  
  //Set the initial model count and the model cap.
  ( newScene -> mCount ) = 0;
  ( newScene -> mCap ) = SCENE_CAPACITY;
  
  //Create a Model** list to a block of dynamically allocated memory of size
  //mCap times Model*.
  ( newScene -> mList ) = ( Model ** )( malloc ( ( newScene -> mCap) * sizeof( Model * ) ) );
  
  //Return the pointer to the newScene.
  return newScene;
}

/**
  Frees a scene's allocated space.
  
  @param s The scene pointer to free.
*/
void freeScene ( Scene *s )
{
  //Go through list of models, freeing each
  for ( int i = 0; i < ( s -> mCount ); i++ ) {

    free( ( s -> mList )[i] );
  }
  
  //Free the list itself
  free( s -> mList );
  
  //Finaly, free the scene
  free( s );
}

/**
    Applies a transformation to a given scene.
    
    @param s The scene pointer to apply the transformation to.
    @param name The name of the model to apply the transformation to.
    
    @return true or false if the transformation was applied.
*/
bool applyToScene( Scene *s, char const *name,
void (*f)( double pt[ 2 ], double a, double b ), double a, double b )
{

  return false;
}

/**
  Sorts the models contained in a scene's mList.
  
  @param s The scene to sort the models in.
*/
void sortModels( Scene *s )
{

  Model *temp;
  
  for ( int i = 1; i < ( s -> mCount ); i++ ) {

    for ( int j = 1; j < ( s -> mCount ); j++ ) {

      if ( strcmp( ( ( s -> mList )[j] -> name ), ( ( s -> mList )[j - 1] -> name ) ) < 0) {

        temp = ( s -> mList )[j - 1];
        ( s -> mList )[j - 1] = ( s -> mList )[j];
        ( s -> mList )[j] = temp;
        
      }
    }
  }
}
