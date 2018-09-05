#include "MEA21_lib.h"

void modifyRegister(Uint32 reg, Uint32 Mask, Uint32 Value);
int get_bit32(Uint32 bits, int index);
void read_segment(Uint32 start, int reads, Uint32* recv);
void write_segment(Uint32 start, int reads, Uint32* send);
void increment_register(Uint32 address);
UInt32 nth_bit(UInt32 index);
UInt32 set_bit(UInt32 word, UInt32 index);
