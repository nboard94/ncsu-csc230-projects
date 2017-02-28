/** 
    @file grid.h
    @author Nicholas Board (ndboard)

    Contains prototypes of the functions
    defined in grid.c
*/


/** 
    Writes the the board horizontally.

    @param rpos The row position of the word to write.
    @param cpos The column position of the word to write.
    @param word The word to write.
    @param rows The number of rows in the grid.
    @param cols The number of columns in the grid.
    @param grid The grid to write the word to.
*/
void writeHorizontal( int rpos, int cpos, char word[], int rows, int cols, char grid[ rows ][ cols ] );

/** 
    Writes the the board vertically.

    @param rpos The row position of the word to write.
    @param cpos The column position of the word to write.
    @param word The word to write.
    @param rows The number of rows in the grid.
    @param cols The number of columns in the grid.
    @param grid The grid to write the word to.
*/
void writeVertical( int rpos, int cpos, char word[], int rows, int cols, char grid[ rows ][ cols ] );

/** 
    Prints the grid to standard output.

    @param rows The number of rows in the grid.
    @param cols The number of cols in the grid.
    @param board The grid to print out.
*/
void printGrid( int rows, int cols, char board[ rows ][ cols ] );