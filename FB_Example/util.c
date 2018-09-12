#include "util.h"


Word set_field(Word word, int first_bit, int field_size, int field_value){
  int field = (field_value << first_bit);
  int mask = (1 << field_size) - 1;
  int masked = ~((~word.v) | mask);

  Word w;
  w.v = field | masked;
  return w;
}


void set_field_by_address(Address address, int first_bit, int field_size, int field_value){
  Word w = read_register(address);
  w = set_field(w, first_bit, field_size, field_value);
  write_register(address, w);
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


// unsafe due to no typing
void read_segment(int start, int reads, int* recv){
  int ii;
  for (ii = 0; ii < reads; ii++){
    recv[ii] = READ_REGISTER(start + (ii*4));
  }
}


// unsafe due to no typing
void write_segment(int start, int writes, int* send){
  int ii;
  for (ii = 0; ii < writes; ii++){
    WRITE_REGISTER((start + (ii*4)), send[ii]);
  }
}


// unsafe due to no typing
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


Word read_register(Address address){
  Word w;
  w.v = READ_REGISTER(address.v);
  return w;
}


int get_bit32(int bits, int index){
  return (((bits) >> (index)) & 1);
}


void write_segment_typed(Address start_address, int writes, Word* send){
  int ii;
  for (ii = 0; ii < writes; ii++){
    write_register(start_address, send[ii]);
    start_address.v += 4;
  }
}


void read_segment_typed(Address start_address, int reads, Word* receive){
  int ii;
  for (ii = 0; ii < reads; ii++){
    receive[ii] = read_register(start_address);
    start_address.v += 4;
  }
}


void reset_segment(Address start_address, int writes){
  int ii;
  for (ii = 0; ii < writes; ii++){
    write_register_int(start_address, 0);
    start_address.v += 4;
  }
}


void write_segment_fields(Address start_address, int writes, Word send, int first_bit, int field_size){
  int ii;
  for(ii = 0; ii < writes; ii++){
    Word temp = read_register(start_address);
    temp = set_field(temp, first_bit, field_size, send.v);
    write_register(start_address, temp);
    start_address.v += 4;
  }
}


int in_range(int i, int min, int max){
  return((i >= 0) && (i <= max));
}
