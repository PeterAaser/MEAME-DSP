// #include <stdio.h>
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
#include "version.h"
#include "registers.h"
#include "MEA21_lib.h"

#include "comms.h"

// MCS stuff
char dsp_version[] = "(>)"SW_STRING"(<)";
CSL_GpioRegsOvly gpioRegs = (CSL_GpioRegsOvly)CSL_GPIO_0_REGS;
///////////////


void main()
{
  MEA21_init();
  setup();

  while(1){
    execute_instructions();
  }
}
