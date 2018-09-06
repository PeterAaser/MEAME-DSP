#ifndef UTIL_H
#define UTIL_H

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
void write_segment(int start, int reads, int* send);
void increment_register(Address address);
Word nth_bit(int index);
Word set_bit(Word word, int index);
int get_bit32(Uint32 bits, int index);



/**
   Changes only the field defined by field_size.
   Rest of the word is unchanged
*/
Word set_field(Word word, int first_bit, int field_size, int field_value);


/**
   More typesafe way of reading.
   Ensures through struct wrapping that addresses and values are not mixed
 */
void write_register(Address address, Word word);

#endif // UTIL_H
