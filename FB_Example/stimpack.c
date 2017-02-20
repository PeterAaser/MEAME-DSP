#include "stimpack.h"
#include "registers.h"
#include "util.h"


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
  int mux = stimulus_group->DAC;
  int config = 0;

  int electrode_address;
  int ii;
  for (ii = 0; ii < 2; ii++)
    {
      for (electrode_address = 0; electrode_address < 30; electrode_address++)
        {
          if(get_bit32(stimulus_group->electrodes[ii], electrode_address))
            {
              int a = electrode_address*ii;
              DAC_select_mask[a/15]              |=   (mux << 2*(a%15));
              electrode_configuration_mask[a/15] |=   (config << 2*(a%15));
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

  WRITE_REGISTER( DEBUG5, 0x1 );

  // For each electrode group get the configuration
  Uint32 ii;
  for (ii = 0; ii < 3; ii++)
    {
      {
        // Instead of letting * and & have a tug of war.
        stimulus_group group = stimulus_groups[ii];

        WRITE_REGISTER( DEBUG6, ii );

        configure_electrode_group(&group,
                                  stimulus_enable_mask,
                                  DAC_select_mask,
                                  electrode_configuration_mask);
      }
    }

  WRITE_REGISTER( DEBUG7, 0x1);

  WRITE_REGISTER( DEBUG8, stimulus_enable_mask[0]);
  WRITE_REGISTER( DEBUG9, stimulus_enable_mask[1]);



  write_segment( ELECTRODE_ENABLE, 2, stimulus_enable_mask);
  write_segment( ELECTRODE_DAC_SEL, 4, DAC_select_mask);
  write_segment( ELECTRODE_MODE, 4, electrode_configuration_mask);

  return;
}

void trigger_stimulus_group(stimulus_group* stimulus_group)
{
  fire_trigger(stimulus_group->DAC);
}


void fire_trigger(int trigger)
{
  WRITE_REGISTER( MANUAL_TRIGGER, (1 << trigger));
}


int read_stim_request()
{
  Uint32 DAC = READ_REGISTER( DAC_ID );

  // stimulus_group* s = &(stimulus_groups[DAC]);

  stimulus_groups[DAC].sample          = READ_REGISTER( SAMPLE );
  stimulus_groups[DAC].period          = READ_REGISTER( PERIOD );
  stimulus_groups[DAC].tick            = (stimulus_groups[DAC].tick >= stimulus_groups[DAC].period) ? 0 : stimulus_groups[DAC].tick;

  read_segment( ELECTRODES, 2, stimulus_groups[DAC].electrodes );

  WRITE_REGISTER( DEBUG4, 1 );

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
}

void setup_stimpack()
{
  int ii = 0;
  for (ii = 0; ii < 3; ii++)
    {
      stimulus_groups[ii].DAC = ii;
      stimulus_groups[ii].electrodes[0] = 0;
      stimulus_groups[ii].electrodes[1] = 0;
      stimulus_groups[ii].period = 0x10000;
      stimulus_groups[ii].tick = 0;
      stimulus_groups[ii].sample = 0;
    }
}
