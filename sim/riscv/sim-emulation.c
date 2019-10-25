/* RISC-V simulator.
 */


#include "config.h"
#include <inttypes.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/time.h>

#include "sim-emulation.h"

#include "main.h"

void *sim_emulation_setup(int argc, char **argv,
			  void *gdbServiceProcessor,
			  void (*register_sd)(void *sp, void *sd),
			  void (*tick)(void *sp, int ticks),
			  unsigned long long (*read_reg)(void *sp,
							 unsigned long long addr),
			  void (*write_reg)(void *sp, unsigned long long addr,
					    unsigned long long val)
			  ) {
    struct captured_main_args args;
    args.argc = argc;
    args.argv = argv;
    args.interpreter_p = "console";

    gdbSP = gdbServiceProcessor;

    tick_cb = tick;
    register_sd_cb = register_sd;
    read_reg_cb = read_reg;
    write_reg_cb = write_reg;
    
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
    
