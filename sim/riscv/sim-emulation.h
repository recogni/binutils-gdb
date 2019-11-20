#ifndef SIM_EMULATION_H
#define SIM_EMULATION_H

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * Temporary copies of systemC callback info.  This gets moved to sim_state
 * in sim_create_inferior - the sim_state is not created at the time of the
 * call so we have to keep a copy someplace until then.
 */


/*
 * Interrupt info from systemC to the processor(s).
 * Each is 8 bits so we can support up to 4 processors
 */
typedef union sim_emulation_rupt {
    struct {
	unsigned ext_rupt:1;
	unsigned timer_rupt:1;
	unsigned reset:1;
	unsigned halt:1;
	unsigned unused:4;
    };
    uint8_t rupt_bits;
} sim_emulation_rupt_t;

typedef struct sim_emulation_rupts {
    sim_emulation_rupt_t proc[4];
} sim_emulation_rupts_t;
    
extern void *gdbSP;
extern void (*register_sd_cb)(void *, void *);
extern sim_emulation_rupts_t (*tick_cb)(void *, int);
extern uint32_t (*read_reg_cb)(void *sp, uint32_t addr);
extern void (*write_reg_cb)(void *sp, uint32_t addr, uint32_t val);

#ifdef __cplusplus
} // extern "C"

// C++ only routines that get called from systemC
extern void sim_emulation_start_gdb(
	       int argc, char **argv,
	       char *progname2,
	       void *gdbServiceProcessor,
	       void (*register_sd)(void *sp, void *sd),
	       sim_emulation_rupts_t (*tick)(void *sp, int ticks),
	       uint32_t (*read_reg)(void *sp, uint32_t addr),
	       void (*write_reg)(void *sp, uint32_t addr, uint32_t val)
				);

extern void sim_emulation_fini(void);

extern int sim_emulation_mem_write(uint32_t addr, uint32_t *val, int size);
extern int sim_emulation_mem_read(uint32_t addr, uint32_t *val, int size);

#endif
#endif
