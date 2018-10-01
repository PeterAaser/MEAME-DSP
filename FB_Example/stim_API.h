#ifndef SAPI_H
#define SAPI_H

#include "util.h"




/**
   We want all function calls to be safe (not side effecting wrt dsp memory)
   thus all functions except unsafe_commit only alters a config.
 */
typedef struct Electrode_config {
  Word enabled_electrodes[2];
  Word DAC_select[4];
  Word electrode_mode[4];
} Electrode_config;


/**
   Sets the trigger defaults. See inside for more info
   Should be run once during setup, not as a user call.
*/
io_void setup_triggers();


void disable_electrodes(Electrode_config* cfg);

/**
   Activates electrodes using and assigns DAC pairs.
   If called twice, previous definition is not reset, thus
   a full reset is necessary to change a single group twice.
 */
void configure_electrode_group(int group_idx,
                               int electrodes1,
                               int electrodes2,
                               Electrode_config* cfg);

/**
   Sets all electrodes to manual or auto mode. Please don't call it with
   anything else :3

   Should not be public?
 */
#define MANUAL 0
#define AUTO   3 //0b11
void setup_mode_select(int mode, Electrode_config* cfg);


io_void reset_stimulus();
io_void manual_trigger(int group);
io_void commit_config(Electrode_config* cfg);
io_void commit_config_debug(Electrode_config* cfg);

#endif // SAPI_H

