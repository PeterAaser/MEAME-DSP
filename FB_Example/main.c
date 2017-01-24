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
      WRITE_REGISTER(0x002C, 0x700 + 1*value);
      for (i = 0; i < 100000; i++);
      value = 1 - value; // switch on/off
    }
}

void stimPack(Uint32 update){

  static int stimuliPeriod = 50000/4;
  static int stimuliCounter = 10;
  static int segment = 0;

  if(update){
    stimuliPeriod = update;
    if(stimuliCounter > stimuliPeriod){
      stimuliCounter = stimuliPeriod - 1;
    }
    return;
  }

  if(stimuliCounter >= stimuliPeriod){
    stimuliCounter = 0;

    int enable;
    int mux;
    int config;

    StimulusEnable[0] = 0;
    StimulusEnable[1] = 0;

    DAC_select[0] = 0;
    DAC_select[1] = 0;
    DAC_select[2] = 0;
    DAC_select[3] = 0;

    elec_config[0] = 0;
    elec_config[1] = 0;
    elec_config[2] = 0;
    elec_config[3] = 0;


    int electrode;
    for (electrode = 0; electrode < HS1_CHANNELS/4; electrode++) // HS1_CHANNELS <- 120
      {

        enable = 1;
        mux = 1; // Stimulation Source is DAC 1
        config = 0; // Use Sidestream 1 for Stimulation Switch

        StimulusEnable[electrode/30] |= (enable << electrode%30);
        DAC_select[electrode/15] |= (mux << 2*(electrode%15));
        elec_config[electrode/15] |= (config << 2*(electrode%15));
      }

    for (electrode = 0; electrode < 2; electrode++)
      {
        WRITE_REGISTER(0x9158+electrode*4, StimulusEnable[electrode]); // Enable Stimulation on STG
      }

    for (electrode = 0; electrode < 4; electrode++)
      {
        WRITE_REGISTER(0x9160+electrode*4, DAC_select[electrode]);  // Select DAC 1 for Stimulation Electrodes
        WRITE_REGISTER(0x9120+electrode*4, elec_config[electrode]); // Configure Stimulation Electrodes to Listen to Sideband 1
      }

    WRITE_REGISTER(0x0218, segment << 16);  // select segment for trigger 1
    WRITE_REGISTER(0x0214, 0x00010001);     // Start Trigger 1
    segment = 1 - segment; // alternate between segment 0 and 1

  }
  else {
    stimuliCounter++;
  }
}
