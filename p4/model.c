#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include"model.h"

Model *loadModel( char const *fname )
{
  int capacity = 10;
  
  Model *newModel = ( Model * )( malloc ( sizeof( Model ) ) );
  ( newModel -> pList ) = ( double(*)[2] )( malloc( 2 * capacity * sizeof( double * ) ) );
  ( newModel -> pCount ) = 0;
  
  FILE *mounted = fopen( fname, "r" );
  
  if( mounted == NULL ) {
    
    return NULL;
  }
  
  else {
    
    double p1x = 0.0;  
    double p1y = 0.0; 
    double p2x = 0.0;
    double p2y = 0.0;
    
    while ( fscanf( mounted, "%lf %lf %lf %lf", &p1x, &p1y, &p2x, &p2y ) == 4 ) {
      
      if ( ( newModel -> pCount) >= capacity ) {
        capacity *= 2;
        ( newModel -> pList ) = (double(*)[2]) realloc( (newModel -> pList), 2 * capacity * sizeof( double * ) );
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