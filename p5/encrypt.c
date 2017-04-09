#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "bits.h"
#include "codes.h"
 
/** Where the encrypt program begins execution.
	Takes two command-line arguments, specifying 
	the input and output files. 
*/
int main ( int argc, char *argv[] )
{

	// Check for the correct number of command line arguments.
	// If there is anything other than three, print the
	// error message and return 1.
	if( argc != 3 ) {

		fprintf( stderr, "usage: encrypt <infile> <outfile>\n" );
		return 1;
	}

	// Open the input file with reading permissions.
	FILE *inputFile = fopen( argv[1], "r" );
	// Open the output file with binary writing permissions.
	FILE *outputFile = fopen( argv[2], "wb" );
	// Declare the BitBuffer used while encrypting.
	BitBuffer *buffer = (BitBuffer *)malloc( sizeof( BitBuffer ) );
	
	// Initialize fields in the buffer.
	( buffer -> bits ) = 0;
	( buffer -> bcount ) = 0;
	
	// Check to make sure the input file exists.
	// If it points to NULL, print the error message and return 1.
	if ( inputFile == NULL ) {

		fprintf( stderr, "Invalid file\n");
		return 1;
	}  

	// The current character being worked on.
	unsigned char *currentChar = (unsigned char *)malloc( sizeof( unsigned char ) );
	// The code for the current character.
	int currentCode = 0;
	// The number of bits needed for the currentCode.
	int currentSize = 0;

	// Loop continuously.
	while( true ) {
		
		// If EOF has been reached, flush remaining bits.
		// Close both files, free memory, and return 0;
		if( feof( inputFile ) ) {
			
			flushBits( buffer, outputFile );
			free( buffer );
			free( currentChar );
			fclose ( inputFile );
			fclose ( outputFile );
			return 0;
		}
		
		// If one char was successfully read, retrieve the corresponding code
		// and number of bits, and then call fwrite.
		if ( fread( currentChar, sizeof(unsigned char), 1, inputFile ) == 1 ) {
		
			// Check to make sure currentChar is valid.
			// If not, print error message and return 1.
			if( !( ( *currentChar >= 'A' && *currentChar <= 'Z' ) 
				|| *currentChar == ' ' || *currentChar == '\n' ) ) {
			
				fprintf( stderr, "Invalid file\n" );
				return 1;
			}
		
			currentCode = symToCode( *currentChar );
			currentSize = bitsInCode( *currentChar );
			
			writeBits( currentCode, currentSize, buffer, outputFile );
		}
	}

	// If EOF has been reached, flush remaining bits.
	// Close both files, free memory, and return 0;
	flushBits( buffer, outputFile );
	free( buffer );
	free( currentChar );
	fclose ( inputFile );
	fclose ( outputFile );
	return 0;
 }
