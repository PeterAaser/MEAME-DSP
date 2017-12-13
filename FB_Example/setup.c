#include "setup.h"
#include "registers.h"
// #include "stimpack.h"
#include "stim_queue.h"
#include "logger.h"


/**
   TODO: Document me!
 */
void setup_trigger() {
  // TODO: What the fuck is a trigger packet??
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

  // I don't think these are necessary.
  WRITE_REGISTER(0x9190, 1); // Trigger 1 Repeat
  WRITE_REGISTER(0x9194, 1); // Trigger 2 Repeat
  WRITE_REGISTER(0x9198, 1); // Trigger 3 Repeat

  // 0x00020100 = 0b00000000 00000010 00000001 00000000
  // bits set: 8, 17
  WRITE_REGISTER(0x9104, 0x00020100); // DAC1 to Trigger1, DAC2 to Trigger2, DAC3 to Trigger3
  WRITE_REGISTER(0x9108, 0x00020100); // SBS1 to Trigger1, SBS2 to Trigger2, SBS3 to Trigger3
}


void reset_state()
{
  reset_logger();
  reset_comms();
}

#define SINGLE_SEGMENT 0x10000000

#define DAC_PAIR_1 0
#define DAC_PAIR_2 2
#define DAC_PAIR_3 4

void setup()
{
  reset_state();

  // With blanking turned off the pulse from an electrode is visible on other
  // sites, useful during development.
  WRITE_REGISTER(BLANKING_EN1, 0x0);
  WRITE_REGISTER(BLANKING_EN2, 0x0);

  setup_stim_queue();

  int StimAmplitude = 300;     // in units of 0.571 mV. 40*0.571 = 22.84 mV
  int StimPeriod    = FIRING_PERIOD;
  int StimStepsize  = 4;      // step size (resolution) in units of 0.571.


  //  Set bit 28 to 1
  WRITE_REGISTER(0x9200, SINGLE_SEGMENT); // Inititialze STG Memory, use only one segment

  // Upload sines to all 3 SBS/stim pairs
  // calls god-awful MCS code

  UploadSine(DAC_PAIR_1, StimAmplitude, StimPeriod, 1, StimStepsize);
  UploadSine(DAC_PAIR_2, StimAmplitude, StimPeriod, 1, StimStepsize);
  UploadSine(DAC_PAIR_3, StimAmplitude, StimPeriod, 1, StimStepsize);


  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////

  setup_trigger();

  WRITE_REGISTER(DSP_INDATA_CTRL, DSPINDATACTRL_VALUE);  // Enable Irq and HS1 Data
}
