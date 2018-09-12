#include "stim_API.h"
#include "stim_queue.h"
#include "MEA21_lib.h"
#include "registers.h"
#include "util.h"
#include "logger.h"
#include "stimulus.h"

#define IDLE   0
#define PRIMED 1
#define FIRING 2

#define LOGGING

int mode = HALTED;

Stimulus_request stim_reqs[3];

// takes 12 hours to roll over at 20µs tick rate.
// More likely to introduce a bug failsafing this than just letting it be.
int step = 0;

void run_group(int group_idx);


// Maintain stim qs
io_void run_stim_queue(){
  if(!(mode == RUNNING))
    return;

  int ii;
  for(ii = 0; ii < 3; ii++){
    run_group(ii);
  }

  step++;
}


void run_group(int group_idx){
  int should_trigger = ((stim_reqs[group_idx].active) &&
                        (step == stim_reqs[group_idx].next_firing_timestep));

  if(should_trigger){

    // just for safety
    if(stim_reqs[group_idx].period == 0){
      stim_reqs[group_idx].active = 0;
      return;
    }

    manual_trigger(group_idx);
    stim_reqs[group_idx].next_firing_timestep += stim_reqs[group_idx].period;
  }
}


void read_stim_group_request(int group_idx, int next_period){
  if(!in_range(group_idx, 0, 2)){
    return;
  }

  int period_diff = next_period - stim_reqs[group_idx].period;
  stim_reqs[group_idx].next_firing_timestep += period_diff;
  if(stim_reqs[group_idx].next_firing_timestep <= step){
    stim_reqs[group_idx].next_firing_timestep = step + 1;
  }
}


void set_stim_queue_state(int state){
  mode = state;
}


void toggle_stim_group(int group_idx, int status){

  if(!in_range(group_idx, 0, 2)){
    return;
  }

  stim_reqs[group_idx].active = status;

  if(status){

    // just for safety
    if(stim_reqs[group_idx].period == 0){
      stim_reqs[group_idx].active = 0;
      return;
    }

    stim_reqs[group_idx].next_firing_timestep = stim_reqs[group_idx].period + step;
  }
}


void setup_stim_queue(){
  int ii;
  for(ii = 0; ii < 3; ii++){
    stim_reqs[ii].active = 0;
    stim_reqs[ii].period = 0;
    stim_reqs[ii].next_firing_timestep = 0;
  }
}
