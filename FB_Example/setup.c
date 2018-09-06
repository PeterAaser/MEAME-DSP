#include "setup.h"
#include "registers.h"
#include "stim_queue.h"
#include "logger.h"


void reset_state();

/**
   TODO: Document me!
 */
void setup_trigger() {

  Address trigger_ctrl, trigger1_repeat, trigger2_repeat, trigger3_repeat;
  trigger_ctrl.v = TRIGGER_CTRL;
  trigger1_repeat.v = 0x9190;
  trigger2_repeat.v = 0x9194;
  trigger3_repeat.v = 0x9198;

  write_register_int(trigger_ctrl, 1);

  // Sets the amounts of repeats per trigger
  write_register_int(trigger1_repeat, 1);
  write_register_int(trigger2_repeat, 1);
  write_register_int(trigger3_repeat, 1);

  // assign DAC1 to trigger1, DAC2 to trigger2 and DAC3 to trigger3
  Word trigger_settings; trigger_settings.v = 0;
  trigger_settings = set_field(trigger_settings, 16, 2, 2);
  trigger_settings = set_field(trigger_settings, 8,  2, 1);
  trigger_settings = set_field(trigger_settings, 0,  2, 0);

  Address DAC;
  Address SBS;
  DAC.v = 0x9104;
  SBS.v = 0x9108;

  write_register(DAC, trigger_settings);
  write_register(SBS, trigger_settings);

  /**
    From the mouth of madness:
    Setting bit 28 one segment will be set up automaticaly. Setting bit 29 the memory is subdivided into
    256 segments with interal logic.
  */
  Address mystery_register;
  mystery_register.v = 0x9200;
  write_register(mystery_register, nth_bit(28)); // Inititialze STG Memory, use only one segment
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
  Address blanking_en1, blanking_en2;
  blanking_en1.v = BLANKING_EN1;
  blanking_en2.v = BLANKING_EN2;
  write_register_int(blanking_en1, 0);
  write_register_int(blanking_en2, 0);

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
