/**
  @file voronoi.c
  @file Nicholas Board (ndboard)

  This program takes in three points through commandline and
  creates a voronoi diagram based off of them.  It pixel in the
  image is painted a different color based off of which of the
  three input points is closest.
*/

#include<stdio.h>
#include<stdbool.h>
#include<math.h>

// The six doubles that store the input coordinates.
double pointOneX = 0;
double pointOneY = 0;
double pointTwoX = 0;
double pointTwoY = 0;
double pointThreeX = 0;
double pointThreeY = 0;

// Controls the parameters.
#define DIMENSIONS 100
#define COORDINATES 6
#define INVALID_INPUT_EXIT 100

// Prototyping functions used.
bool nearestPoint( int, int, double, double );
void chooseColor( int, int);
double getDistance( double, double, double, double );

/**
  Where the program begins.

  @return program exit status
*/
int main( ) {

  /**
    Takes in the coordinates.  The int count keeps track
    of the amount of doubles successfully read in.  The if
    statement will make sure six are read in, and gives an
    error message if not.
  */
  int count = scanf( "%lf%lf%lf%lf%lf%lf", &pointOneX, &pointOneY, &pointTwoX );
  int count = scanf( "%lf%lf%lf%lf%lf%lf", &pointTwoY, &pointThreeX, &pointThreeY );

  if ( count != COORDINATES ) {
    printf("Invalid input\n");
    return INVALID_INPUT_EXIT;
  }

  // Prints the header information for the .ppm filetype.
  printf( "P3\n%d %d\n255\n", DIMENSIONS, DIMENSIONS );

  /**
    Loops through DIMENSIONS^2 times, calling chooseColor
    on eaach pixel and switches printing to a new line
    after every DIMENSIONS amount.
  */
  for ( int i = 0; i < DIMENSIONS; i++ ) {
    for ( int j = 0; j < DIMENSIONS; j++ ) {

      chooseColor( j, i );
    }

    printf( "\n" );
  }

  // Exits the program successfully.
  return 0;
}

/**
  Determines if the given point is the closest one to the given pixel.

  @param col The column of the given pixel.
  @param row The two of the given pixel.
  @param x The x coordinate of the given white point.
  @param y The y coordinate of the given white point.
  @return 1 if the given point is closest to the given pixel.
  @return 0 if the given point is not the closest to the given pixel.
*/
bool nearestPoint( int col, int row, double x, double y ) {

  //test for white point
  int testX = round( x );
  int testY = round( y );

  if ( testX == col && testY == row ) {
    return 0;
  }

  // The distances between the given pixel and each white point.
  double a = getDistance( col, row, pointOneX, pointOneY );
  double b = getDistance( col, row, pointTwoX, pointTwoY );
  double c = getDistance( col, row, pointThreeX, pointThreeY );

  // Determines the smallest of the distances.
  double smallest = a;

  if ( b < smallest ) {
    smallest = b;
  }

  if ( c < smallest ) {
    smallest = c;
  }

  // The distance between the given pixel and the given white point.
  double d = getDistance( col, row, x, y );
  
  // If the closest white point was given, then 1 is returned.
  if ( smallest == d ) {
    return 1;
  }

  // If the closest white point was not given, then 0 is returned.
  return 0;
}

/**
  Checks each white point to see if it is the closest, and it is
  painted the appropriate color.  If it is matched with none then
  it is painted white.

  @param col The column of the current pixel.
  @param row The row of the current pixel.
*/
void chooseColor( int col, int row ) {

  // Paints the pixel red if it is closest to the first point.
  if ( nearestPoint( col, row, pointOneX, pointOneY ) == 1 ) {
    printf( "255   0   0 " );
  }
  // Paints the pixel green if it is closest to the second point.
  else if ( nearestPoint( col, row,  pointTwoX, pointTwoY ) == 1) {
    printf( "  0 255   0 " );
  }
  // Paints the pixel blue if it is closest to the third point.
  else if ( nearestPoint( col, row, pointThreeX, pointThreeY ) == 1) {
    printf( "  0   0 255 " );
  }
  // Paints the pixel white if it is a given point.
  else {
    printf( "255 255 255 " );
  }
}

/**
  Determines the distance between two points given.

  @param x1 The x coordinate of the first point.
  @param y1 The x coordinate of the first point.
  @param x2 The x coordinate of the second point.
  @param y2 The x coordinate of the second point.
*/
double getDistance( double x1, double y1, double x2, double y2) {
  return sqrt( ( ( x1 - x2 ) * ( x1 - x2 ) ) + ( ( y1 - y2 ) * ( y1 - y2 ) ) );
}
