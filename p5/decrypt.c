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
  
  
  // Loop continuously.
  while ( true ){

    // Read the next valid code, if there is one.
    int returned = readBits( buffer, inputFile);

    // A code is returned.  Match it with the proper symbol
    // and write that to the output file.
    if ( returned >= 0 ) {

        fprintf( outputFile, "%c", codeToSym( returned) );
    }
    
    // EOF is reached under valid conditions.  Free allocated
    // memory, close files, and return 0;
    else if ( returned == -1 ) {

      free( buffer );
      fclose( inputFile );
      fclose( outputFile );
      return 0;
    }
    
    // EOF is reached under invalid conditions.  Print error message,
    // free allocated memory, close files, and return 0;
    else if ( returned == -2 ) {

      fprintf( stderr, "Invalid file\n" );
      free( buffer );
      fclose( inputFile );
      fclose( outputFile );
      return 1;
    }
  }
}
