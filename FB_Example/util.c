#include "util.h"

void ModifyRegister(Uint32 reg, Uint32 Mask, Uint32 Value)
  {
    Uint32 Temp;

    Temp = READ_REGISTER(reg);
    Temp &= ~Mask;
    Temp |= (Value & Mask);
    WRITE_REGISTER(reg, Temp);
  }
