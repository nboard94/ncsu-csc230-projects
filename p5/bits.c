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
    // If there is any code left in the buffer, that needs to be retrieved.
    int code = ( buffer -> bits );
    int currentBitCount = ( buffer -> bits );
    
    // Iterate through the maxbits in a code and the size of a byte.
    while ( currentBitCount <= 20 ) {
      
      if ( currentBitCount > 0 ) {
        
        // CHECK FOR VALID CODE
        // check if the first bit is 0
        //if it is, check if any other bits are 1
        //if so, this is a bad code
        if( ( code & 0x8000) == 0 && code > 0 )
          return -2;

        int bitOne = -1;
        int bitTwo = -1;
        int itCount = 0;
        
        for( int i = 0; i < sizeof( int ) * 8; i++ ) {
          
            bitOne = bitTwo;
            bitTwo = code & ~( 0x01 << i );
            
            if( bitOne == 0 && bitTwo == 0 ) {
              // get the valid part of the code by making a mask to get the bits up to counter
              // update buffer with the bits not contained within the valid part
              //return valid;
            }
            
            itCount++;  
        }
      }
    
      // If we're here, it means that a 00 wasn't found in the code.
      // Look at the next byte.
      int next = fgetc( fp );
      
      // check if byte is EOF, if it is check if remianing bits in buffer, if so then bad code
      if ( feof( fp ) && ( buffer -> bcount ) > 0 ) {
        return -2;
      }
        
      // combine left over bits of the code with the new byte
      
      code = /* code combined with new byte */
      ( buffer -> bcount ) += ONE_BYTE_SIZE;
    }

    return -2;
}


