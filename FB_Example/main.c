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
#include "version.h"
#include "MEA21_lib.h"

char dsp_version[] = "(>)"SW_STRING"(<)";


CSL_GpioRegsOvly gpioRegs = (CSL_GpioRegsOvly)CSL_GPIO_0_REGS;

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

void main()
{
  static int count = 0;
  int count_test1;
  int count_test2;

  static int value = 0;
  volatile int i;

  int StimAmplitude;
  int StimPeriod;
  int StimRepeats;
  int StimStepsize;

  MEA21_init();

// Communication betwen USB and DSP via i2c
  //init_i2c();
  //IER |= 0x20;  // enable CPUINT5 (i2c)

  WRITE_REGISTER(DSP_INDATA_CTRL, DSPINDATACTRL_VALUE);  // Enable Irq and HS1 Data

#ifdef USE_SIMULATOR
  timer_setup();
  timer_setperiod(1000);
  IER |= 0x80;  // enable CPUINT7 (timer)
#else
  timer_setup();
  timer_setperiod(13653332); // 10 Hz timer frequency, blink LED with 5 Hz
  IER |= 0x80;  // enable CPUINT7 (timer)
#endif

  WRITE_REGISTER(0x318, 0x1);  // set AUX 1 as output

  threshold = READ_REGISTER(0x1000);
  deadtime  = READ_REGISTER(0x1004);

  StimAmplitude = READ_REGISTER(0x1008);
  StimPeriod    = READ_REGISTER(0x100c);
  StimRepeats   = READ_REGISTER(0x1010);
  StimStepsize  = READ_REGISTER(0x1014);

//	WRITE_REGISTER(0x9200, 0x10000000; // Inititialze STG Memory, use only one segment
  WRITE_REGISTER(0x9200, 0x20000000); // Inititialze STG Memory, use 256 segments
#ifndef USE_SIMULATOR
  while (READ_REGISTER(0x9200) & 0x30000000); // wait for segment initialization to finish
#endif

  WRITE_REGISTER(0x0310, 0x0); // set AUX 1 to value 0
  WRITE_REGISTER(0x0310, 0x1); // set AUX 1 to value one

  SetSegment(0, 0); // select Segment 0 for DAC 1
  SetSegment(1, 0); // select Segment 0 for Sideband 1
  UploadSine(0, StimAmplitude, StimPeriod, StimRepeats, StimStepsize);
  SetSegment(0, 1); // select Segment 1 for DAC 1
  SetSegment(1, 1); // select Segment 0 for Sideband 1
  UploadSine(0, StimAmplitude/2, StimPeriod, StimRepeats, StimStepsize);

  WRITE_REGISTER(0x0310, 0x0); // set AUX 1 to value 0

  SetupTrigger();

  while(1)
  {
    WRITE_REGISTER(0x002C, 0x700 + 1*value);
    for (i = 0; i < 100000; i++);
    value = 1 - value; // switch on/off
  }
}

void UploadSine(int Channel, int Amplitude, int Period, int Repeats, int Stepsize)
{
  int yold = 0;
  int duration = 0;
  int datapoints = 0;
  volatile int i;
  int y;
  int vectors_used;

  vectors_used = 0;

  ClearChannel(Channel);
  for (i = 0; i < Period; i++)
    {
      y = Amplitude * sin((((double)i)/Period)*2*3.1415);
      //		y = -(Amplitude *i)/Period;

      if (abs(y - yold) > Stepsize)
        {
          vectors_used += AddDataPoint(Channel, duration, yold+0x8000);
          datapoints++;
          yold = y;
          duration = 1; // 20 us
        }
      else
        {
          duration++;
        }
    }
  vectors_used += AddDataPoint(Channel, duration, yold+0x8000);
  AddLoop(Channel, vectors_used, Repeats);

  // Create Sideband Information
  ClearChannel(Channel+1);
  vectors_used = 0;
  vectors_used += AddDataPoint(Channel+1, Period, 0x0019);
  AddLoop(Channel+1, vectors_used, Repeats);
  //	AddDataPoint(Channel+1, 10, 0x0009); // keep Electrode connected to ground after stimulation
}

void AddLoop(int Channel, int Vectors, int Repeats)
{
  Uint32 ChannelReg;
  Uint32 LoopVector;

  ChannelReg = 0x9f20 + Channel*4;

  if (Repeats > 1)
    {
      LoopVector = 0x10000000 | (Repeats << 16) | Vectors;
      WRITE_REGISTER(ChannelReg, LoopVector);
    }
}

void SetSegment(int Channel, int Segment)
{
  Uint32 SegmentReg = 0x9200 + Channel*0x20;
  WRITE_REGISTER(SegmentReg, Segment);  // Any write to this register clears the Channeldata
}

void ClearChannel(int Channel)
{
  Uint32 ClearReg = 0x920c + Channel*0x20;
  WRITE_REGISTER(ClearReg, 0);      // Any write to this register clears the Channeldata
}

int AddDataPoint(int Channel, int duration, int value)
{
  int vectors_used = 0;
  int	Vector;
  Uint32 ChannelReg = 0x9f20 + Channel*4;

  if (duration > 1000)
    {
      Vector = 0x04000000 | (((duration / 1000) - 1) << 16) | (value & 0xffff);
      WRITE_REGISTER(ChannelReg, Vector);  // Write Datapoint to STG Memory
      duration %= 1000;
      vectors_used++;
    }

  if (duration > 0)
    {
      Vector = ((duration - 1) << 16) | (value & 0xffff);
      WRITE_REGISTER(ChannelReg, Vector);  // Write Datapoint to STG Memory
      vectors_used++;
    }

  return vectors_used;
}

void SetupTrigger()
{
  WRITE_REGISTER(0x0200, 0x1);  // Enable Trigger Packets
  WRITE_REGISTER(0x0204, 0x0);  // Setup Trigger
  WRITE_REGISTER(0x0208, 0x0);  // Setup Trigger

  WRITE_REGISTER(0x020c, 0x0);  // Setup Trigger
  WRITE_REGISTER(0x0210, 0x0);  // Setup Trigger

  WRITE_REGISTER(0x0218, 0x0);  // Setup Trigger
  WRITE_REGISTER(0x021c, 0x0);  // Setup Trigger
  WRITE_REGISTER(0x0220, 0x0);  // Setup Trigger
  WRITE_REGISTER(0x0224, 0x0);  // Setup Trigger
  WRITE_REGISTER(0x0228, 0x0);  // Setup Trigger
  WRITE_REGISTER(0x022c, 0x0);  // Setup Trigger

  WRITE_REGISTER(0x9190, 1); // Trigger 1 Repeat
  WRITE_REGISTER(0x9194, 1); // Trigger 2 Repeat
  WRITE_REGISTER(0x9198, 1); // Trigger 3 Repeat

  WRITE_REGISTER(0x9104, 0x00020100); // DAC1 to Trigger1, DAC2 to Trigger2, DAC3 to Trigger3
  WRITE_REGISTER(0x9108, 0x00020100); // SBS1 to Trigger1, SBS2 to Trigger2, SBS3 to Trigger3
}

// Mask: Set to 1 for each bit which is to be modified
// Value: New Value for modified bits
void ModifyRegister(Uint32 reg, Uint32 Mask, Uint32 Value)
{
  Uint32 Temp;

  Temp = READ_REGISTER(reg);
  Temp &= ~Mask;
  Temp |= (Value & Mask);
  WRITE_REGISTER(reg, Temp);
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
        //			WRITE_REGISTER(0x8140+i*4, StimulusEnable[i]); // Enable hard blanking for Stimulation Electrodes
      }

    for (electrode = 0; electrode < 4; electrode++)
      {
        WRITE_REGISTER(0x9160+electrode*4, DAC_select[electrode]);  // Select DAC 1 for Stimulation Electrodes
        WRITE_REGISTER(0x9120+electrode*4, elec_config[electrode]); // Configure Stimulation Electrodes to Listen to Sideband 1
      }

    WRITE_REGISTER(0x0218, segment << 16);  // select segment for trigger 1
    WRITE_REGISTER(0x0214, 0x00010001);     // Start Trigger 1
    segment = 1 - segment; // alternate between segment 0 and 1

    // analyze data
    // configure stim signal


  }
  else {
    stimuliCounter++;
  }

}
