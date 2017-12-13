#ifndef REGISTERS_H_
#define REGISTERS_H_

/*
  Some registers are named twice, with and without number prefix.
  When we want to set all electrode mode registers we do so in a
  for loop which becomes clearer when using the first register
  in a block without the numeric post-fix since they will all be written to
 */

////////////////////////////////////////
////////////////////////////////////////
////////////////////////////////////////
///// HS1
#define HS1_BASE           (0x8000)
#define BLANKING_EN1       (HS1_BASE + 0x150)
#define BLANKING_EN2       (HS1_BASE + 0x154)


////////////////////////////////////////
////////////////////////////////////////
////////////////////////////////////////
///// MAILBOX
#define MAIL_BASE          (0x1000)

#define COMMS1             (MAIL_BASE + 0xc)
#define COMMS2             (MAIL_BASE + 0x10)
#define COMMS3             (MAIL_BASE + 0x14)
#define COMMS4             (MAIL_BASE + 0x18)

#define COMMS5             (MAIL_BASE + 0x1c)
#define COMMS6             (MAIL_BASE + 0x20)

#define ERROR              (MAIL_BASE + 0x24)
#define ERROR_VAL          (MAIL_BASE + 0x28)
#define ERROR_OP1          (MAIL_BASE + 0x2c)
#define ERROR_OP2          (MAIL_BASE + 0x30)

#define ENTRIES            (MAIL_BASE + 0x34)

#define DEBUG10            (MAIL_BASE + 0x400)
#define DEBUG11            (MAIL_BASE + 0x404)
#define DEBUG12            (MAIL_BASE + 0x408)
#define DEBUG13            (MAIL_BASE + 0x40c)
#define DEBUG14            (MAIL_BASE + 0x410)
#define DEBUG15            (MAIL_BASE + 0x414)
#define DEBUG16            (MAIL_BASE + 0x418)
#define DEBUG17            (MAIL_BASE + 0x41c)
#define DEBUG18            (MAIL_BASE + 0x420)
#define DEBUG19            (MAIL_BASE + 0x424)

#define DEBUG20            (MAIL_BASE + 0x428)
#define DEBUG21            (MAIL_BASE + 0x42c)
#define DEBUG22            (MAIL_BASE + 0x430)
#define DEBUG23            (MAIL_BASE + 0x434)
#define DEBUG24            (MAIL_BASE + 0x438)
#define DEBUG25            (MAIL_BASE + 0x43c)
#define DEBUG26            (MAIL_BASE + 0x440)
#define DEBUG27            (MAIL_BASE + 0x444)
#define DEBUG28            (MAIL_BASE + 0x448)
#define DEBUG29            (MAIL_BASE + 0x44c)

#define DEBUG30            (MAIL_BASE + 0x450)
#define DEBUG31            (MAIL_BASE + 0x454)
#define DEBUG32            (MAIL_BASE + 0x458)
#define DEBUG33            (MAIL_BASE + 0x45c)
#define DEBUG34            (MAIL_BASE + 0x460)
#define DEBUG35            (MAIL_BASE + 0x464)
#define DEBUG36            (MAIL_BASE + 0x468)
#define DEBUG37            (MAIL_BASE + 0x46c)
#define DEBUG38            (MAIL_BASE + 0x470)
#define DEBUG39            (MAIL_BASE + 0x474)

#define DEBUG40            (MAIL_BASE + 0x478)
#define DEBUG41            (MAIL_BASE + 0x47c)
#define DEBUG42            (MAIL_BASE + 0x480)
#define DEBUG43            (MAIL_BASE + 0x484)
#define DEBUG44            (MAIL_BASE + 0x488)
#define DEBUG45            (MAIL_BASE + 0x48c)
#define DEBUG46            (MAIL_BASE + 0x490)
#define DEBUG47            (MAIL_BASE + 0x494)
#define DEBUG48            (MAIL_BASE + 0x498)
#define DEBUG49            (MAIL_BASE + 0x49c)

#define DEBUG50            (MAIL_BASE + 0x4A0)
#define DEBUG51            (MAIL_BASE + 0x4A4)
#define DEBUG52            (MAIL_BASE + 0x4A8)
#define DEBUG53            (MAIL_BASE + 0x4Ac)
#define DEBUG54            (MAIL_BASE + 0x4B0)
#define DEBUG55            (MAIL_BASE + 0x4B4)
#define DEBUG56            (MAIL_BASE + 0x4B8)
#define DEBUG57            (MAIL_BASE + 0x4Bc)
#define DEBUG58            (MAIL_BASE + 0x4C0)
#define DEBUG59            (MAIL_BASE + 0x4C4)

#define DEBUG60            (MAIL_BASE + 0x4C8)
#define DEBUG61            (MAIL_BASE + 0x4Cc)
#define DEBUG62            (MAIL_BASE + 0x4D0)
#define DEBUG63            (MAIL_BASE + 0x4D4)
#define DEBUG64            (MAIL_BASE + 0x4D8)
#define DEBUG65            (MAIL_BASE + 0x4Dc)
#define DEBUG66            (MAIL_BASE + 0x4E0)
#define DEBUG67            (MAIL_BASE + 0x4E4)
#define DEBUG68            (MAIL_BASE + 0x4E8)
#define DEBUG69            (MAIL_BASE + 0x4Ec)

#define DEBUG70            (MAIL_BASE + 0x4F0)
#define DEBUG71            (MAIL_BASE + 0x4F4)
#define DEBUG72            (MAIL_BASE + 0x4F8)
#define DEBUG73            (MAIL_BASE + 0x4Fc)
#define DEBUG74            (MAIL_BASE + 0x500)
#define DEBUG75            (MAIL_BASE + 0x504)
#define DEBUG76            (MAIL_BASE + 0x508)
#define DEBUG77            (MAIL_BASE + 0x50c)
#define DEBUG78            (MAIL_BASE + 0x510)
#define DEBUG79            (MAIL_BASE + 0x514)

#define DEBUG80            (MAIL_BASE + 0x518)
#define DEBUG81            (MAIL_BASE + 0x51c)
#define DEBUG82            (MAIL_BASE + 0x520)
#define DEBUG83            (MAIL_BASE + 0x524)
#define DEBUG84            (MAIL_BASE + 0x528)
#define DEBUG85            (MAIL_BASE + 0x52c)
#define DEBUG86            (MAIL_BASE + 0x530)
#define DEBUG87            (MAIL_BASE + 0x534)
#define DEBUG88            (MAIL_BASE + 0x538)
#define DEBUG89            (MAIL_BASE + 0x53c)


#define WRITTEN_ADDRESS    (MAIL_BASE + 0x50)
#define COUNTER            (MAIL_BASE + 0x54)
#define PING_SEND          (MAIL_BASE + 0x58)
#define PING_READ          (MAIL_BASE + 0x5c)

#define LOG_BASE           (MAIL_BASE + 0x600)

#define MAILBOX_END        (MAIL_BASE + 0xFFC)


#define COMMS_BUFFER_MASTER_IDX (MAIL_BASE + 0x64) // MEAME -> DSP
#define COMMS_BUFFER_SLAVE_IDX  (MAIL_BASE + 0x68) // DSP -> MEAME
#define COMMS_BUFFER_START      (MAIL_BASE + 0x6c)


#define COMMS_MSG_BASE              (MAIL_BASE + 0x200)
#define COMMS_INSTRUCTIONS_EXECUTED (COMMS_MSG_BASE + 0x0)
#define COMMS_LAST_OP_TYPE          (COMMS_MSG_BASE + 0x4)
#define COMMS_LAST_OP_1             (COMMS_MSG_BASE + 0x8)
#define COMMS_LAST_OP_2             (COMMS_MSG_BASE + 0xc)
#define COMMS_LAST_ERROR            (COMMS_MSG_BASE + 0x10)
#define COMMS_LAST_ERROR_VAL        (COMMS_MSG_BASE + 0x14)


#define STIMPACK_MSG_BASE           (MAIL_BASE + 0x300)
#define STIMPACK_GROUP_DUMPED_GROUP (STIMPACK_MSG_BASE + 0x4)
#define STIMPACK_GROUP_DAC          (STIMPACK_MSG_BASE + 0x8)
#define STIMPACK_GROUP_ELECTRODES0  (STIMPACK_MSG_BASE + 0xc)
#define STIMPACK_GROUP_ELECTRODES1  (STIMPACK_MSG_BASE + 0x10)
#define STIMPACK_GROUP_PERIOD       (STIMPACK_MSG_BASE + 0x14)
#define STIMPACK_GROUP_TICK         (STIMPACK_MSG_BASE + 0x18)
#define STIMPACK_GROUP_SAMPLE       (STIMPACK_MSG_BASE + 0x1c)
#define STIMPACK_GROUP_FIRES        (STIMPACK_MSG_BASE + 0x20)


// essentially which trigger should be triggered
#define STIMPACK_SAMPLE        (STIMPACK_MSG_BASE + 0x24)

#define STIMPACK_PERIOD        (STIMPACK_MSG_BASE + 0x28)
#define STIMPACK_ELECTRODES0   (STIMPACK_MSG_BASE + 0x2c)
#define STIMPACK_ELECTRODES1   (STIMPACK_MSG_BASE + 0x30)



////////////////////////////////////////
////////////////////////////////////////
////////////////////////////////////////
///// STG
#define STIM_BASE          (0x9000)

#define ELECTRODE_ENABLE   (STIM_BASE + 0x158)
#define ELECTRODE_ENABLE1  (STIM_BASE + 0x158)
#define ELECTRODE_ENABLE2  (STIM_BASE + 0x15c)

#define ELECTRODE_MODE     (STIM_BASE + 0x120)
#define ELECTRODE_MODE1    (STIM_BASE + 0x120)
#define ELECTRODE_MODE2    (STIM_BASE + 0x124)
#define ELECTRODE_MODE3    (STIM_BASE + 0x128)
#define ELECTRODE_MODE4    (STIM_BASE + 0x12c)

#define ELECTRODE_DAC_SEL  (STIM_BASE + 0x160)
#define ELECTRODE_DAC_SEL1 (STIM_BASE + 0x160)
#define ELECTRODE_DAC_SEL2 (STIM_BASE + 0x164)
#define ELECTRODE_DAC_SEL3 (STIM_BASE + 0x168)
#define ELECTRODE_DAC_SEL4 (STIM_BASE + 0x16c)

#define TRIGGER_REPEAT1    (STIM_BASE + 0x190)
#define TRIGGER_REPEAT2    (STIM_BASE + 0x194)
#define TRIGGER_REPEAT3    (STIM_BASE + 0x198)


#define STG_MWRITE1  (STIM_BASE + 0xf20)
#define STG_MWRITE2  (STIM_BASE + 0xf24)
#define STG_MWRITE3  (STIM_BASE + 0xf28)
#define STG_MWRITE4  (STIM_BASE + 0xf2c)
#define STG_MWRITE5  (STIM_BASE + 0xf30)
#define STG_MWRITE6  (STIM_BASE + 0xf34)
#define STG_MWRITE7  (STIM_BASE + 0xf38)
#define STG_MWRITE8  (STIM_BASE + 0xf3c)


#define STG_MWRITE1  (STIM_BASE + 0xf20)
#define STG_MWRITE2  (STIM_BASE + 0xf24)
#define STG_MWRITE3  (STIM_BASE + 0xf28)
#define STG_MWRITE4  (STIM_BASE + 0xf2c)
#define STG_MWRITE5  (STIM_BASE + 0xf30)
#define STG_MWRITE6  (STIM_BASE + 0xf34)
#define STG_MWRITE7  (STIM_BASE + 0xf38)
#define STG_MWRITE8  (STIM_BASE + 0xf3c)


#define STG_MCLEAR_AND_WRITE1  (STIM_BASE + 0xf40)
#define STG_MCLEAR_AND_WRITE2  (STIM_BASE + 0xf44)
#define STG_MCLEAR_AND_WRITE3  (STIM_BASE + 0xf48)
#define STG_MCLEAR_AND_WRITE4  (STIM_BASE + 0xf4c)
#define STG_MCLEAR_AND_WRITE5  (STIM_BASE + 0xf50)
#define STG_MCLEAR_AND_WRITE6  (STIM_BASE + 0xf54)
#define STG_MCLEAR_AND_WRITE7  (STIM_BASE + 0xf58)
#define STG_MCLEAR_AND_WRITE8  (STIM_BASE + 0xf5c)


////////////////////////////////////////
////////////////////////////////////////
////////////////////////////////////////
///// TRIGGERS

#define TRIGGER_CTRL_BASE  (0x0200)

#define TRIGGER_CTRL       (TRIGGER_CTRL_BASE)
#define TRIGGER_CTRL1      (TRIGGER_CTRL_BASE)
#define START_STIM1        (TRIGGER_CTRL_BASE + 0x4)
#define END_STIM1          (TRIGGER_CTRL_BASE + 0x8)
#define WRITE_START1       (TRIGGER_CTRL_BASE + 0xc)
#define READ_START1        (TRIGGER_CTRL_BASE + 0x10)

#define TRIGGER_CTRL2      (TRIGGER_CTRL_BASE + 0x20)
#define START_STIM2        (TRIGGER_CTRL_BASE + 0x24)
#define END_STIM2          (TRIGGER_CTRL_BASE + 0x28)
#define WRITE_START2       (TRIGGER_CTRL_BASE + 0x2c)
#define READ_START2        (TRIGGER_CTRL_BASE + 0x30)

#define TRIGGER_CTRL3      (TRIGGER_CTRL_BASE + 0x40)
#define START_STIM3        (TRIGGER_CTRL_BASE + 0x44)
#define END_STIM3          (TRIGGER_CTRL_BASE + 0x48)
#define WRITE_START3       (TRIGGER_CTRL_BASE + 0x4c)
#define READ_START3        (TRIGGER_CTRL_BASE + 0x50)


#define MANUAL_TRIGGER     (TRIGGER_CTRL_BASE + 0x14)

/* and so on... */

#endif /* REGISTERS_H_ */
