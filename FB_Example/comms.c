#include "comms.h"
#include "MEA21_lib.h"
#include "registers.h"
#include "stim_queue.h"
#include "simple_stim.h"
#include "logger.h"

#define  DUMP               1
#define  RESET              2
#define  STIM_REQUEST       3
#define  STIM_DEBUG         4
#define  START_STIM_QUEUE   5
#define  STOP_STIM_QUEUE    6
#define  SLOW_MODE          7
#define  STIM_GROUP_REQUEST 8
#define  ENABLE_STIM_GROUP  9

#define  ENABLE_SIMPLE_STIM 10
#define  DISABLE_SIMPLE_STIM 11

void execute_instruction();
void signal_slave_idx();
void get_master_idx();
void write_state_to_debug();

void handle_start_sq();
void handle_stop_sq();
void handle_stim_req();
void handle_reset();
void handle_stim_group_req();
void handle_slowmode();
void handle_sg_toggle();

Uint32 top_instruction_id = 0;

// should be in terms of mod 2
int check_next_instruction(){
  Uint32 master_idx = READ_REGISTER(MASTER_INSTRUCTION_ID);
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
  Uint32 op_type = READ_REGISTER(INSTRUCTION_TYPE);

  MEAME_log(4, COMMS_INSTRUCTION_RECEIVED, op_type, top_instruction_id);

  switch(op_type)
    {
    case STIM_REQUEST :
      handle_stim_req();
      break;

    case START_STIM_QUEUE :
      handle_start_sq();
      break;

    case STOP_STIM_QUEUE :
      handle_stop_sq();
      break;

    case SLOW_MODE :
      handle_slowmode();
      break;

    case STIM_GROUP_REQUEST :
      handle_stim_group_req();
      break;

    case ENABLE_STIM_GROUP :
      handle_sg_toggle();
      break;

    case RESET :
      handle_reset();
      break;
  }
}


void handle_reset(){

}


// Stimulus request has been issued
void handle_stim_req(){
  Uint32 group_idx = READ_REGISTER(STIM_QUEUE_GROUP);
  Uint32 period    = READ_REGISTER(STIM_QUEUE_PERIOD);
  Uint32 elec0     = READ_REGISTER(STIM_QUEUE_ELEC0);
  Uint32 elec1     = READ_REGISTER(STIM_QUEUE_ELEC1);

  read_stim_request(group_idx, period, elec0, elec1);
}

// Stimulus group req issued
void handle_stim_group_req(){
  Uint32 group_idx = READ_REGISTER(STIM_QUEUE_GROUP);
  Uint32 period    = READ_REGISTER(STIM_QUEUE_PERIOD);

  read_stim_group_request(group_idx, period);
}

void handle_stop_sq(){
  set_stim_queue_running(HALTED);
}
void handle_start_sq(){
  set_stim_queue_running(RUNNING);
}


// factor is a power of 2
void handle_slowmode(){
  Uint32 factor = READ_REGISTER(SLOW_MODE_FACTOR);
  if(factor > 1){
    set_slow_mode(1, factor);
  }
  else{
    set_slow_mode(0, 1);
  }
}

void handle_sg_toggle(){
  Uint32 idx = READ_REGISTER(STIM_QUEUE_TOGGLE_SG);
  Uint32 status = READ_REGISTER(STIM_QUEUE_TOGGLE_VAL);
  toggle_stim_queue(idx, status);
}

void handle_simple_enable(){
  enable_simple_stim();
}
void handle_simple_disable(){
  disable_simple_stim();
}

void reset_comms(Uint32 DAC_idx){
  int ii;
  for(ii = 0; ii < 0x1FFC/4; ii++){
    WRITE_REGISTER(MAIL_BASE + (ii*4), 0x0);
  }
}
