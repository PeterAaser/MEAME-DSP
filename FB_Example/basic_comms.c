/* #include "basic_comms.h" */
/* #include "MEA21_lib.h" */
/* #include "registers.h" */

/* // MEAME wants to read value at address */
/* #define  MEAME_READ_OP   1 */

/* // MEAME wants to write value to address */
/* #define  MEAME_WRITE_OP  2 */

/* // MEAME wants to dump data, clear, or something like that */
/* #define  DUMP            3 */

/* #define  RESET           4 */

/* volatile int comms_buffer_slave_idx = 0; */
/* volatile int comms_buffer_master_idx = 0; */
/* volatile int instructions_executed = 0; */

/* void execute_instruction(); */
/* void signal_slave_idx(); */
/* void get_master_idx(); */
/* void write_state_to_debug(); */


/* void get_master_idx(){ */
/*   comms_buffer_master_idx = READ_REGISTER(COMMS_BUFFER_MASTER_IDX); */
/* } */


/* void execute_instructions(){ */
/*   // If master instruction == slave idx we have no instruction to execute */
/*   if(comms_buffer_slave_idx == comms_buffer_master_idx){ */
/*     get_master_idx(); */
/*     return; */
/*   } */
/*   else{ */
/*     execute_instruction(); */
/*   } */

/*   comms_buffer_slave_idx++; */
/*   signal_slave_idx(); */
/* } */


/* void execute_instruction(){ */

/*   volatile int op_type = READ_REGISTER(COMMS_BUFFER_START); */
/*   volatile int op2     = READ_REGISTER(COMMS_BUFFER_START + 0x4); */
/*   volatile int op3     = READ_REGISTER(COMMS_BUFFER_START + 0x8); */

/*   write_state_to_debug(); */

/*   // MEAME wants to read a value. We read the value address from op2 */
/*   // and write the value of the address to the op2 register. */
/*   if(op_type == MEAME_READ_OP){ */
/*     volatile int read_value = READ_REGISTER(op2); */
/*     WRITE_REGISTER(COMMS_BUFFER_START + 0x8, read_value); */
/*     WRITE_REGISTER(DEBUG9, read_value); */
/*   } */


/*   // MEAME wants to write a value. op2 is the address, op3 is the value */
/*   // to be written to op2 */
/*   else if(op_type == MEAME_WRITE_OP){ */
/*     WRITE_REGISTER(op2, op3); */
/*   } */


/*   else if(op_type == DUMP){ */
/*     WRITE_REGISTER(DEBUG1, comms_buffer_slave_idx); */
/*     WRITE_REGISTER(DEBUG2, comms_buffer_master_idx); */
/*   } */

/*   else if(op_type == RESET){ */
/*     comms_buffer_slave_idx = 0; */
/*     comms_buffer_master_idx = 0; */

/*     WRITE_REGISTER(DEBUG1, 0x0); */
/*     WRITE_REGISTER(DEBUG2, 0x0); */
/*     WRITE_REGISTER(DEBUG3, 0x0); */
/*     WRITE_REGISTER(DEBUG4, 0x0); */
/*     WRITE_REGISTER(DEBUG5, 0x0); */
/*     WRITE_REGISTER(DEBUG6, 0x0); */
/*     WRITE_REGISTER(DEBUG7, 0x0); */
/*     WRITE_REGISTER(DEBUG8, 0x0); */
/*     WRITE_REGISTER(DEBUG9, 0x0); */

/*     WRITE_REGISTER(COMMS_BUFFER_SLAVE_IDX = 0); */
/*     WRITE_REGISTER(COMMS_BUFFER_MASTER_IDX = 0); */

/*     // TODO figure out why this is necessary */
/*     int count = 0; */
/*     int guard = n_instruction_registers; */
/*     for(count = 0; count < guard; count ++){ */
/*       WRITE_REGISTER((COMMS_BUFFER_START + (count*4)), 0x0); */
/*     } */
/*   } */

/*   instructions_executed++; */
/*   WRITE_REGISTER(DEBUG8, instructions_executed); */
/* } */

/* void signal_slave_idx(){ */
/*   WRITE_REGISTER(COMMS_BUFFER_SLAVE_IDX, comms_buffer_slave_idx); */
/* } */


/* void write_state_to_debug(){ */
/*   WRITE_REGISTER(COMMS1, comms_buffer_slave_idx); */
/*   WRITE_REGISTER(COMMS2, comms_buffer_master_idx); */
/*   WRITE_REGISTER(COMMS3, instructions_executed); */
/*   WRITE_REGISTER(COMMS4, last_op_type); */
/*   WRITE_REGISTER(COMMS5, last_op1); */
/*   WRITE_REGISTER(COMMS6, last_op2); */
/* } */
