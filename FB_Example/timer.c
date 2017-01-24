#include "MEA21_lib.h"
#include "timer.h"

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

#include <c6x.h>

void timer_setup()
{
  CSL_TmrRegsOvly tmr1Regs = (CSL_TmrRegsOvly)CSL_TMR_1_REGS;

  // clear TIM12 register
  CSL_FINST(tmr1Regs->TIMLO,TMR_TIMLO_TIMLO,RESETVAL);


  CSL_FINS(tmr1Regs->TCR, TMR_TCR_CLKSRC_LO, 0);

  // select 32 bit unchained mode and take the timer out of reset
  CSL_FINS(tmr1Regs->TGCR, TMR_TGCR_TIMMODE, 1);  // 32bit unchained
  CSL_FINST(tmr1Regs->TGCR, TMR_TGCR_TIMLORS, RESET_OFF);
}

void timer_setperiod(int period)
{
  CSL_TmrRegsOvly tmr1Regs = (CSL_TmrRegsOvly)CSL_TMR_1_REGS;

  CSL_FINST(tmr1Regs->TCR, TMR_TCR_ENAMODE_LO, DISABLE);
  CSL_FINS(tmr1Regs->PRDLO,TMR_PRDLO_PRDLO, period);
  CSL_FINST(tmr1Regs->TIMLO,TMR_TIMLO_TIMLO,RESETVAL);
  CSL_FINS(tmr1Regs->TCR, TMR_TCR_ENAMODE_LO, 2);  // continous mode
}
