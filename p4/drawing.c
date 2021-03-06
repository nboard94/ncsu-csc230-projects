/**
    @file drawing.c
    @author Nick Board (ndboard)

    This program allows the user to manipulate models from text
    files and saves the output in a form that gnuplot can use.
*/

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<math.h>

#include"model.h"
#include"scene.h"

/** The max size for a line buffer. */
#define LINE_SIZE 101

/** The max size for an arg buffer. */
#define ARG_BUFFER 50

/** The maz size for an appropriate arg or command. */
#define MAX_ARGS 20

/** 180 degrees used converting from degrees to rads. */
#define SEMI_CIRCLE 180

/** Default size for number of models in a scene. */
#define SCENE_SIZE 100

/** The number three. */
#define THREE 3

//Prototype functions used later in the program.
void invalidInput( int );
void containsEOF( char* );

/**
    Where the program begins.  Prompts the user for input and then
    deals with the user input.

    @return program exit status
 */
int main()
{
  Scene *scene = makeScene();
  ( scene -> mCap ) = SCENE_SIZE;

  int promptCount = 1;
  char command[ARG_BUFFER];
  bool invalidCommand;

  while ( true ) {

    invalidCommand = true;
    
    while ( invalidCommand ) {

      printf( "cmd %d> ", promptCount );
      
      if ( scanf( "%s", command ) != 1 ) {

        if ( getchar() == EOF ) {

          return 0;
        }
      }
      
      if ( strlen ( command ) > MAX_ARGS ) {

        while ( getchar() != ' ' );
        invalidInput( promptCount );
      }
      else {

        invalidCommand = false;
      }
    }
    
    containsEOF( command );

    //USER SELECTS TO LOAD FROM A FILE.
    if ( strcmp ( "load", command ) == 0 ) {
    
      char line[LINE_SIZE];
      char arg1[ARG_BUFFER];
      char arg2[ARG_BUFFER];
      bool doop = false;

      fgets( line, LINE_SIZE, stdin );
      sscanf( line, "%s %s", arg1, arg2 );

      if (  strlen( arg1 ) < 1 || strlen( arg2 ) < 1 ||
            strlen( arg1 ) > MAX_ARGS || strlen ( arg2 ) > MAX_ARGS ) {

        invalidInput( promptCount );
        goto SKIP;
      }
      
      if ( ( scene -> mCount ) >= ( scene -> mCap ) ) {

        ( scene -> mCap ) *= 2;
        ( scene -> mList ) = ( Model ** )
                             realloc( ( scene -> mList ), ( scene -> mCap) * sizeof( Model * )  );
      }
      
      Model *newModel = loadModel( arg2 );
      
      for ( int i = 0; i < ( scene -> mCount ); i++ ) {

        if ( strcmp( ( ( scene -> mList )[i] -> name ), arg1 ) == 0 )  {

          //printf( "name: %s, model: %s, i:%d\n ", arg1,  ( scene -> mList )[i] -> name , i );
          invalidInput( promptCount );
          doop = true;
        }
      }
      
      if ( !doop ) {
        if ( newModel == NULL ) {

          fprintf( stderr, "Can't open file: %s\n", arg2 );
        }
        else {

          strcpy( newModel->name, arg1 );
          strcpy( newModel->fname, arg2 );
          
          ( scene->mList[ ( scene -> mCount ) ] ) = newModel;
          ( scene->mCount )++;
        }
      }
      
      doop = false;
      arg1[0] = '\0';
      arg2[0] = '\0';
    }
    
    //USER SELECTS TO SAVE TO A FILE.
    else if ( strcmp( "save", command ) == 0 ) {

      char line[LINE_SIZE];
      char arg[MAX_ARGS];
      Model *current;
      double p1x;
      double p1y;
      double p2x;
      double p2y;

      fgets( line, LINE_SIZE, stdin );
      sscanf( line, "%20s", arg );
      
      if ( strlen( arg ) < 1 || strlen (arg) > MAX_ARGS ) {

        invalidInput( promptCount );
        goto SKIP;
      }
      
      FILE *mounted = fopen( arg, "w" );
      
      if ( mounted == NULL ) {

        fprintf( stderr, "Can't open file: %s\n", arg );
      }
      
      for ( int i = 0; i < ( scene -> mCount ); i++ ) {

        current = ( scene -> mList )[i];
        
        for ( int j = 0; j < ( current -> pCount ); j += 2 ) {

          p1x = ( current -> pList )[j][0];
          p1y = ( current -> pList )[j][1];
          
          p2x = ( current -> pList )[j + 1][0];
          p2y = ( current -> pList )[j + 1][1];
          
          fprintf( mounted, "%.3lf %.3lf\n%.3lf %.3lf\n\n", p1x, p1y, p2x, p2y );
          //printf( "%lf %lf\n%lf %lf\n\n", p1x, p1y, p2x, p2y );
        }
      }
      
      fclose( mounted );
    }
    
    //USER SELECTS TO DELETE A MODEL.
    else if ( strcmp( "delete", command ) == 0 ) {

      char line[LINE_SIZE];
      char arg[MAX_ARGS];
      bool found = false;
      int i = 0;

      fgets( line, LINE_SIZE, stdin );
      sscanf( line, "%20s", arg );
      
      if ( strlen( arg ) < 1 || strlen (arg) > MAX_ARGS ) {

        invalidInput( promptCount );
        goto SKIP;
      }
      
      while ( !found && i < ( scene -> mCount ) ) {

        if (strcmp( arg, ( ( scene -> mList )[i] ) -> name ) ) {

          found = true;
          
          for ( int j = i - 1; j < ( scene -> mCount ); j++ ) {

            ( scene -> mList )[j] = ( scene -> mList )[j + 1];
            
          }
          
          //freeModel( ( scene -> mList )[ scene -> mCount ] );
          ( scene -> mCount )--;
        }
        
        i++;
      }
      
      if ( !found ) {

        invalidInput( promptCount );
      }
      
    }
    
    //USER SELECTS TO LIST ALL MODELS.
    else if ( strcmp( "list", command ) == 0 ) {

      sortModels( scene );
      
      for ( int i = 0; i < ( scene -> mCount ); i++ ) {

        printf( "%s %s (%d)\n", ( ( scene -> mList )[ i ] -> name ),
                                ( ( scene -> mList )[ i ] -> fname ),
                                ( ( scene -> mList )[ i ] -> pCount ) / 2 );
      }
    }
    
    //USER SELEECTS TO TRANSLATE A MODEL.
    else if ( strcmp( "translate", command ) == 0 ) {

      char line[LINE_SIZE];
      char arg[MAX_ARGS];
      double changeX;
      double changeY;
      bool found = false;

      fgets( line, LINE_SIZE, stdin );
      if ( sscanf( line, "%20s %lf %lf", arg, &changeX, &changeY ) != THREE ) {

        invalidInput( promptCount );
      }
      
      if ( strlen( arg ) < 1 || strlen (arg) > LINE_SIZE ) {

        invalidInput( promptCount );
        goto SKIP;
      }
      
      else {

        for ( int i = 0; i < ( scene -> mCount ); i++ ) {
        
          if ( strcmp( arg, ( ( scene -> mList )[i] ) -> name ) == 0 ) {

            for ( int j = 0; j < ( ( scene -> mList )[i] -> pCount ); j++ ) {

              ( ( scene -> mList )[i] -> pList )[j][0] += changeX;
              ( ( scene -> mList )[i] -> pList )[j][1] += changeY;
            }
            
            found = true;
          }
        }
        
        if ( !found ) {
        
        invalidInput( promptCount );
      }
      }
    }
    
    //USER SELECTS TO SCALE A MODEL. (PROBLEM WITH >20 CHECK.)
    else if ( strcmp( "scale", command ) == 0 ) {

      char line[LINE_SIZE];
      char arg[MAX_ARGS];
      double factor;
      bool found = false;

      fgets( line, LINE_SIZE, stdin );
      if ( sscanf( line, "%20s %lf", arg, &factor ) != 2 ) {

        invalidInput( promptCount );
      }
      
      else {

        for ( int i = 0; i < ( scene -> mCount ); i++ ) {
        
          if ( strcmp( arg, ( ( scene -> mList )[i] ) -> name ) == 0 ) {

            for ( int j = 0; j < ( ( scene -> mList )[i] -> pCount ); j++ ) {

              ( ( scene -> mList )[i] -> pList )[j][0] *= factor;
              ( ( scene -> mList )[i] -> pList )[j][1] *= factor;
            }
            
            found = true;
          }
        }
        
        if ( !found ) {
        
        invalidInput( promptCount );
      }
      }
    }
    
    //USER SELECTS TO ROTATE A MODEL.
    else if ( strcmp( "rotate", command ) == 0 ) {

      char line[LINE_SIZE];
      char arg[MAX_ARGS];
      double degree;
      double rad;
      bool found = false;
      
      double oldX;
      double oldY;
      double newX;
      double newY;

      fgets( line, LINE_SIZE, stdin );
      if ( sscanf( line, "%20s %lf", arg, &degree ) != 2 ) {

        invalidInput( promptCount );
      }
      
      if ( strlen( arg ) < 1 || strlen (arg) > MAX_ARGS ) {

        invalidInput( promptCount );
        goto SKIP;
      }
      
      else {

        rad = ( degree * M_PI ) / SEMI_CIRCLE;
        
        for ( int i = 0; i < ( scene -> mCount ); i++ ) {
        
          if ( strcmp( arg, ( ( scene -> mList )[i] ) -> name ) == 0 ) {

            for ( int j = 0; j < ( ( scene -> mList )[i] -> pCount ); j++ ) {

              oldX = ( ( scene -> mList )[i] -> pList )[j][0];
              oldY = ( ( scene -> mList )[i] -> pList )[j][1];
              
              newX = oldX * cos( rad ) - oldY * sin( rad );
              newY = oldX * sin( rad ) + oldY * cos( rad );
              //printf( "rotate X: %lf, Y: %lf\n ", newX, newY );
              
              ( ( scene -> mList )[i] -> pList )[j][0] = newX;
              ( ( scene -> mList )[i] -> pList )[j][1] = newY;
            }
            
            found = true;
          }
        }
        
        if ( !found ) {
        
        invalidInput( promptCount );
      }
      }
    }
    
    //USER SELECTS TO QUIT THE PROGRAM.
    else if ( strcmp( "quit", command ) == 0 ) {

      return 0;
    }
    
    else {

      char line[LINE_SIZE];
      fgets( line, LINE_SIZE, stdin );
      invalidInput( promptCount );
    }
    
    SKIP: promptCount++;
  }
  
  return 0;
}

/**
    Checks a string for the EOF character.
    Exits if it is detected.
    
    @param str The string to check.
 */
void containsEOF( char *str ) {

  for (int i = 0; str[i]; i++ ) {

    if ( str[i] == EOF ) {

      exit( 0 );
    }
  }

}

/**
    Used to print the invalid input indication to
    the commandline using the current prompt number.
    
    @param promptCount The current command's number.
 */
void invalidInput( int promptCount ) {

  fprintf( stderr, "Command %d invalid\n", promptCount );
}
