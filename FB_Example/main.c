#include <stdio.h>
#include <string.h>

#include <cslr_pllc.h>
#include <cslr_gpio.h>
#include <cslr_emifa.h>
#include <cslr_ddr2.h>
#include <cslr_dev.h>
#include <cslr_intc.h>
#include <cslr_chip.h>
#include <cslr_edma3cc.h>
#include <cslr_tmr.h>
#include <soc.h>

#include <math.h>

#include <c6x.h>

extern void intcVectorTable(void);

#include "main.h"
#include "setup.h"
#include "timer.h"
#include "stimulus.h"
#include "stimpack.h"
#include "version.h"
#include "MEA21_lib.h"

char dsp_version[] = "(>)"SW_STRING"(<)";

CSL_GpioRegsOvly gpioRegs = (CSL_GpioRegsOvly)CSL_GPIO_0_REGS;


void main()
{
  static int count = 0;
  int count_test1;
  int count_test2;

  static int value = 0;
  volatile int i;


  MEA21_init();
  setup();

  while(1)
    {
      // No idea, maybe a busy loop?
      WRITE_REGISTER(0x002C, 0x700 + 1*value);
      for (i = 0; i < 100000; i++);
      value = 1 - value; // switch on/off
    }
}

