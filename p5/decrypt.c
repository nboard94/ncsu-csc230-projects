#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "bits.h"
#include "codes.h"

#define NUM_ARGS 3

/** Where the decrypt program begins execution.
    Takes two command-line arguments, specifying
    the input and output files.
*/
int main ( int argc, char *argv[] )
{

  // Check for the correct number of command line arguments.
  // If there is anything other than three, print the
  // error message and return 1.
  if ( argc != NUM_ARGS ) {

    fprintf( stderr, "usage: decrypt <infile> <outfile>\n" );
    return 1;
  }
  
  // Open the input file with reading permissions.
  FILE *inputFile = fopen( argv[1], "rb" );
  // Open the output file with binary writing permissions.
  FILE *outputFile = fopen( argv[2], "w" );
  // Declare the BitBuffer used while encrypting.
  BitBuffer *buffer = (BitBuffer *)malloc( sizeof( BitBuffer ) );
  ( buffer -> bcount ) = 0;
  // Code retreived from the input file.
  int code;
  // A character looked up using a code.
  char currentChar;
  
  // Check to make sure the input file exists.
  // If it points to NULL, print the error message and return 1.
  if ( inputFile == NULL ) {

    perror( argv[1] );
    return 1;
  }
  
  // Loop continuously, readBits will let us know when
  // EOF has been found.
  while ( true ){

    // Retrieve a valid code or an EOF flag.
    code = readBits( buffer, inputFile );
    
    // If code is -1, we reached EOF with valid input, return 0;
    if ( code == -1 ) {

      return 0;
    }

    // If code is -2, we reached EOF with invalid input.
    // Print message to standard error and return 1.
    if ( code == -2 ) {

      fprintf( stderr, "Invalid file\n");
      return 1;
    }
    
    // If there is a positive value in code, we need to find the corresponding
    // character and print it to the output file.
    if ( code > 0 ) {

      currentChar = codeToSym( code );
      fprintf( outputFile, "%c", currentChar );
    }
  }
}
