 /**
  @file codes.h
  @author Nick Board
  
  Header file for the codes.c compoenent, with functions supporting
  mapping betweeen symbols and the codes used to represent them.
 */
 
 #include <stdio.h>
 #include <stdlib.h>
 
 /**
  Description copied from project website.
  Given the ASCII code for a character, this function returns the
  code used to represent it.  For example, if you call symToCode( 'A' ),
  it should return 0x2c.  If there's no code to represent the given
  character, this function returns -1.
 */
 int symToCode( unsigned char ch );
 
 /**
  Description copied from project website.
  Given the ASCII code for a character, this function returns the number
  of bits in the code used to represent it.  For example, if you call
  bitsInCode( 'A' ). it should return 6.  If there's no code to represent
  the given character, this function returns -1.
 */
 int bitsInCode( unsigned char ch );
 
 /**
  Description copied from project website.
  Given a code, this  function returns the ASCII character it represents.
  For example, if you call codeToSym( 0x2c ), it should return the ASCII
  code for A.  If the code does not represent a character, the function
  returns -1.
 */
 int codeToSym( int code );
