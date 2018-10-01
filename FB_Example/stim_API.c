#include "stim_API.h"
#include "stim_queue.h"
#include "MEA21_lib.h"
#include "registers.h"
#include "util.h"
#include "logger.h"
#include "stimulus.h"
#include "errors.h"


io_void manual_trigger(int group){
  if(!in_range(group, 0, 2)){
    raise_manual_trigger_error(group);
    return;
  }
  Address trigger_address;
  trigger_address.v = 0x0214;
  write_register_int(trigger_address, (1 << group));
}


void disable_electrodes(Electrode_config* cfg){
  int ii;
  for(ii = 0; ii < 2; ii++){
    cfg->enabled_electrodes[ii].v = 0;
  }
}


io_void reset_stimulus(){
  Address electrode_enable_start, electrode_dac_sel_start, electrode_mode_start;
  electrode_enable_start.v  = ELECTRODE_ENABLE;
  electrode_dac_sel_start.v = ELECTRODE_DAC_SEL;
  electrode_mode_start.v    = ELECTRODE_MODE;

  reset_segment(electrode_enable_start, 2);
  reset_segment(electrode_dac_sel_start, 4);
  reset_segment(electrode_mode_start, 4);
}


// Should be run once
io_void setup_triggers(){

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

  Address DAC, SBS;
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



// TODO verify that this does what it is supposed to.
// TODO document the differences in mode type
void setup_mode_select(int mode, Electrode_config* cfg){
  Word mode_fields;
  mode_fields.v = 0;
  int ii;

  if(!((mode == 0) || (mode == 3))){
    raise_illegal_mode(mode);
    return;
  }

  // create the correct word
  for(ii = 0; ii < 15; ii++){
    mode_fields = set_field(mode_fields, ii*2, 2, mode);
  }

  // apply to config
  for(ii = 0; ii < 4; ii++){
    cfg->electrode_mode[ii] = mode_fields;
  }
}


void configure_electrode_group(int group_idx,
                               int electrodes1,
                               int electrodes2,
                               Electrode_config* cfg){

  cfg->enabled_electrodes[0].v |= electrodes1;
  cfg->enabled_electrodes[1].v |= electrodes2;


  // should be optimized away
  int electrodes[2];
  electrodes[0] = electrodes1;
  electrodes[1] = electrodes2;

  // 00 = None, 01 = trigger 1 and so forth.
  int trigger_mux = group_idx + 1;

  // Assigns DAC trigger sources to group index for every enabled electrode
  int ii, kk;
  for(ii = 0; ii < 4; ii++){

    int electrode_index  = ii/2;
    int electrode_offset = (ii%2)*15;

    int dac_index = ii;

    for(kk = 0; kk < 15; kk++){

      int dac_offset = kk*2;
      int electrode_enabled = get_bit32(electrodes[electrode_index], kk+electrode_offset);

      if(electrode_enabled){
        cfg->DAC_select[dac_index] = set_field(cfg->DAC_select[dac_index], dac_offset, 2, trigger_mux);
      }
    }
  }
}


io_void __commit_config(Electrode_config* cfg, Address mode_address, Address enable_address, Address DAC_sel_address);

io_void commit_config(Electrode_config* cfg){

  Address mode_address, enable_address, DAC_sel_address;
  mode_address.v    = ELECTRODE_MODE;
  enable_address.v  = ELECTRODE_ENABLE;
  DAC_sel_address.v = ELECTRODE_DAC_SEL;

  __commit_config(cfg,
                mode_address,
                enable_address,
                DAC_sel_address);
}

io_void commit_config_debug(Electrode_config* cfg){

  Address mode_address, enable_address, DAC_sel_address;
  mode_address.v    = CFG_DEBUG_MODE0;
  enable_address.v  = CFG_DEBUG_ELEC0;
  DAC_sel_address.v = CFG_DEBUG_DAC0;

  __commit_config(cfg,
                mode_address,
                enable_address,
                DAC_sel_address);
}

io_void __commit_config(Electrode_config* cfg,
                       Address mode_address,
                       Address enable_address,
                       Address DAC_sel_address){

  // Commit electrode mode
  int ii;
  for(ii = 0; ii < 4; ii++){
    set_field_by_address(mode_address, 0, 30, cfg->electrode_mode[ii].v);
    mode_address.v += 4;
  }

  // Commit DAC select
  for(ii = 0; ii < 4; ii++){
    set_field_by_address(DAC_sel_address, 0, 30, cfg->DAC_select[ii].v);
    DAC_sel_address.v += 4;
  }

  // Commit electrode enable
  for(ii = 0; ii < 2; ii++){
    set_field_by_address(enable_address, 0, 30, cfg->enabled_electrodes[ii].v);
    enable_address.v += 4;
  }
}
