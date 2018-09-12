#ifndef UTIL_H
#define UTIL_H

/**
   Denotes that a side effect visible outside will happen, such as writing
   a register.
   A regular void is just side effecting wrt own program state, i.e changing
   some memory in a pointer.
 */
typedef void io_void;

typedef struct Word {
  int v;
} Word;

typedef struct Address {
  int v;
} Address;


#include "MEA21_lib.h"


/**
   Reads 32 bit words from start address into receive buffer
*/
void read_segment(int start, int reads, int* recv);


/**
   Writes 32 bit words to to memory from the send buffer
*/
void write_segment(int start, int writes, int* send);
void increment_register(Address address);
Word nth_bit(int index);
Word set_bit(Word word, int index);
int get_bit32(int bits, int index);

void write_segment_typed(Address start_address, int writes, Word* send);
void read_segment_typed(Address start_address, int reads, Word* receive);
void reset_segment(Address start_address, int writes);

void write_segment_fields(Address start_address, int writes, Word send, int first_bit, int field_size);
/**
   Changes only the field defined by field_size.
   Rest of the word is unchanged
*/
Word set_field(Word word, int first_bit, int field_size, int field_value);
io_void set_field_by_address(Address address, int first_bit, int field_size, int field_value);


/**
   More typesafe way of reading.
   Ensures through struct wrapping that addresses and values are not mixed
 */
io_void write_register(Address address, Word word);
io_void write_register_int(Address address, int word);

Word read_register(Address address);

int in_range(int i, int min, int max);

#endif // UTIL_H
