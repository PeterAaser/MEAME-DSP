#include "MEA21_lib.h"

void modifyRegister(Uint32 reg, Uint32 Mask, Uint32 Value);
int get_bit32(Uint32 bits, int index);
void read_segment(Uint32 start, int reads, Uint32* recv);
void write_segment(Uint32 start, int reads, Uint32* send);

void print_(char* s);
