#include <csl.h>
#include <cslr_tmr.h>
#include <cslr_gpio.h>
#include <cslr_chip.h>
#include <cslr_edma3cc.h>
#include <soc.h>
#include <c6x.h>

#include "main.h"
#include "stimpack.h"
#include "registers.h"
#include "irq.h"
#include "MEA21_lib.h"

int num_tr_cross[HS1_CHANNELS/2];
int last_tr_cross[HS1_CHANNELS/2];

Uint32 reg_written_address;
Uint32 reg_value;
Uint32 stim_req;

// Mailbox write interrupt
// use "#define USE_MAILBOX_IRQ" in global.h to enable this interrupt
interrupt void interrupt8(void)
{
  static Uint32 req_id = 0;
  req_id++;

  reg_written_address = (READ_REGISTER(0x428)) + 0x1000;
  reg_value           = READ_REGISTER(reg_written_address);


  if(reg_written_address == REQUEST_ID){
    read_stim_request();
    WRITE_REGISTER(REQUEST_ACK, reg_value);
  }

  if(reg_written_address == DUMP_STIM_GROUP)
    {
      dump_stim_group(reg_value);
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


  run_stimpack();


// Lots of bullshit that I have no clue what does
////////////////////////////////////////
////////////////////////////////////////
////////////////////////////////////////
  static int timestamp = 0;
  static int segment = 0;

  int i;
  CSL_Edma3ccRegsOvly edma3ccRegs = (CSL_Edma3ccRegsOvly)CSL_EDMA3CC_0_REGS;

  Int32* restrict adc_i_p = &adc_intern[0];
  Int32* restrict HS1_Data_p = (Int32 *)&MeaData[HS1_DATA_OFFSET];


  CSL_FINST(edma3ccRegs->ICRH, EDMA3CC_ICRH_I52, CLEAR);

  WRITE_REGISTER(0x0310, 0x1);


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
  CSL_FINST(edma3ccRegs->ESRH, EDMA3CC_ESRH_E53, SET);

  WRITE_REGISTER(0x0310, 0x0);

  timestamp++;

  ////////////////////////////////////////
  ////////////////////////////////////////
  ////////////////////////////////////////
}


// timer irq
interrupt void interrupt7(void)
{
  static int led = 0;
  CSL_FINS(gpioRegs->OUT_DATA, GPIO_OUT_DATA_OUT2, led); // LED
  led = 1 - led;
}
