#include "util.h"

void modifyRegister(Uint32 reg, Uint32 Mask, Uint32 Value)
{
  Uint32 Temp;

  Temp = READ_REGISTER(reg);
  Temp &= ~Mask;
  Temp |= (Value & Mask);
  WRITE_REGISTER(reg, Temp);
}

int get_bit32(Uint32 bits, int index)
{
  return (((bits) >> (index)) & 1);
}

void read_segment(Uint32 start, int reads, Uint32* recv)
{
  int ii;
  for (ii = 0; ii < reads; ii++)
    {
      recv[ii] = READ_REGISTER(start + (ii*4));
    }
}

void write_segment(Uint32 start, int writes, Uint32* send)
{
  int ii;
  for (ii = 0; ii < writes; ii++)
    {
      WRITE_REGISTER((start + (ii*4)), send[ii]);
    }
}

void write_byte_segment(Uint32 start, int writes, char* send)
{
  int ii;
  for (ii = 0; ii < writes; ii++)
    {
      WRITE_REGISTER((start + ii), send[ii]);
    }
}
