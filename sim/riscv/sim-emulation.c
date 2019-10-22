/* RISC-V simulator.
 */

#include "sim-basics.h"

#include "sim-emulation.h"

#include "main.h"

char *fake_arg = "GDB Simulation";
void sim_emulation_setup(void (*tick_cb)(int ticks, void *clk), void *clk) {
    struct captured_main_args args;
    args.argc = 1;
    args.argv = &fake_arg;
    args.interpreter_p = "console";
    
    tick_wait = tick_cb;
    tick_clk = clk;
    gdb_main(&args);
}
    

void sim_emulation_fini(void) {
}
    

int sim_emulation_pin_get(char * pin_type, unsigned pin_instance) {
    //    return proc.pins.pin_get(pin_type, pin_instance);
    return 0;
}

void sim_emulation_pin_set(char * pin_type, unsigned pin_instance,
		       int pullup, int val) {
    //    proc.pins.ext_pin_set(pin_type, pin_instance, val);
    //proc.pins.ext_pin_pullup(pin_type, pin_instance, pullup);
}

int sim_emulation_mem_write(unsigned long long addr, unsigned long long *val, int size) {
    return 1;
}

int sim_emulation_mem_read(unsigned long long addr, unsigned long long *val, int size) {
    return 1;
}
    
