/**
    @file grid.c
    @author Nicholas Board (ndboard)

    The grid function will allow the caller to write
    horizontally or vertically to their game board,
    as wall as print the board to standard output.
*/
#include <stdio.h>
#include <string.h>
#include "grid.h"

/**
    Writes the the board horizontally.

    @param rpos The row position of the word to write.
    @param cpos The column position of the word to write.
    @param word The word to write.
    @param rows The number of rows in the grid.
    @param cols The number of columns in the grid.
    @param grid The grid to write the word to.
*/
void writeHorizontal( int rpos, int cpos, char *word,
                      int rows, int cols, char grid[ rows ][ cols ] )
{
  for ( int i = 0; word[i]; i++ ) {
    grid[rpos][cpos + i] = word[i];
  }
}

/**
    Writes the the board vertically.

    @param rpos The row position of the word to write.
    @param cpos The column position of the word to write.
    @param word The word to write.
    @param rows The number of rows in the grid.
    @param cols The number of columns in the grid.
    @param grid The grid to write the word to.
*/
void writeVertical( int rpos, int cpos, char word[],
                    int rows, int cols, char grid[ rows ][ cols ] )
{
  for ( int i = 0; word[i]; i++ ) {
    grid[rpos + i][cpos] = word[i];
  }
}

/**
    Prints the grid to standard output.

    @param rows The number of rows in the grid.
    @param cols The number of cols in the grid.
    @param board The grid to print out.
*/
void printGrid( int rows, int cols, char board[ rows ][ cols ] )
{
  for ( int i = 0; i < rows; i++ ) {
    for ( int j = 0; j <  cols ; j++ ) {
      if ( j < ( cols - 1 ) )
        printf( "%c ", board[i][j] );

      if ( j == ( cols - 1 ) )
        printf( "%c", board[i][j] );
    }
    printf( "\n" );
  }
}
