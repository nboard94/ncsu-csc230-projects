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
  //
  
  int code;
  char currentChar;
  
  // Loop continuously.
  while ( true ){

    // Read some bits.
    code = readBits( buffer, inputFile );
    
    // If code is -1, we reached EOF and it's time to stop.
    if( code == -1 ) {
      
      return 0;
    }

    // If code is -2, there is an error with input and we need to stop.
    if( code == -2 ) {
      
      fprintf( stderr, "Invalid input\n");
      return 1;
    } 
    
    // If we've made it this far, get the character that the code represents,
    // and print that character to the outputFile.
    currentChar = codeToSym( code );
    fprintf( outputFile, "%c", currentChar );
  }
}
