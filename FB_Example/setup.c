#include "comms.h"
#include "setup.h"
#include "registers.h"
#include "stim_queue.h"
#include "logger.h"
#include "stim_API.h"


void reset_state();

void reset_state(){
  reset_logger();
  reset_comms();
  setup_stim_queue();
}


// With blanking turned off the pulse from an electrode is visible on other
// sites, useful during development.
void setup_blanking(){
  Address blanking_en1, blanking_en2;
  blanking_en1.v = BLANKING_EN1;
  blanking_en2.v = BLANKING_EN2;
  write_register_int(blanking_en1, 0);
  write_register_int(blanking_en2, 0);
}


void setup()
{
  reset_state();

  setup_triggers();
  setup_blanking();

  /* int StimAmplitude = 100;     // in units of 0.571 mV. 100*0.571 = 22.84 mV */
  /* int StimPeriod    = FIRING_PERIOD; */
  /* int StimStepsize  = 4;      // step size (resolution) in units of 0.571. */
  /* UploadSine(DAC_PAIR_1, StimAmplitude, StimPeriod, 1, StimStepsize); */

  /* #define DAC_PAIR_1 0 */
  /* #define DAC_PAIR_2 2 */
  /* #define DAC_PAIR_3 4 */

  // Upload sines to all 3 SBS/stim pairs
  // calls god-awful MCS code
  /* UploadSine(DAC_PAIR_1, StimAmplitude, StimPeriod, 1, StimStepsize); */
  /* UploadSine(DAC_PAIR_2, StimAmplitude, StimPeriod, 1, StimStepsize); */
  /* UploadSine(DAC_PAIR_3, StimAmplitude, StimPeriod, 1, StimStepsize); */

  WRITE_REGISTER(DSP_INDATA_CTRL, DSPINDATACTRL_VALUE);  // Enable Irq and HS1 Data
}
