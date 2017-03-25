#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include"model.h"

#define INIT_CAP 10
#define PROPER_SCAN 4

Model *loadModel( char const *fname )
{
  int capacity = INIT_CAP;
  
  Model *newModel = ( Model * )( malloc ( sizeof( Model ) ) );
  ( newModel -> pList ) = ( double(*)[2] )( malloc( 2 * capacity * sizeof( double * ) ) );
  ( newModel -> pCount ) = 0;
  
  FILE *mounted = fopen( fname, "r" );
  
  if ( mounted == NULL ) {

    return NULL;
  }
  
  else {

    double p1x;
    double p1y;
    double p2x;
    double p2y;
    
    while ( fscanf( mounted, "%lf %lf %lf %lf", &p1x, &p1y, &p2x, &p2y ) == PROPER_SCAN ) {

      if ( ( newModel -> pCount) >= capacity ) {
        capacity *= 2;
        ( newModel -> pList ) = (double(*)[2])
                                realloc( (newModel -> pList), 2 * capacity * sizeof( double * ) );
      }
      
      ( newModel -> pList )[ ( newModel -> pCount ) ][0] = p1x;
      ( newModel -> pList )[ ( newModel -> pCount ) ][1] = p1y;
      ( newModel -> pCount )++;
      
      ( newModel -> pList )[ ( newModel -> pCount ) ][0] = p2x;
      ( newModel -> pList )[ ( newModel -> pCount ) ][1] = p2y;
      ( newModel -> pCount )++;
    }
  }
  
  fclose( mounted );
  return newModel;
}

void freeModel ( Model *m )
{
  free( m -> pList );
  free( m );
}

void applyToModel( Model *m, void (*f)( double pt[ 2 ], double a, double b ), double a, double b )
{

}
