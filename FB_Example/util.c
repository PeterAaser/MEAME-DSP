#include "util.h"


Word set_field(Word word, int first_bit, int field_size, int field_value){
  int field = (field_value << first_bit);
  int mask = (1 << field_size) - 1;
  int masked = ~((~word.v) | mask);

  Word w;
  w.v = field | masked;
  return w;
}


Word set_bit(Word word, int index){
  Word w;
  w.v = (1 << index) | word.v;
  return w;
}


Word nth_bit(int index){
  Word w;
  w.v = (1 << index);
  return w;
}


void read_segment(int start, int reads, int* recv){
  int ii;
  for (ii = 0; ii < reads; ii++){
    recv[ii] = READ_REGISTER(start + (ii*4));
  }
}


void write_segment(int start, int writes, int* send){
  int ii;
  for (ii = 0; ii < writes; ii++){
    WRITE_REGISTER((start + (ii*4)), send[ii]);
  }
}

void write_byte_segment(int start, int writes, char* send){
  int ii;
  for (ii = 0; ii < writes; ii++){
    WRITE_REGISTER((start + ii), send[ii]);
  }
}

void increment_register(Address address){
  int current = READ_REGISTER(address.v);
  WRITE_REGISTER(address.v, current+1);
}

void write_register(Address address, Word word){
  WRITE_REGISTER(address.v, word.v);
}

void write_register_int(Address address, int word){
  WRITE_REGISTER(address.v, word);
}

int get_bit32(Uint32 bits, int index){

  return (((bits) >> (index)) & 1);
}
