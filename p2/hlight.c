/**
  @file hlight.c
  @file Nicholas Board (ndboard)

  This program takes in HTML script and will
  output it will all tags colored red and all
  entities colored blue.
*/

#include<stdio.h>
#include<stdlib.h>

// Define the color escape codes and exit numbers.
#define RED "\x1B[31m"
#define BLUE "\x1B[34m"
#define BLACK "\x1B[0m"
#define UNSUCCESSFUL_TAG_EXIT 101
#define UNSUCCESSFUL_ENTITY_EXIT 100

// Prototyping functions used.
void showTag();
void showEntity();

/**
  Reads the input HTML file one character
  at a time and check for the beginning of tags
  and entities.

  @return program exit status
*/
int main ()
{

  // The current character
  int c;

  // Continues to go through, checking for tags and entities until EOF
  while ( ( c = getchar() ) != EOF ){
    if ( c == '<' )
    {
      showTag();
    }

    else if (c == '&' )
    {
      showEntity();
    }
    else {
      putchar( c );
    }
  }

  // Exits the program successfully.
  return 0;
}

/**
  Colors tags red.
*/
void showTag()
{
  // The current character.
  int c;

  // Starts the red text.
  printf( "%s", RED );

  // Need to manually place another '<'.
  putchar( '<' );

  // Parses through the rest of the tag until '>'
  while ( ( c = getchar() ) != '>' ) {

    // If the EOF is reached before the end of the tag, error is reported.  
    if ( c == -1 ) {
       printf( "%s", BLACK );
       exit( UNSUCCESSFUL_TAG_EXIT );
    }

    putchar( c );
  }

  // Needs to mannually place another '>'.
  putchar( '>' );

  // Changes color back to black.
  printf( "%s", BLACK );
}

/**
  Colors entities blue.
*/
void showEntity()
{
  // The current character.
  int c;

  // Starts the blue text.
  printf( "%s", BLUE );

  // Need to manually place another '&'.
  putchar( '&' );

  // Parses through the rest of the tag until ';'
  while ( ( c = getchar() ) != ';' ) {

    // If the EOF is reached before the end of the entity, error is reported. 
    if ( c == -1 ) {
       printf( "%s", BLACK );
       exit( UNSUCCESSFUL_ENTITY_EXIT );
    }

    putchar( c );
  }

  // Needs to mannually place another ';'.
  putchar( ';' );

  // Changes color back to black.
  printf( "%s", BLACK );
}
