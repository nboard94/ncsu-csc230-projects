/**
    @file puzzle.c
    @author Nicholas Board (ndboard)

    The puzzle program will read a dictionary of words
    from an input file.  It will then take user input to
    create a set of letters.  It then reports if the given
    letters match any of the words in the dictionary.
    Continues to accept user input until the EOF character
    is reached.
*/
#include <stdio.h>
#include <string.h>
#include "grid.h"

/** Define the max dimensions of the board, max word length, and others. */
#define MAX_DIMENSIONS 40
#define MAX_WORDLENGTH 10
#define PARAM_ONE 3
#define PARAM_TWO 4

/**
    Starts the program.  Makes sure that the proper file can be opened,
    passes one row of information at a time to the writeHorizontal and
    writeVertical functions to fill in the gameboard.

    @param argc The number of command-line arguments.
    @param argv The list of command-line arguments.
    
    @return program exit status
*/
int main( int argc, char *argv[] )
{

  // Declare basic variables, amount of rows, amount of
  // columns, and amount of words.
  int rows = 0;
  int cols = 0;
  int wordCount = 0;
  
  // Check to make sure the number of command line arguments is one.
  // If not, print error message and return 1.
  if ( argc != 2 ) {
    fprintf( stderr, "usage: puzzle <input-file>\n" );
    return 1;
  }
  
  // Open the file, make sure that it has opened successfully.
  // If the file points to null, print error message and return 1.
  FILE *inputFile = fopen( argv[1], "r" );
  if ( inputFile == NULL ) {
    fprintf( stderr, "usage: puzzle <input-file>\n" );
    return 1;
  }
  
  // Read in the number of rows, cols, and the wordCount.
  // If file does not match the format, print error message and return 1.
  if ( fscanf( inputFile, "%d %d\n%d\n", &rows, &cols, &wordCount ) != PARAM_ONE) {
    fprintf( stderr, "Invalid input file\n" );
    return 1;
  }
  
  // Create the variables to hold information about each word.  As well
  // as the array to temporarily hold a word before adding it to the board,
  // and the board itself.
  char orientation = ' ';
  int rpos = 0;
  int cpos = 0;
  char word[MAX_WORDLENGTH];
  char board[rows][cols];
  int wordCheck = 0;
  int len = 0;
  int availableSpace = 0;
  
  // Fill in all slots on the board with space characters.
  for (int i = 0; i < cols; i++ ) {
    for ( int j = 0; j < rows; j++ ) {
      board[i][j] = ' ';
    }
  }
  
  // Continues to read in a word and its orientation and location
  // as long as all parameters are correctly matched.
  while ( fscanf( inputFile, "%c %d %d %s\n", &orientation, &rpos, &cpos, word ) == PARAM_TWO ) {

    // Set the len variable to length of the word.
    len = strlen( word );
    
    // Check to make sure the word doesn't extend beyond our
    // specified maximum, prints error and returns 1 if it does.
    if ( strlen( word ) > MAX_WORDLENGTH ) {
      fprintf( stderr, "Invalid input file\n" );
      return 1;
    }
    
    // Check to make sure the word wouldn't extend beyong the limits
    // of the game board horizontally.  If so, prints error and returns 1.
    if ( orientation == 'H' )  {
      availableSpace = cols - cpos;
      
      if ( len > availableSpace ) {
        fprintf( stderr, "Invalid input file\n" );
        return 1;
      }
    }
    
    // Check to make sure the word wouldn't extend beyond the limits
    // of the game board vertically.  If so, prints error and returns 1.
    if ( orientation == 'V' && ( cpos + len - 1 ) > rows )  {
      availableSpace = rows - rpos;
      
      if ( len > availableSpace ) {
        fprintf( stderr, "Invalid input file\n" );
        return 1;
      }
    }
    
    // This large hunk of code makes sure that the words new letters
    // do not contradict letters that are already in the grid.
    if (orientation == 'H' ) {

      for ( int i = 0; word[i]; i++ ) {

        if ( board[rpos][cpos + i] != ' ' && board[rpos][cpos + i] != word[i] ) {
          fprintf( stderr, "Invalid input file\n" );
          return 1;
        }
      }
    }
    
    if (orientation == 'V') {

      for ( int i = 0; word[i]; i++ ) {
      
        if ( board[rpos + i][cpos] != ' ' && board[rpos + i][cpos] != word[i] ) {
          fprintf( stderr, "Invalid input file\n" );
          return 1;
        }
      }
    }


    // Calls the writeHorizontal or the writeVertical function that are
    // defined in grid.c and prototyped in grid.h, depending on the
    // orientation variable.
    if ( orientation == 'H' )
      writeHorizontal( rpos, cpos, word, rows, cols, board );

    if ( orientation == 'V' )
      writeVertical( rpos, cpos, word, rows, cols, board );

    // Increment wordcheck.
    wordCheck++;
  }
  
  // Close the file stream.
  fclose( inputFile );
  
  // Checks wordCheck against wordCount to make sure that
  // the expected number of words was read in.
  if ( wordCheck != wordCount) {
    fprintf( stderr, "Invalid input file\n" );
    return 1;
  }
  
  // Calls thr printGrid function defined in grid.c
  // and prototyped in grid.h, passing in the rows,
  // cols, and the board.
  printGrid( rows, cols, board );
  
  // Return 0 if the program execcuted successfully.
  return 0;
}
