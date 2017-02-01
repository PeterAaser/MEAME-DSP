
/*
  Some registers are named twice, with and without number prefix.
  When we want to set all electrode mode registers we do so in a
  for loop which becomes clearer when using the first register
  in a block without the numeric post-fix since they will all be written to
 */


#define MAIL_BASE           (0x1000)
#define REQUEST_ID          (MAIL_BASE)
#define STIM_FREQ_REQ       (MAIL_BASE + 0x8)
#define STIM_ELECTRODE_REQ  (MAIL_BASE + 0xc)
#define STIM_DAC_GROUP      (MAIL_BASE + 0x10)

#define STIM_BASE           (0x9000)

#define ELECTRODE_ENABLE    (STIM_BASE + 0x158)
#define ELECTRODE_ENABLE1   (STIM_BASE + 0x158)
#define ELECTRODE_ENABLE2   (STIM_BASE + 0x15c)

#define ELECTRODE_MODE      (STIM_BASE + 0x120)
#define ELECTRODE_MODE1     (STIM_BASE + 0x120)
#define ELECTRODE_MODE2     (STIM_BASE + 0x124)
#define ELECTRODE_MODE3     (STIM_BASE + 0x128)
#define ELECTRODE_MODE4     (STIM_BASE + 0x12c)

#define ELECTRODE_DAC_SEL   (STIM_BASE + 0x160)
#define ELECTRODE_DAC_SEL1  (STIM_BASE + 0x160)
#define ELECTRODE_DAC_SEL2  (STIM_BASE + 0x164)
#define ELECTRODE_DAC_SEL3  (STIM_BASE + 0x168)
#define ELECTRODE_DAC_SEL4  (STIM_BASE + 0x16c)

#define TRIGGER_REPEAT1     (STIM_BASE + 0x190)
#define TRIGGER_REPEAT2     (STIM_BASE + 0x194)
#define TRIGGER_REPEAT3     (STIM_BASE + 0x198)


#define TRIGGER_CTRL_BASE   (0x0200)

#define TRIGGER_CTRL        (TRIGGER_CTRL_BASE)
#define TRIGGER_CTRL1       (TRIGGER_CTRL_BASE)
#define START_STIM1         (TRIGGER_CTRL_BASE + 0x4)
#define END_STIM1           (TRIGGER_CTRL_BASE + 0x8)
#define WRITE_START1        (TRIGGER_CTRL_BASE + 0xc)
#define READ_START1         (TRIGGER_CTRL_BASE + 0x10)

#define TRIGGER_CTRL2       (TRIGGER_CTRL_BASE + 0x20)
#define START_STIM2         (TRIGGER_CTRL_BASE + 0x24)
#define END_STIM2           (TRIGGER_CTRL_BASE + 0x28)
#define WRITE_START2        (TRIGGER_CTRL_BASE + 0x2c)
#define READ_START2         (TRIGGER_CTRL_BASE + 0x30)

#define TRIGGER_CTRL3       (TRIGGER_CTRL_BASE + 0x40)
#define START_STIM3         (TRIGGER_CTRL_BASE + 0x44)
#define END_STIM3           (TRIGGER_CTRL_BASE + 0x48)
#define WRITE_START3        (TRIGGER_CTRL_BASE + 0x4c)
#define READ_START3         (TRIGGER_CTRL_BASE + 0x50)


#define MANUAL_TRIGGER      (TRIGGER_CTRL_BASE + 0x14)

/* and so on... */