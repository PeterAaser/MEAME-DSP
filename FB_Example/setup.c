#include "setup.h"

void SetupTrigger()
{

  // Some confusion here as these registers are mentioned twice
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

void setup()
{
  int StimAmplitude;
  int StimPeriod;
  int StimRepeats;
  int StimStepsize;

  WRITE_REGISTER(DSP_INDATA_CTRL, DSPINDATACTRL_VALUE);  // Enable Irq and HS1 Data

  timer_setup();
  timer_setperiod(13653332); // 10 Hz timer frequency, blink LED with 5 Hz
  IER |= 0x80;  // enable CPUINT7 (timer)

  WRITE_REGISTER(0x318, 0x1);  // set AUX 1 as output

  threshold = READ_REGISTER(0x1000);
  deadtime  = READ_REGISTER(0x1004);

  // TODO these are mailbox registers, figure out what's going on
  StimAmplitude = READ_REGISTER(0x1008);
  StimPeriod    = READ_REGISTER(0x100c);
  StimRepeats   = READ_REGISTER(0x1010);
  StimStepsize  = READ_REGISTER(0x1014);

//	WRITE_REGISTER(0x9200, 0x10000000; // Inititialze STG Memory, use only one segment
  WRITE_REGISTER(0x9200, 0x20000000); // Inititialze STG Memory, use 256 segments

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

}
