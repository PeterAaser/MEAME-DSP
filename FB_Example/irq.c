#include <csl.h>
#include <cslr_tmr.h>
#include <cslr_gpio.h>
#include <cslr_chip.h>
#include <cslr_edma3cc.h>
#include <soc.h>
#include <c6x.h>

#include "main.h"
#include "irq.h"
#include "MEA21_lib.h"

int num_tr_cross[HS1_CHANNELS/2];
int last_tr_cross[HS1_CHANNELS/2];

Uint32 reg_written;
Uint32 reg_value;
Uint32 stim_req;

// Mailbox write interrupt
// use "#define USE_MAILBOX_IRQ" in global.h to enable this interrupt
interrupt void interrupt8(void)
{
  reg_written = READ_REGISTER(0x428);
  reg_value   = READ_REGISTER(0x1000 + reg_written);

  threshold = READ_REGISTER(0x1000);
  deadtime  = READ_REGISTER(0x1004);

  if(reg_written == 0x8){
    stim_req = READ_REGISTER(0x1008);
    WRITE_REGISTER(0x100C, stim_req);
    stimPack(stim_req);
  }
}

// FPGA data available (do not use)
interrupt void interrupt4(void)
{
}

// I2C Interrupt
interrupt void interrupt5(void)
{
  //handle_i2c_commands();
}

// DMA finished Interrupt
interrupt void interrupt6(void)
{


  stimPack(0);

  static int timestamp = 0; // exists only in this function but is created only once on the first function call (i.e. static)
  static int segment = 0;

  int i;
  CSL_Edma3ccRegsOvly edma3ccRegs = (CSL_Edma3ccRegsOvly)CSL_EDMA3CC_0_REGS;

  Int32* restrict adc_i_p = &adc_intern[0]; // we create here a pointer for compiler optimization reasons
  Int32* restrict HS1_Data_p = (Int32 *)&MeaData[HS1_DATA_OFFSET];
//	Int32* restrict IF_Data_p  = (Int32 *)&MeaData[IF_DATA_OFFSET];


  // Prepare DMA for next data transfer DO NOT CHANGE THE FOLLOWING LINE
  CSL_FINST(edma3ccRegs->ICRH, EDMA3CC_ICRH_I52, CLEAR);	// Clear pending interrupt for event 52
    //

  // Write to AUX register to see how long interrupt takes (set output to high, at the end set output to low)
  WRITE_REGISTER(0x0310, 0x1); // set AUX 1 to value one

#if 0

  // inititial setup
  if (timestamp == 0) {

    timestamp = 0;
    for (i = 0; i < HS1_CHANNELS/2; i++)
      {
        num_tr_cross[i]=0;
        last_tr_cross[i]=-deadtime;
      }

  }


  // collect data
  else if (timestamp < 5000)
    {
      for (i = 0; i < HS1_CHANNELS/2; i++)
        {
          *adc_i_p++ = *HS1_Data_p++;
        }

      for (i = 0; i < HS1_CHANNELS/2; i++)
        {
          if(adc_intern[i] < threshold && ((timestamp-last_tr_cross[i]) > deadtime) )
            {
              num_tr_cross[i]++;
              last_tr_cross[i] = timestamp;
            }
        }
    }
  // analyze, might take longer
  else if (timestamp == 5000)
    {
      int enable;
      int mux;
      int config;
      int iMeanActivity = 0;
      for (i = 0; i < HS1_CHANNELS/2; i++)
        {
          iMeanActivity = iMeanActivity + num_tr_cross[i];
        }
      iMeanActivity = iMeanActivity /(HS1_CHANNELS/2);

      StimulusEnable[0] = 0;
      StimulusEnable[1] = 0;

      DAC_select[0] = 0;
      DAC_select[1] = 0;
      DAC_select[2] = 0;
      DAC_select[3] = 0;

      elec_config[0] = 0;
      elec_config[1] = 0;
      elec_config[2] = 0;
      elec_config[3] = 0;

      for (i = 0; i < HS1_CHANNELS/2; i++)
        {
          // if (num_tr_cross[i] <= iMeanActivity) {
          if (num_tr_cross[i] > 0)
            //			if (i == 8 || i == 17)
            {
              enable = 1;
              mux = 1; // Stimulation Source is DAC 1
              config = 0; // Use Sidestream 1 for Stimulation Switch
            }
          else
            {
              enable = 0;
              mux = 0; // Keep MUX at ground
              config = 3; // Keep Switches static
            }

          StimulusEnable[i/30] |= (enable << i%30);
          DAC_select[i/15] |= (mux << 2*(i%15));
          elec_config[i/15] |= (config << 2*(i%15));
        }

      for (i = 0; i < 2; i++)
        {
          WRITE_REGISTER(0x9158+i*4, StimulusEnable[i]); // Enable Stimulation on STG
          //			WRITE_REGISTER(0x8140+i*4, StimulusEnable[i]); // Enable hard blanking for Stimulation Electrodes
        }

      for (i = 0; i < 4; i++)
        {
          WRITE_REGISTER(0x9160+i*4, DAC_select[i]);  // Select DAC 1 for Stimulation Electrodes
          WRITE_REGISTER(0x9120+i*4, elec_config[i]); // Configure Stimulation Electrodes to Listen to Sideband 1
        }

      WRITE_REGISTER(0x0218, segment << 16);  // select segment for trigger 1
      WRITE_REGISTER(0x0214, 0x00010001);     // Start Trigger 1
      segment = 1 - segment; // alternate between segment 0 and 1

      // analyze data
      // configure stim signal
    }
  // Wait for stimulation to finish
  else if (timestamp < 100000)
    {
      // analyze data
      // configure stim signal
    }
  else
    {
      // write result to mailbox


      //      timestamp = -1;
    }

  if (timestamp == 0)
    {
      WRITE_REGISTER(0x002C, 0x404); //switch on HS2 LED
    }
  else if (timestamp == 50000)
    {
      WRITE_REGISTER(0x002C, 0x400); //switch off HS2 LED
    }
  else if (timestamp == 100000)
    {
      timestamp = -1;
    }

#endif

    //MonitorData[0] = timestamp;
    if ((int)MeaData[HS1_DATA_OFFSET + 0] > (int)threshold)
    {
      MonitorData[0] = MeaData[HS1_DATA_OFFSET + 0];
        WRITE_REGISTER(FEEDBACK_REGISTER, 1);
    }
    else
    {
        MonitorData[0] = 0; //timestamp;
        WRITE_REGISTER(FEEDBACK_REGISTER, 0);
    }
    MonitorData[1] = MeaData[HS1_DATA_OFFSET + 0];
    CSL_FINST(edma3ccRegs->ESRH, EDMA3CC_ESRH_E53, SET);  // Trigger DMA event 53


  /*for (i = 0; i < HS2_CHANNELS; i++)
  {
    *adc_i_p++ = *HS2_Data_p++;
  }
  */
  /*for (i = 0; i < IF_CHANNELS; i++)
  {
    *adc_i_p++ = *IF_Data_p++;
  }*/


  WRITE_REGISTER(0x0310, 0x0); // set AUX 1 to value zero


    // loop through electrode
        // check for threshold crossing if passed deadtime (compare to timestamp that indicates last detected spike)
        // if yes then add 1 to rate counter and save timestamp

        // check static counter if 1 second in samples reached

        // decide which electrodes to stimulate

        // write stimulation to DACs


        // MONITOR EXECUTION TIME WITH DIGITAL PULSE`


  timestamp++;
}


// timer irq
interrupt void interrupt7(void)
{
  static int led = 0;
  CSL_FINS(gpioRegs->OUT_DATA, GPIO_OUT_DATA_OUT2, led); // LED
  led = 1 - led;
}
