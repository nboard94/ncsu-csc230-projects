#include <stdio.h>
#include <stdlib.h>
#include "codes.h"

#define THREE 3
#define FOUR 4
#define FIVE 5
#define SIX 6
#define SEVEN 7
#define EIGHT 8
#define NINE 9
#define TEN 10
#define ELEVEN 11
#define TWELVE 12
#define NUM_CHARS 28

 /**
  Description copied from project website.
  Given the ASCII code for a character, this function returns the
  code used to represent it.  For example, if you call symToCode( 'A' ),
  it should return 0x2c.  If there's no code to represent the given
  character, this function returns -1.
 */
 int symToCode( unsigned char ch )
 {
  static unsigned char charTable[NUM_CHARS] = {  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
                      'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R',
                      'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '\n', ' ' };

  static int codeTable[NUM_CHARS] = {  0x2c, 0x354, 0x6b4, 0xd4, 0x4, 0x2b4, 0x1b4, 0x154, 0x14,
                  0xb6c, 0x1ac, 0x2d4, 0x6c, 0x34, 0x36c, 0x5b4, 0xdac, 0xb4,
                  0x54, 0xc, 0xac, 0x2ac, 0x16c, 0x6ac, 0xd6c, 0x6d4, 0x56c, 0x5ac };

  //printf( "Code" );
  for ( int i = 0; i < NUM_CHARS; i++ ) {

    if ( ch == charTable[i] )
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
  static unsigned char charTable[NUM_CHARS] = {  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
                      'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R',
                      'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '\n', ' ' };
                      
  static int countTable[NUM_CHARS] = {  SIX, TEN, ELEVEN, EIGHT, THREE, TEN, NINE, NINE, FIVE,
                  TWELVE, NINE, TEN, SEVEN, SIX, TEN, ELEVEN, TWELVE, EIGHT,
                  SEVEN, FOUR, EIGHT, TEN, NINE, ELEVEN, TWELVE, ELEVEN, ELEVEN, ELEVEN };

  for ( int i = 0; i < NUM_CHARS; i++ ) {

    if ( ch == charTable[i] )
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
   static unsigned char charTable[NUM_CHARS] = {  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
                      'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R',
                      'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '\n', ' ' };

  static int codeTable[NUM_CHARS] = {0x2c, 0x354, 0x6b4, 0xd4, 0x4, 0x2b4, 0x1b4, 0x154, 0x14,
                0xb6c, 0x1ac, 0x2d4, 0x6c, 0x34, 0x36c, 0x5b4, 0xdac, 0xb4,
                0x54, 0xc, 0xac, 0x2ac, 0x16c, 0x6ac, 0xd6c, 0x6d4, 0x56c, 0x5ac };

  for ( int i = 0; i < NUM_CHARS; i++ ) {

    if ( code == codeTable[i] )
      return charTable[i];
  }
  
  return -1;
 }
