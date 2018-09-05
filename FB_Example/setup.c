#include "setup.h"
#include "registers.h"
#include "stim_queue.h"
#include "logger.h"


void reset_state();

/**
   TODO: Document me!
 */
void setup_trigger() {

  WRITE_REGISTER(TRIGGER_CTRL1, 0x1);

  // Sets the amounts of repeats per trigger
  WRITE_REGISTER(0x9190, 1); // Trigger 1 Repeats once
  WRITE_REGISTER(0x9194, 1); // Trigger 2 Repeats once
  WRITE_REGISTER(0x9198, 1); // Trigger 3 Repeats once


  //                              |2 |     |1 |     |0 |
  // 0x00020100 = 0b00000000 00000010 00000001 00000000
  // bits set: 8, 17
  WRITE_REGISTER(0x9104, 0x00020100); // DAC1 to Trigger1, DAC2 to Trigger2, DAC3 to Trigger3
  WRITE_REGISTER(0x9108, 0x00020100); // SBS1 to Trigger1, SBS2 to Trigger2, SBS3 to Trigger3

  /**
    From the mouth of madness:
    Setting bit 28 one segment will be set up automaticaly. Setting bit 29 the memory is subdivided into
    256 segments with interal logic.
  */
  WRITE_REGISTER(0x9200, nth_bit(28)); // Inititialze STG Memory, use only one segment
}


void reset_state(){
  reset_logger();
  reset_comms();
  setup_stim_queue();
}

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

  setup_trigger();

  /* int StimAmplitude = 100;     // in units of 0.571 mV. 100*0.571 = 22.84 mV */
  /* int StimPeriod    = FIRING_PERIOD; */
  /* int StimStepsize  = 4;      // step size (resolution) in units of 0.571. */
  /* UploadSine(DAC_PAIR_1, StimAmplitude, StimPeriod, 1, StimStepsize); */




  // Upload sines to all 3 SBS/stim pairs
  // calls god-awful MCS code
  /* UploadSine(DAC_PAIR_1, StimAmplitude, StimPeriod, 1, StimStepsize); */
  /* UploadSine(DAC_PAIR_2, StimAmplitude, StimPeriod, 1, StimStepsize); */
  /* UploadSine(DAC_PAIR_3, StimAmplitude, StimPeriod, 1, StimStepsize); */

  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////


  WRITE_REGISTER(DSP_INDATA_CTRL, DSPINDATACTRL_VALUE);  // Enable Irq and HS1 Data
}
