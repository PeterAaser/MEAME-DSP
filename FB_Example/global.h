#ifndef GLOBAL_H_
#define GLOBAL_H_

// #define USE_SIMULATOR
#define INIT_IRQ
// #define USE_MAILBOX_IRQ

#include <csl.h>

#define I2C_BUFFER_SIZE 512

#define MONITOR_ARRAY   		256


#define STIMULUS_VECTOR_SIZE 	10
#define SENDDATA_PER_FRAME 		64

#define DOWNSAMPLE         	1
#define FRAMES_PER_LOOP    	1

#define INCLUDE_HS1_DATA
//#define INCLUDE_HS2_DATA
#define INCLUDE_IF_DATA
#define INCLUDE_HS1_FILTER_DATA
//#define INCLUDE_HS2_FILTER_DATA
//#define INCLUDE_DIGIO_DATA
//#define INCLUDE_STATUS_DATA

#define TOTAL_ANALOG_CHANNELS 256

extern Int32 adc_intern[TOTAL_ANALOG_CHANNELS];

extern unsigned int ddr_data[1000000]; // this makes the variable visible to all .c files that include global.h

extern int threshold;
extern int deadtime;


extern Uint32 StimulusEnable[2];
extern Uint32 DAC_select[4];
extern Uint32 elec_config[4];



#endif /*GLOBAL_H_*/
