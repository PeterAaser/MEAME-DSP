#include "MEA21_lib.h"

void run_stim_queue();
void setup_stim_queue();

void read_stim_request(Uint32 group_idx,
                       Uint32 period,
                       Uint32 elec1,
                       Uint32 elec2);

void set_stim_queue_running(Uint32 running);

void reset_comms();

// what the fuck dude
#define RUNNING 1
#define HALTED  2
