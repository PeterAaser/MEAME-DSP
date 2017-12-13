#include "MEA21_lib.h"



// the followign two lines create a variable in the external RAM
unsigned int ddr_data[1000000]; 
#pragma DATA_SECTION (ddr_data, ".dataddr");


Int32 adc_intern[TOTAL_ANALOG_CHANNELS];
#pragma DATA_ALIGN(adc_intern, 8);


int threshold;
int deadtime;

// commented out by me
/* Uint32 StimulusEnable[2]; */
/* Uint32 DAC_select[4]; */
/* Uint32 elec_config[4]; */
