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
void configure_electrodes(stimulus_group** stimulus_groups);

// fire main cannons.
void trigger_stimulus_group(stimulus_group* stimulus_group);
void fire_trigger(int trigger);



// "A static field is only visible in the file it is declared in"
static stimulus_group stimulus_groups[3];



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
void configure_electrodes(stimulus_group** stimulus_groups)
{
  Uint32 stimulus_enable_mask[2] = {0};
  Uint32 DAC_select_mask[4] = {0};
  Uint32 electrode_configuration_mask[4] = {0};

  // For each electrode group get the configuration
  int ii;
  for (ii = 0; ii < 3; ii++)
    {
      if(stimulus_groups[ii] != NULL)
        {
          configure_electrode_group(stimulus_groups[ii],
                                    stimulus_enable_mask,
                                    DAC_select_mask,
                                    electrode_configuration_mask);
        }
    }

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


int read_stim_req()
{
  Uint32 DAC        = READ_REGISTER( DAC_ID );

  stimulus_group s = stimulus_groups[DAC];

  s.sample          = READ_REGISTER( SAMPLE );
  s.period          = READ_REGISTER( PERIOD );
  s.tick            = (s.tick >= s.period) ? 0 : s.tick;

  read_segment( ELECTRODES, 2, s.electrodes );

  return 1;
}





////////////////////////////////////////
////////////////////////////////////////
//// old shit
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
