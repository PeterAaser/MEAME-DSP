#include "comms.h"
#include "MEA21_lib.h"
#include "registers.h"
#include "stim_queue.h"
#include "logger.h"


// MEAME wants to read value at address
#define  MEAME_READ_OP   1

// MEAME wants to write value to address
#define  MEAME_WRITE_OP  2

// MEAME wants to dump data, clear, or something like that
// TODO: Not implemented?
#define  DUMP            3
#define  RESET           4
#define  STIM_REQUEST    5
#define  STIM_DEBUG      6

#define Q_SIZE 10
#define instructions_per_segment 3
#define n_instruction_registers Q_SIZE*instructions_per_segment

volatile Uint32 comms_buffer_slave_idx = 0;
volatile Uint32 comms_buffer_master_idx = 0;
volatile Uint32 instructions_executed = 0;

volatile Uint32 last_op_type = 0;
volatile Uint32 last_op1     = 0;
volatile Uint32 last_op2     = 0;

void execute_instruction();
void signal_slave_idx();
void get_master_idx();
void write_state_to_debug();

void handle_write_op(Uint32 op2, Uint32 op3);
void handle_reset();
void handle_read_op(Uint32 op2, Uint32 op_address);
void handle_stim_req(Uint32 group);


void get_master_idx(){
  comms_buffer_master_idx = READ_REGISTER(COMMS_BUFFER_MASTER_IDX);
}

void execute_instructions(){

  // If master instruction == slave idx we have no instruction to execute
  if(comms_buffer_slave_idx == comms_buffer_master_idx){
    get_master_idx();
    return;
  }
  else{
    execute_instruction();
  }

  // If this code path is executed then we already know the slave idx
  // instruction has been executed, so all that remains is setting next
  // instruction pointer
  if(comms_buffer_slave_idx < comms_buffer_master_idx){
    comms_buffer_slave_idx++;
  }
  else if(comms_buffer_slave_idx > comms_buffer_master_idx){
    if(comms_buffer_slave_idx == Q_SIZE - 1){
      comms_buffer_slave_idx = 0;
    }
    else{
      comms_buffer_slave_idx++;
    }
  }

  // Since we've executed an instruction we must signal this to MEAME
  signal_slave_idx();
}


void execute_instruction(){
  instructions_executed++;

  Uint32 op_address = COMMS_BUFFER_START + (instructions_per_segment*comms_buffer_slave_idx*4);

  volatile Uint32 op_type = READ_REGISTER(op_address);
  volatile Uint32 op2     = READ_REGISTER(op_address + 4);
  volatile Uint32 op3     = READ_REGISTER(op_address + 8);

  write_state_to_debug();

  // rly...
  last_op_type = op_type;
  last_op1 = op2;
  last_op2 = op3;

  if(op_type == MEAME_READ_OP)
    handle_read_op(op2, op_address);

  else if(op_type == MEAME_WRITE_OP)
    handle_write_op(op2, op3);

  else if(op_type == RESET)
    handle_reset();

  else if(op_type == STIM_REQUEST)
    handle_stim_req(op2);

  else if(op_type == STIM_DEBUG){}

  else {
    WRITE_REGISTER(ERROR, 0x2);
    WRITE_REGISTER(ERROR_VAL, op_type);
    WRITE_REGISTER(ERROR_OP1, op2);
    WRITE_REGISTER(ERROR_OP2, op3);
  }
}


// MEAME wants to write a value. op2 is the address, op3 is the value
// to be written to op2
void handle_write_op(Uint32 op2, Uint32 op3){
  // op2 : address to write to
  // op3 : value to be written
  if((op2 >= 0x1000) && (op2 <= 0x2000)){
    WRITE_REGISTER(op2, op3);
  }
  else{
    WRITE_REGISTER(ERROR, 0x1);
    WRITE_REGISTER(ERROR_VAL, op2);
  }
}


// MEAME wants to read a value. We read the value address from op2
// and write the value of the address to the op2 register.
void handle_read_op(Uint32 op2, Uint32 op_address){
  volatile Uint32 read_value = READ_REGISTER(op2);
  WRITE_REGISTER(op_address + 8, read_value);
}


void handle_reset(){
  comms_buffer_slave_idx = 0;
  comms_buffer_master_idx = 0;

  // TODO figure out why this is necessary
  Uint32 count = 0;
  Uint32 guard = n_instruction_registers;
  for(count = 0; count < guard; count ++){
    WRITE_REGISTER((COMMS_BUFFER_START + (count*4)), 0x0);
  }
}

// Stimulus request has been issued
void handle_stim_req(Uint32 group_idx){
  // MEAME_log(COMMS_READ_REQ, COMMS_READ_REQ, group_idx, group_idx);
  read_stim_request(group_idx);
}


void signal_slave_idx(){
  WRITE_REGISTER(COMMS_BUFFER_SLAVE_IDX, comms_buffer_slave_idx)
}

void write_state_to_debug(){
  WRITE_REGISTER(COMMS1, comms_buffer_slave_idx);
  WRITE_REGISTER(COMMS2, comms_buffer_master_idx);
  WRITE_REGISTER(COMMS3, instructions_executed);
  WRITE_REGISTER(COMMS4, last_op_type);
  WRITE_REGISTER(COMMS5, last_op1);
  WRITE_REGISTER(COMMS6, last_op2);
}

void reset_comms(){
  WRITE_REGISTER(COMMS_BUFFER_MASTER_IDX, 0);
  WRITE_REGISTER(COMMS_BUFFER_SLAVE_IDX, 0);
};
