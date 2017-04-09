#include <stdio.h>
#include <stdlib.h>
#include "codes.h"
 
 /**
	Description copied from project website.
	Given the ASCII code for a character, this function returns the
	code used to represent it.  For example, if you call symToCode( 'A' ),
	it should return 0x2c.  If there's no code to represent the given
	character, this function returns -1.
 */
 int symToCode( unsigned char ch )
 {
	static unsigned char charTable[28] = {	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
											'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R',
											'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '\n', ' ' };

	static int codeTable[28] = {	0x2c, 0x354, 0x6b4, 0xd4, 0x4, 0x2b4, 0x1b4, 0x154, 0x14,
									0xb6c, 0x1ac, 0x2d4, 0x6c, 0x34, 0x36c, 0x5b4, 0xdac, 0xb4,
									0x54, 0xc, 0xac, 0x2ac, 0x16c, 0x6ac, 0xd6c, 0x6d4, 0x56c, 0x5ac };

	//printf( "Code" );
	for( int i = 0; i < 28; i++ ) {
		
		if( ch == charTable[i] )
			return codeTable[i];
	}
	
	return -1;
 }
 
  /**
	Description copied from project website.
	Given the ASCII code for a character, this function returns the number
	of bits in the code used to represent it.  For example, if you call
	bitsInCode( 'A' ). it should return 6.  If there's no code to represent
	the given character, this function returns -1.
 */
 int bitsInCode( unsigned char ch ) 
 {
	static unsigned char charTable[28] = {	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
											'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R',
											'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '\n', ' ' };
											
	static int countTable[28] = {	6, 10, 11, 8, 3, 10, 9, 9, 5,
									12, 9, 10, 7, 6, 10, 11, 12, 8,
									7, 4, 8, 10, 9, 11, 12, 11, 11, 11 };

	//printf( "Size\n" );
	for( int i = 0; i < 28; i++ ) {
		
		if( ch == charTable[i] )
			return countTable[i];
	}
	
	return -1;
 }
 
 /**
	Description copied from project website.
	Given a code, this  function returns the ASCII character it represents.
	For example, if you call codeToSym( 0x2c ), it should return the ASCII
	code for A.  If the code does not represent a character, the function
	returns -1.
 */
 int codeToSym( int code )
 {
	 static unsigned char charTable[28] = {	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
											'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R',
											'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '\n', ' ' };
									
	static int codeTable[28] = {0x2c, 0x354, 0x6b4, 0xd4, 0x4, 0x2b4, 0x1b4, 0x154, 0x14,
								0xb6c, 0x1ac, 0x2d4, 0x6c, 0x34, 0x36c, 0x5b4, 0xdac, 0xb4,
								0x54, 0xc, 0xac, 0x2ac, 0x16c, 0x6ac, 0xd6c, 0x6d4, 0x56c, 0x5ac };
									
	for( int i = 0; i < 28; i++ ) {
		
		if( code == codeTable[i] )
			return charTable[i];
	}
	
	return -1;
 }
  