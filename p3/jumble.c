/** 
    @file jumble.c
    @author Nicholas Board (ndboard)

    The jumble program will read a dictionary of words
    from an input file.  It will then take user input to
    create a set of letters.  It then reports if the given
    letters match any of the words in the dictionary.
    Continues to accept user input until the EOF character
    is reached.
*/
#include<stdio.h>
#include<stdbool.h>
#include<string.h>

/** Definitions for controlling the lengths of strings. */
#define MAX_WORDS 100000
#define MAX_WORD_LENGTH 20
#define EXTRA_WORD_LENGTH 22

/** Declare allowed global variables, the word array and wordCount. */
char words[MAX_WORDS][EXTRA_WORD_LENGTH];
int wordCount = 0;

/** Prototype functions called during the main. */
void readWords( char const * );
bool getLetters( char * );
bool matches( char const *, char const * );

/** 
    Starts the program.  Makes sure that the proper file can be opened,
    passes it to readWords to populate words.  Then constantly calls
    getLetters until EOF is reached.

    @param argc The number of command-line arguments.
    @param argv The list of command-line arguments.
    
    @return program exit status
*/
int main(  int argc, char *argv[]  )
{
  char letters[EXTRA_WORD_LENGTH];
  
  // Check to make sure the number of command line arguments is one.
  // If not, print error message and return 1.
  if( argc != 2 ) {
    fprintf( stderr, "usage: jumble <word-file>\n" );
    return 1;
  }
  
  // Check to make sure file exists and can be read.
  // If not, print error message and return 1.
  // Close the file after these checks.
  FILE *inputFile = fopen( argv[1], "r" );
  if ( inputFile == NULL ) {
    fprintf( stderr, "Can't open word file\n" );
    return 1;
  }
  fclose( inputFile );
  
  // Call readWords to populate the words array.
  readWords( argv[1] );
  
  // Cycle through every word and check that it is no longer
  // than 20 characters, and consists only of lowercases.
  int countCheck = 0;
  int currentLen = 0;
  for( int i = 0; i < wordCount; i++ ) {
    
    currentLen = strlen( words[i] );
    if( currentLen > MAX_WORD_LENGTH ) {
      fprintf( stderr, "Invalid word file\n" );
      return 1;
    }
    
    for( int j = 0; words[i][j]; j++ ) {
      
      if( words[i][j] < 'a' || words[i][j] > 'z' ) {
        fprintf( stderr, "Invalid word file\n" );
        return 1;
      }
    }
    
    countCheck++;
  }
  
  // Make sure that we haven't exceeded the max amount of words.
  if( wordCount > MAX_WORDS ) {
    fprintf( stderr, "Invalid word file\n" );
    return 1;
  }
  
  //Make sure that wordCount is correct.
  if( wordCount != countCheck ) {
    fprintf( stderr, "Invalid word file\n" );
    return 1;
  }
  
  //Call getLetters until EOF is reached.
  while( getLetters( letters ) == 0);
  
  // Exit successfully.
  return 0;
}

/** 
    Reads words from the input file and fills the global
    words array.

    @param filename The file to be processed.
*/
void readWords( char const *filename )
{
  // Opens the filestream for the inputfile.
  FILE *inputFile = fopen( filename, "r" );
  
  // Populate the words array, increment wordCount.
  while ( fscanf( inputFile, "%21s", words[wordCount] ) == 1 ) {
    
    wordCount++;
  }
  
  // Close the file stream.
  fclose( inputFile );
}

/** 
    Prompts the user for input and will fill the letters array.
    Makes sure the number of letters does not exceed 20, and are
    all lowercase.  Then calls the matches function and loops
    until EOF is reached.

    @param letters The array of lettesr to populate.
    
    @return 1 When EOF is reached.
    @return 0 If the loop is broken out of.
*/
bool getLetters( char *letters )
{

  // Will loop continuously.
  bool isValid;
  while( true ) {

    // Reset isValid, and prompts the user.
    isValid = true;
    printf( "letters> " );

    // Populates the letters array with, at most, 21
    // characters.  Also checks to see if EOF is the
    // first character entered.
    if( isValid && scanf( "%21s", letters ) != 1 ) {
    
      if( getchar() == EOF )
        return 1;
    
      printf( "Invalid letters\n" );
      isValid = false;
    }

    // Check to make sure the letters array does not
    // exceed our maximum amount of letters.
    int len = strlen( letters );
    if( isValid && ( len > MAX_WORD_LENGTH || len <= 0 ) ) {
      
      printf( "Invalid letters\n" );
      isValid = false;
    }

    // Make sure that every character in letters is
    // either EOF, at which point we break out of the
    // function, or a lowercase letter.
    for( int i = 0; letters[i]; i++ ) {

      if( letters[i] == EOF )
        return 1;

      if( isValid && ( letters[i] < 'a' || letters[i] > 'z' ) ) {

        printf( "Invalid letters\n" );
        isValid = false;
      } 
    }

    // If we have a valid string of letters, we compare
    // it to every word in words, reporting matches.
    if( isValid ) {

      for( int i = 0; i < wordCount; i++ ) {

        if( matches( words[i], letters ) ) {

          printf( "%s\n", words[i] ); 
        }
      }
    }
  }
  
  // Return successfully.
  return 0;
}

/** 
    Checks to see if the given set of letters matches, or is
    an anagram of, the given word.

    @param word The current word to compare against.
    @param argv The letters to compare against.
    
    @return True or false if there is or isn't a match.
*/
bool matches( char const *word, char const *letters )
{
  // Check to make sure that the word and letters are the same length.
  int len1 = strlen( word );
  int len2 = strlen( letters );
  
  if( len1 != len2 )
    return 0;
  
  // Declare two bool arrays that will be used for checking,
  // and set every value to false.
  bool matchW[len1];
  bool matchL[len2];
  
  for( int i = 0; i < len1; i++ ) {
    matchW[i] = false;
    matchL[i] = false;
  }
  
  // For every letter in the word, compare it against every letter
  // in letters, and check to make sure that letter hasn't been used yet.
  // If two unused letters are matched, their corresponding values
  // in the match arrays are set to true.
  for( int i = 0; word[i]; i++ ) {
    
    for( int j = 0; letters[j]; j++ ) {
      
      if( matchW[i] == false && matchL[j] == false && word[i] == letters[j] ) {
        
        matchW[i] = true;
        matchL[j] = true;
        
      }
    }
  }
  
  // Make sure every value in the word was matched.
  for( int i = 0; i < len1; i++ ) {
    if( matchW[i] == false )
      return 0;
  }
  
  // Make sure every value in the letters was matched.
  for( int i = 0; i < len1; i++ ) {
    if( matchL[i] == false )
      return 0;
  }
  
  // If all tests have passed, return true.
  return 1;
}
