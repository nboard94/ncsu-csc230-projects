#include<stdio.h>
#include<stdlib.h>

#define RED "\x1B[31m"
#define BLUE "\x1B[34m"
#define BLACK "\x1B[0m"
#define UNSUCCESSFUL_TAG_EXIT 101
#define UNSUCCESSFUL_ENTITY_EXIT 100

void showTag();
void showEntity();

int main ()
{

  int c;

  while( ( c = getchar() ) != EOF ){
    if ( c == '<' )
    {
      showTag();
    }

    else if (c == '&' )
    {
      showEntity();
    }
    else {
      putchar( c );
    }
  }

  return 0;
}

void showTag()
{
  int c;

  printf( "%s", RED );

  putchar( '<' );

  while ( ( c = getchar() ) != '>' ) {

    if ( c == -1 ) {
       printf( "%s", BLACK );
       exit( UNSUCCESSFUL_TAG_EXIT );
    }

    putchar( c );
  }

  putchar( '>' );

  printf( "%s", BLACK );
}

void showEntity()
{
 int c;

  printf( "%s", BLUE );

  putchar( '&' );

  while ( ( c = getchar() ) != ';' ) {

    if ( c == -1 ) {
       printf( "%s", BLACK );
       exit( UNSUCCESSFUL_ENTITY_EXIT );
    }

    putchar( c );
  }

  putchar( ';' );

  printf( "%s", BLACK );
}