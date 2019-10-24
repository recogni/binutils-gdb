#ifndef SIM_EMULATION_H
#define SIM_EMULATION_H

#ifdef __cplusplus
extern "C"
{
#endif
    
    extern void (*tick_wait)(int, void *clk);
    extern void *tick_clk;
    
#ifdef __cplusplus
} // extern "C"

// C++ only routines that get called from systemC
extern void sim_emulation_setup(int argc, char **argv,
				void (*tick_cb)(int, void*), void *clk);
extern void sim_emulation_fini(void);
//extern void sim_emulation_set_reg_write_callback(std::function<int (unsigned long long,
//  							              unsigned long long)> fn);
extern int sim_emulation_pin_get(char * pin_type, unsigned pin_instance);
extern void sim_emulation_pin_set(char * pin_type, unsigned pin_instance,
			   int pullup, int val);
extern int sim_emulation_mem_write(unsigned long long addr, unsigned long long *val, int size);
extern int sim_emulation_mem_read(unsigned long long addr, unsigned long long *val, int size);

#endif
#endif
