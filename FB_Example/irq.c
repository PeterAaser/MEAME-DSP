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

// use "#define USE_MAILBOX_IRQ" in global.h to enable this interrupt
// Word of advice, I never got this to consistantly work. Might be
// fundamentally broken, may just as well be programmer error.
interrupt void interrupt8(void)
{
  // There used to be a lot of code here. Now it's gone
}

// FPGA data available (do not use)
interrupt void interrupt4(void)
{
}

// I2C Interrupt
interrupt void interrupt5(void)
{
  //handle_i2c_commands();
}

// DMA finished Interrupt
interrupt void interrupt6(void)
{
  run_stim_queue();


  // No idea.
  CSL_Edma3ccRegsOvly edma3ccRegs = (CSL_Edma3ccRegsOvly)CSL_EDMA3CC_0_REGS;
  CSL_FINST(edma3ccRegs->ICRH, EDMA3CC_ICRH_I52, CLEAR);
}


// timer irq
interrupt void interrupt7(void)
{
  /* static int led = 0; */
  /* CSL_FINS(gpioRegs->OUT_DATA, GPIO_OUT_DATA_OUT2, led); // LED */
  /* led = 1 - led; */
}
