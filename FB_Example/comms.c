#include "comms.h"
#include "MEA21_lib.h"
#include "registers.h"
#include "stim_queue.h"
#include "logger.h"

#define  DUMP               3
#define  RESET              4
#define  STIM_REQUEST       5
#define  STIM_DEBUG         6
#define  START_STIM_QUEUE   7
#define  STOP_STIM_QUEUE    8
#define  SLOW_MODE          9

void execute_instruction();
void signal_slave_idx();
void get_master_idx();
void write_state_to_debug();

void handle_start_sq();
void handle_stop_sq();
void handle_stim_req();
void handle_reset();

Uint32 top_instruction_id;

// should be in terms of mod 2
int update_and_check_next_instruction(){
  Uint32 next = (READ_REGISTER(INSTRUCTION_ID) + 1) % 2;
  int r = next > top_instruction_id;
  if(r){
    top_instruction_id = next;
  }
  return r;
}

void signal_instruction_complete(){
  top_instruction_id++;
  WRITE_REGISTER(INSTRUCTION_TYPE, top_instruction_id);
}

void execute_instructions(){

  // No instruction has been issued
  if(!update_and_check_next_instruction()){
    return;
  }

  execute_instruction();
  signal_instruction_complete();
}


void execute_instruction(){
  Uint32 op_type = READ_REGISTER(INSTRUCTION_TYPE);

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

void handle_stop_sq(){
  set_stim_queue_running(HALTED);
}
void handle_start_sq(){
  set_stim_queue_running(RUNNING);
}

void reset_comms(Uint32 DAC_idx){
  int ii;
  for(ii = 0; ii < 0x1FFC/4; ii++){
    WRITE_REGISTER(MAIL_BASE + (ii*4), 0x0);
  }
}
