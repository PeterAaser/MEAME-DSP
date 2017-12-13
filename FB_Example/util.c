#include "util.h"

// TODO: Is this MCS shite or is it mine?
void modifyRegister(Uint32 reg, Uint32 Mask, Uint32 Value){

  Uint32 Temp;

  Temp = READ_REGISTER(reg);
  Temp &= ~Mask;
  Temp |= (Value & Mask);
  WRITE_REGISTER(reg, Temp);
}

// TODO: Likewise
int get_bit32(Uint32 bits, int index){

  return (((bits) >> (index)) & 1);
}


/**
   Reads 32 bit words from start address into receive buffer
 */
void read_segment(Uint32 start, int reads, Uint32* recv){
  int ii;
  for (ii = 0; ii < reads; ii++){
    recv[ii] = READ_REGISTER(start + (ii*4));
  }
}


/**
   Writes 32 bit words to to memory from the send buffer
*/
void write_segment(Uint32 start, int writes, Uint32* send){
  int ii;
  for (ii = 0; ii < writes; ii++){
    WRITE_REGISTER((start + (ii*4)), send[ii]);
  }
}

void write_byte_segment(Uint32 start, int writes, char* send){
  int ii;
  for (ii = 0; ii < writes; ii++){
    WRITE_REGISTER((start + ii), send[ii]);
  }
}

void increment_register(Uint32 address){
  Uint32 current = READ_REGISTER(address);
  WRITE_REGISTER(address, current+1);
}
