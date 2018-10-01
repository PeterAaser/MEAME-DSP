#include <csl.h>
#include <cslr_tmr.h>
#include <cslr_gpio.h>
#include <cslr_chip.h>
#include <cslr_edma3cc.h>
#include <soc.h>
#include <c6x.h>

#include "main.h"
#include "stim_queue.h"
#include "registers.h"
#include "irq.h"
#include "MEA21_lib.h"

int num_tr_cross[HS1_CHANNELS/2];
int last_tr_cross[HS1_CHANNELS/2];

int slowmode = 0;
Uint32 slowdown_factor = 1;
Uint32 counter = 0;

// use "#define USE_MAILBOX_IRQ" in global.h to enable this interrupt
// Word of advice, I never got this to consistantly work. Might be
// fundamentally broken, may just as well be programmer error.
interrupt void interrupt8(void)
{
}

// FPGA data available (do not use)
interrupt void interrupt4(void)
{
}

// I2C Interrupt
interrupt void interrupt5(void)
{
}

// DMA finished Interrupt
interrupt void interrupt6(void)
{
  run_stim_queue();

  // No idea.
  CSL_Edma3ccRegsOvly edma3ccRegs = (CSL_Edma3ccRegsOvly)CSL_EDMA3CC_0_REGS;
  CSL_FINST(edma3ccRegs->ICRH, EDMA3CC_ICRH_I52, CLEAR);
}


// mystery interrupt!
interrupt void interrupt7(void)
{
}

void set_slow_mode(int set_slowmode, int factor){
  slowdown_factor = (1 << factor);
  counter = 0;
  slowmode = set_slowmode;
}
