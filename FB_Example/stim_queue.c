#include "stim_API.h"
#include "stim_queue.h"
#include "MEA21_lib.h"
#include "registers.h"
#include "util.h"
#include "logger.h"
#include "stimulus.h"
#include "errors.h"

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

  if(step % 4096){
    Address step_counter;
    step_counter.v = STEP_COUNTER;
    increment_register(step_counter);
  }

  step++;
}


void run_group(int group_idx){
  int should_trigger = ((stim_reqs[group_idx].active) &&
                        (step == stim_reqs[group_idx].next_firing_timestep));

  if(should_trigger){

    // In an electrode has period 0 it is an error, however, in order to allow for the
    // case where an electrode is toggled befor the period is set, we allow an electrode
    // to stay active so that it can stimulate once it gets a valid period
    if(stim_reqs[group_idx].period == 0){
      // stim_reqs[group_idx].active = 0;
      raise_zero_period_trigger(group_idx, __LINE__);
      return;
    }

    Address shot_counter;
    shot_counter.v = SHOTS_FIRED;
    increment_register(shot_counter);

    manual_trigger(group_idx);
    stim_reqs[group_idx].next_firing_timestep += stim_reqs[group_idx].period;
  }
}


void read_stim_group_request(int group_idx, int next_period){
  if(!in_range(group_idx, 0, 2)){
    raise_read_request_not_in_range(group_idx, __LINE__);
    return;
  }

  int period_diff = next_period - stim_reqs[group_idx].period;
  stim_reqs[group_idx].next_firing_timestep += period_diff;
  if(stim_reqs[group_idx].next_firing_timestep <= step){
    stim_reqs[group_idx].next_firing_timestep = step + 1;
  }
  stim_reqs[group_idx].period = next_period;
}


void set_stim_queue_state(int state){
  mode = state;
}


void toggle_stim_group(int group_idx, int status){

  if(!in_range(group_idx, 0, 2)){
    raise_read_request_not_in_range(group_idx, __LINE__);
  }

  stim_reqs[group_idx].active = status;

  // If the new setting is the same as the old one nothing happens
  if(status == stim_reqs[group_idx].active)
    return;

  if(status){

    // Ensure that no electrode with period 0 is activated
    if(stim_reqs[group_idx].period == 0){
      stim_reqs[group_idx].active = 0;
      raise_zero_period_trigger(group_idx, __LINE__);
      return;
    }

    // After being toggled, the electrode should fire immediately
    stim_reqs[group_idx].next_firing_timestep = step + 1;
  }

  /* WRITE_REGISTER(DEBUG4, stim_reqs[group_idx].active); */
}


void setup_stim_queue(){
  int ii;
  for(ii = 0; ii < 3; ii++){
    stim_reqs[ii].active = 0;
    stim_reqs[ii].period = 0;
    stim_reqs[ii].next_firing_timestep = 0;
  }
}


io_void write_sq_state(){
  Address sq_debug_addr;
  int ii;
  sq_debug_addr.v = STIM_REQ1_ACTIVE;
  for(ii = 0; ii < 3; ii++){
    write_register_int(sq_debug_addr, stim_reqs[ii].active);
    sq_debug_addr.v += 4;
    write_register_int(sq_debug_addr, stim_reqs[ii].period);
    sq_debug_addr.v += 4;
    write_register_int(sq_debug_addr, stim_reqs[ii].next_firing_timestep);
    sq_debug_addr.v += 4;
  }
}
