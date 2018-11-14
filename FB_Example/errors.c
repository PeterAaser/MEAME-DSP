#define NO_ERROR             0
#define MANUAL_TRIGGER_ERROR 1
#define ILLEGAL_MODE         2
#define ZERO_PERIOD_TRIGGER  3
#define READ_REQ_OOB         4
#define XZIBIT_ERROR         5

#include "util.h"
#include <stdarg.h>
#include "registers.h"

io_void raise_error(int flag){
  Address debug_addr;
  debug_addr.v = ERROR_FLAG;
  write_register_int(debug_addr, flag);
}

io_void set_error_info(int count,...){
  va_list ap;
  int ii;
  int address = ERROR_START;

  va_start(ap, count);

  for(ii = 0; ii < count; ii++){
    if(address > ERROR_END){
      raise_error(XZIBIT_ERROR);
      return;
    }
    WRITE_REGISTER(address, va_arg(ap, int));
    address += 4;
  }
}

void raise_manual_trigger_error(int group){
  raise_error(MANUAL_TRIGGER_ERROR);
  set_error_info(group);
}

void raise_illegal_mode(int mode){
  raise_error(ILLEGAL_MODE);
  set_error_info(mode);
}

void raise_zero_period_trigger(int group_idx, int line){
  raise_error(ZERO_PERIOD_TRIGGER);
  /* set_error_info(group_idx, line); */
}

void raise_read_request_not_in_range(int group_idx, int line){
  raise_error(READ_REQ_OOB);
  /* set_error_info(group_idx, line); */
}
