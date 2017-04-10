#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "bits.h"
#include "codes.h"

#define TWO_BYTE_SIZE 16
#define ONE_BYTE_SIZE 8
#define HALF_BYTE_SIZE 4

/** Write the code stored in the code parameter.  Temporarily
    store bits in the given buffer until we have 8 of them and can
    write them to the file.
    @param code to write out
    @param nbits number of bits in code
    @param buffer pointer to storage for bits that can't be written to the
    file yet.  When this function is called, this buffer may already
    contain some bits left-over from a previous write.
    @param fp file we're writing to, opened for writing in binary mode.
*/
void writeBits( int code, int nbits, BitBuffer *buffer, FILE *fp )
{

  // For some reason I couldn't figure out, my codes were printing backwards,
  // so I need to turn the code around before I process it.
  // Flip the code over, and shift to the low-order position.
  code = ( ( code & 0xff00 ) >> ONE_BYTE_SIZE ) | ( ( code & 0xff ) << ONE_BYTE_SIZE );
  code = ( ( code & 0xf0f0 ) >> HALF_BYTE_SIZE ) | ( ( code & 0xf0f ) << HALF_BYTE_SIZE );
  code = ( ( code & 0xcccc ) >> 2 ) | ( ( code & 0x3333 ) << 2 );
  code = ( ( code & 0xaaaa ) >> 1 ) | ( ( code & 0x5555 ) << 1 );
  code = code >> ( TWO_BYTE_SIZE - nbits );
  
  // Mask to isolate the lowest-order bit.
  int lowMask = 0x1;
  
  // Used to store the lowest-order bit temporarily;
  int currentBit;
  
  // Iterate over every bit in the code.
  for ( int i = 0; i < nbits; i++ ) {

    // Flush the buffer when it's full.
    if ( ( buffer -> bcount ) == ONE_BYTE_SIZE ) {

      flushBits( buffer, fp);
    }
    // Otherwise, make room for a new bit in the low-order position.
    else
      ( buffer -> bits ) = ( buffer -> bits ) << 1;

    // Mask out everything except the lowest-order bit in the code.
    currentBit = code & lowMask;
    
    // Inserts the currentBit into the low-order position of the buffer.
    ( buffer -> bits ) = currentBit | ( buffer -> bits );
    
    // Increment the buffer's bitcount.
    ( buffer -> bcount )++;
    
    // Shift the code to the right to prepare for the next bit to buffer.
    code = code >> 1;
  }
}

/** If there are any bits buffered in buffer, write them out to the
    given file in the high-order bit positions of a byte, leaving zeros
    in the low-order bits.
    @param buffer pointer to storage for unwritten bits left over
    from a previous write.
    @param fp file these bits are to be written to, opened for writing
    in binary mode
*/
void flushBits( BitBuffer *buffer, FILE *fp )
{

  // Used to set the buffer to all zeros.
  unsigned char reset = 0x0;

  // Only need to flush bits if any are buffered.
  if ( ( buffer -> bcount ) > 0 ) {

    // Moves relevant bits into the high-order position.
    ( buffer -> bits ) = ( buffer -> bits ) << ( ONE_BYTE_SIZE - ( buffer -> bcount ) );
    
    // Writes the contents of the buffer to the outputFile, eight bits at a time.
    fwrite( &( buffer -> bits ), sizeof(unsigned char), 1, fp );

    // Empties out the buffer and sets the bit count to zero.
    ( buffer -> bits ) = reset;
    ( buffer -> bcount ) = 0;
  }
}

/** Reads and returns the next valid code from the given file. Each valid code
    starts with a 1 and ends with two consecutive 0s (00).
    if no bits or only 0s have been read when the end of file is reached,
    -1 is returned.
    If the first bit read is a 1 and the end of file is reached before
    two consecutive 0s (00) are read, -2 is returned.
    If the first bit read is a 0 and a 1 is read before the end of the file
    is reached, -2 is returned.
    The given buffer may contain some bits left over from the last read, and if this
    the read has any left-over bits, it should leave them in that buffer.
    @param buffer pointer to storage for left-over from one read call to the
    next.
    @param fp file bits are being read from, opened for reading in binary.
    @return value of the valid code read in, -1 if we reach the
    end-of-file under valid conditions, and -2 if the file is invalid.
*/
int readBits (BitBuffer *buffer, FILE *fp )
{
  
  // This int will hold the returned code.
  int code = 0;
  
  // This bool will let us know if we've started a code.
  bool processing = false;

  // If I have bits leftover from the last read in the buffer,
  // need to start the code with this.
  while( ( buffer -> bcount ) > 0 ) {

    // Grab the high-order bit from the buffer and insert at the end of code.
    // Shift the buffer over and decrement bcount.
    code = code | ( ( buffer -> bits ) & 0x80 );
    ( buffer -> bits ) == ( buffer -> bits ) << 1;
    ( buffer -> bcount )--;

    // Check to see if we've found a 1 and a code has started, if so turn our bool on.
    if ( code == 0x1 )
      processing = true;

    // Check after every insertion to see if a valid end has been read.
    // If it has, return it.  If not, prepare the code for another bit.
    if( processing && ( code & 0x3 ) == 0 ) {

      return code;
    }
    else
      code = code << 1;
  }
  
  // Now it's time to read from the file, raw will hold a byte read directly from the file.
  unsigned char raw;
  fread( &raw, sizeof(unsigned char), 1, fp );
  
  for( int i = 0; i < 8; i++ ) {
    
    // Grab the high-order bit from raw and insert at the end of the code.
    code = code | ( raw & 0x80 );
    
    // Check to see if we've found a 1 and a code has started, if so turn our bool on.
    if ( code == 0x1 )
      processing = true;
    
    // Check after every insertion to see if a valid end has been read.
    // If it has, we need to insert the rest into the buffer and return it.
    // If not, prepare the code for another bit.
    if( processing && ( code & 0x3 ) == 0 ) {
      
      ( buffer -> bits ) = raw;
      ( buffer -> bcount ) = 8 - i;
      return code;
    }
    else
      code = code << 1;
  }
  
  // If we didn't start processing and reach EOF, return -1 to signal the end.
  if ( !processing && feof( fp ) )
    return -1;
  
  // Otherwise, something is wrong with the input, return -2 to signal that.
  if ( processing && feof( fp ) )
    return -2;
  
  return code;
}
