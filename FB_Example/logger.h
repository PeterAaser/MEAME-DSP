#include "registers.h"
#include "util.h"
#include "MEA21_lib.h"
#include <stdarg.h>

#define MODE_SET 1
#define COMMS_INSTRUCTION_RECEIVED 10


void MEAME_log(int count,...);
void log_msg(int msg);
void reset_logger();
