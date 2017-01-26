#include "stimpack.h"
#include "registers.h"

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
  ////////////////////////////////////////
  ////////////////////////////////////////

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

void configure_electrodes(stimulus_group* stimulus_group,
                          Uint32 stimulus_enable_mask[];
                          Uint32 DAC_select_mask[];
                          Uint32 electrode_configuration_mask[];)
{
  int enable = 1;
  int mux = stimulus_group.DAC;
  int config = 0;

  int electrode_address;

  for (electrode_address = 0; electrode_address < stimulus_group->n_electrodes; electrode_address++)
    {
      electrode = stimulus_group->electrodes[electrode_address];

      stimulus_enable_mask[electrode/30]         |=   (enable << (electrode%30));
      DAC_select_mask[electrode/15]              |=   (mux << 2*(electrode%15));
      electrode_configuration_mask[electrode/15] |=   (config << 2*(electrode%15));
    }

  return
}



void update(stimulus_group stimulus_group){

  Uint32 stimulus_enable_mask[2];
  Uint32 DAC_select_mask[4];
  Uint32 electrode_configuration_mask[4];

  configure_electrodes(&stimulus_group,
                       stimulus_enable_mask,
                       DAC_select_mask,
                       electrode_configuration_mask);

  int ii;
  for (ii = 0; ii < 2; ii++)
    {
      WRITE_REGISTER( (ELECTRODE_ENABLE1+ii*4), stimulus_enable_mask[ii]);
    }

  for (ii = 0; ii < 4; ii++)
    {
      WRITE_REGISTER( (ELECTRODE_DAC_SEL1 + ii*4), DAC_select_mask[ii]);  // Select DAC 1 for Stimulation Electrodes
      WRITE_REGISTER( (ELECTRODE_MODE1 + ii*4), electrode_configuration_mask[ii]); // Configure Stimulation Electrodes to Listen to Sideband 1
    }

  WRITE_REGISTER(0x0218, segment << 16);  // select segment for trigger 1
  WRITE_REGISTER(0x0214, 0x00010001);     // Start Trigger 1
  segment = 1 - segment; // alternate between segment 0 and 1

}



stimulus_request read_stim_req(void)
{
  stimulus_request r;
  r.frequency = READ_REGISTER(STIM_FREQ_REQ);
  r.electrode = READ_REGISTER(STIM_FREQ_REQ);
  r.DAC_group = READ_REGISTER(STIM_FREQ_REQ);
  return r;
}
