#include "stimpack.h"
#include "registers.h"
#include "util.h"

#define MANUAL 0
#define AUTO 3


// basic tick book-keeping
void update_electrode_group(stimulus_group* stimulus_group);

// aggregate configuration data
void configure_electrode_group(stimulus_group* stimulus_group,
                               Uint32 stimulus_enable_mask[],
                               Uint32 DAC_select_mask[],
                               Uint32 electrode_configuration_mask[]);

// aggregate configuration data for all groups, upload new config
void configure_electrodes(stimulus_group* stimulus_groups);

// fire main cannons.
void trigger_stimulus_group(stimulus_group* stimulus_group);
void fire_trigger(int trigger);



// "A static field is only visible in the file it is declared in"
static stimulus_group stimulus_groups[3];

void run_stimpack()
{

  int ii;
  for(ii = 0; ii < 3; ii++)
    {
      update_electrode_group(&stimulus_groups[ii]);
    }

  return;
}

void update_electrode_group(stimulus_group* stimulus_group)
{
  if(stimulus_group->tick >= stimulus_group->period)
    {
      stimulus_group->tick = 0;
      trigger_stimulus_group(stimulus_group);
      stimulus_group->fires++;
    }
  else
    {
      stimulus_group->tick++;
    }
}


void configure_electrode_group(stimulus_group* stimulus_group,
                               Uint32 stimulus_enable_mask[],
                               Uint32 DAC_select_mask[],
                               Uint32 electrode_configuration_mask[])
{
  // int mux = stimulus_group->DAC;
  // int config = 0;
  int mux = 1;
  int config = 0;

  int electrode_address;
  int ii;
  for (ii = 0; ii < 2; ii++)
    {
      for (electrode_address = 0; electrode_address < 30; electrode_address++)
        {
          if(get_bit32(stimulus_group->electrodes[ii], electrode_address))
            {
              int a = electrode_address + (ii * 30);
              DAC_select_mask[a/15]              |=   ( mux << ( (2 * a) % 30) );
              electrode_configuration_mask[a/15] |=   (config << ( (2 * a) % 30) );
            }
        }
    }


  stimulus_enable_mask[0] |= stimulus_group->electrodes[0];
  stimulus_enable_mask[1] |= stimulus_group->electrodes[1];

  return;
}


// Enables and configures electrodes
void configure_electrodes(stimulus_group stimulus_groups[])
{
  Uint32 stimulus_enable_mask[2] = {0};
  Uint32 DAC_select_mask[4] = {0};
  Uint32 electrode_configuration_mask[4] = {0};

  // For each electrode group get the configuration
  Uint32 ii;
  for (ii = 0; ii < 3; ii++)
    {

      stimulus_group group = stimulus_groups[ii];

      configure_electrode_group(&group,
                                stimulus_enable_mask,
                                DAC_select_mask,
                                electrode_configuration_mask);

      WRITE_REGISTER( (DEBUG_DAC_SEL11 + 4*ii), DAC_select_mask[0]);
      WRITE_REGISTER( (DEBUG_DAC_SEL12 + 4*ii), DAC_select_mask[1]);
      WRITE_REGISTER( (DEBUG_DAC_SEL13 + 4*ii), DAC_select_mask[2]);
      WRITE_REGISTER( (DEBUG_DAC_SEL14 + 4*ii), DAC_select_mask[3]);
    }


  write_segment( ELECTRODE_ENABLE, 2, stimulus_enable_mask);
  write_segment( ELECTRODE_DAC_SEL, 4, DAC_select_mask);
  write_segment( ELECTRODE_MODE, 4, electrode_configuration_mask);

  return;
}

void trigger_stimulus_group(stimulus_group* stimulus_group)
{
  // TODO this is wrong (but the method it calls is also wrong, and has been hardcoded to be "correct")
  fire_trigger(stimulus_group->DAC);
}


void fire_trigger(int trigger)
{
  static Uint32 triggers_fired = 0;
  triggers_fired++;

  // MCS code
  static int segment = 0;
  WRITE_REGISTER(0x0218, segment << 16);  // select segment for trigger 1
  WRITE_REGISTER(0x0214, 0x00010001);     // Start Trigger 1
  // segment = 1 - segment;

  WRITE_REGISTER( DEBUG10, triggers_fired );
}


int read_stim_request()
{
  Uint32 DAC = READ_REGISTER( DAC_ID );

  stimulus_groups[DAC].sample          = READ_REGISTER( SAMPLE );
  stimulus_groups[DAC].period          = READ_REGISTER( PERIOD );
  stimulus_groups[DAC].tick            = (stimulus_groups[DAC].tick >= stimulus_groups[DAC].period) ? 0 : stimulus_groups[DAC].tick;

  read_segment( ELECTRODES, 2, stimulus_groups[DAC].electrodes );

  configure_electrodes(stimulus_groups);

  return 1;
}

void dump_stim_group(Uint32 group)
{
  WRITE_REGISTER( DEBUG4, stimulus_groups[group - 1].DAC );
  WRITE_REGISTER( DEBUG5, stimulus_groups[group - 1].electrodes[0] );
  WRITE_REGISTER( DEBUG6, stimulus_groups[group - 1].electrodes[1] );
  WRITE_REGISTER( DEBUG7, stimulus_groups[group - 1].period );
  WRITE_REGISTER( DEBUG8, stimulus_groups[group - 1].tick );
  WRITE_REGISTER( DEBUG9, stimulus_groups[group - 1].sample );
  WRITE_REGISTER( DEBUG10, stimulus_groups[group - 1].fires );
}

void setup_stimpack()
{
  int ii = 0;
  for (ii = 0; ii < 3; ii++)
    {
      stimulus_groups[ii].DAC = ii;
      stimulus_groups[ii].electrodes[0] = 0;
      stimulus_groups[ii].electrodes[1] = 0;
      stimulus_groups[ii].period = 0x100000;
      stimulus_groups[ii].tick = 0;
      stimulus_groups[ii].sample = 0;
      stimulus_groups[ii].fires = 0;
    }
}
