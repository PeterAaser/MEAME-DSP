#include "comms.h"
#include "MEA21_lib.h"
#include "registers.h"
#include "stim_queue.h"
#include "simple_stim.h"
#include "logger.h"
#include "stim_API.h"

// take a big shit I guess?
#define  DUMP 1

// NYI, just reset the device (uhh..)
#define  RESET 2

// Toggle electrodes on and off for a certain group
// Takes in two int32 bit fields
#define  CONFIGURE_ELECTRODE_GROUP 3

// Sets the electrode mode for ALL electrodes.
// Should be called from the setup function anyways, so safe to ignore
#define  SET_ELECTRODE_GROUP_MODE_MANUAL 4
#define  SET_ELECTRODE_GROUP_MODE_AUTO 5

#define  COMMIT_CONFIG 6

// Starts sending out triggers. Once running no configuration is allowed
#define  START_STIM_QUEUE 7
#define  STOP_STIM_QUEUE  8

// Sets the frequency of a given group
#define  SET_ELECTRODE_GROUP_PERIOD 9

// Toggles a given stim group from inactive to active
#define  ENABLE_STIM_GROUP  10
#define  DISABLE_STIM_GROUP 11

#define  COMMIT_CONFIG_DEBUG 12
#define  WRITE_SQ_DEBUG 13


Electrode_config electrode_cfg;


void execute_instruction();
void signal_slave_idx();
void get_master_idx();
void write_state_to_debug();


void handle_dump();
void handle_reset();
void handle_configure_electrode_group();
void handle_commit_config();
void handle_commit_config_debug();
void handle_set_electrode_group_mode(int mode);
void handle_sq_toggle(int state);
void handle_set_electrode_group_period();
void handle_stim_group_toggle(int state);
void handle_write_sq_debug();


Int top_instruction_id = 0;

// should be in terms of mod 2
int check_next_instruction(){
  Int master_idx = READ_REGISTER(MASTER_INSTRUCTION_ID);
  if(master_idx > top_instruction_id){
    return 1;
  }
  return 0;
}

void signal_instruction_complete(){
  top_instruction_id++;
  WRITE_REGISTER(SLAVE_INSTRUCTION_ID, top_instruction_id);
}

void execute_instructions(){

  // No instruction has been issued
  if( !(check_next_instruction() == 1) ){
    return;
  }

  execute_instruction();
  signal_instruction_complete();
}


void execute_instruction(){
  Int op_type = READ_REGISTER(INSTRUCTION_TYPE);

  MEAME_log(4, COMMS_INSTRUCTION_RECEIVED, op_type, top_instruction_id);

  switch(op_type)
    {
    case DUMP :
      break;

    case RESET :
      handle_reset();
      break;

    case CONFIGURE_ELECTRODE_GROUP :
      handle_configure_electrode_group();
      break;

    case SET_ELECTRODE_GROUP_MODE_AUTO :
      handle_set_electrode_group_mode(AUTO);
      break;

    case SET_ELECTRODE_GROUP_MODE_MANUAL :
      handle_set_electrode_group_mode(MANUAL);
      break;

    case COMMIT_CONFIG :
      handle_commit_config();
      break;

    case COMMIT_CONFIG_DEBUG :
      handle_commit_config_debug();
      break;

    case START_STIM_QUEUE :
      handle_sq_toggle(RUNNING);
      break;

    case STOP_STIM_QUEUE :
      handle_sq_toggle(HALTED);
      break;

    case SET_ELECTRODE_GROUP_PERIOD :
      handle_set_electrode_group_period();
      break;

    case ENABLE_STIM_GROUP :
      handle_stim_group_toggle(1);
      break;

    case DISABLE_STIM_GROUP :
      handle_stim_group_toggle(0);
      break;

    case WRITE_SQ_DEBUG :
      handle_write_sq_debug();
      break;
  }
}


void handle_reset(){
  electrode_cfg.enabled_electrodes[0].v = 0;
  electrode_cfg.enabled_electrodes[1].v = 0;
  electrode_cfg.DAC_select[0].v = 0;
  electrode_cfg.DAC_select[1].v = 0;
  electrode_cfg.DAC_select[2].v = 0;
  electrode_cfg.DAC_select[3].v = 0;
  electrode_cfg.electrode_mode[0].v = 0;
  electrode_cfg.electrode_mode[1].v = 0;
  electrode_cfg.electrode_mode[2].v = 0;
  electrode_cfg.electrode_mode[3].v = 0;
}


void handle_dump(){
}


// Stimulus request has been issued
void handle_configure_electrode_group(){
  Int group_idx = READ_REGISTER(STIM_QUEUE_GROUP);
  Int elec0     = READ_REGISTER(STIM_QUEUE_ELEC0);
  Int elec1     = READ_REGISTER(STIM_QUEUE_ELEC1);

  configure_electrode_group(group_idx, elec0, elec1, &electrode_cfg);
}


void handle_set_electrode_group_mode(int mode){
  setup_mode_select(mode, &electrode_cfg);

  MEAME_log(MODE_SET, mode,
            electrode_cfg.electrode_mode[0].v,
            electrode_cfg.electrode_mode[1].v,
            electrode_cfg.electrode_mode[2].v,
            electrode_cfg.electrode_mode[3].v
            );
}


// turns the entire stim queue on or off
void handle_sq_toggle(int state){
  set_stim_queue_state(state);
}


void handle_set_electrode_group_period(){
  int group_idx = READ_REGISTER(STIM_QUEUE_GROUP);
  int period    = READ_REGISTER(STIM_QUEUE_PERIOD);

  read_stim_group_request(group_idx, period);
}

// toggles a single stim group
void handle_stim_group_toggle(int state){
  int group_idx = READ_REGISTER(STIM_QUEUE_GROUP);
  toggle_stim_group(group_idx, state);
}


void reset_comms(Int DAC_idx){
  int ii;
  for(ii = 0; ii < (0x1FFC/4); ii++){
    WRITE_REGISTER(MAIL_BASE + (ii*4), 0x0);
  }
}


void handle_commit_config(){
  commit_config(&electrode_cfg);
}

void handle_commit_config_debug(){
  commit_config_debug(&electrode_cfg);
}

void handle_write_sq_debug(){
  write_sq_state();
}
