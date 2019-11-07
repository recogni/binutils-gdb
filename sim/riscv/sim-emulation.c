/* RISC-V simulator.
 */

#include <thread>

#include "config.h"

#include "defs.h"
#include "top.h"
#include "main.h"
#include "utils.h"
#include "inferior.h"

#include "sim-emulation.h"

std::thread gdb_cmd_thread;

struct captured_main_args args;
char *prog_name;

const char *sim_program = "/home/jim/riscv-business/riscv32-unknown-elf/bin/goodbye-world";

static
void sim_gdb_thread(void) {
    gdb_main(&args);	// Initialize GDB
    gdbsim_target_open(NULL, 0);  // "target sim" comand
    target_load(prog_name, 0);	  // Load program
    add_inferior_exec(sim_program);
    command_loop();		// Does not return until gdb exits
}

void sim_emulation_start_gdb(int argc, char **argv,
			     void *gdbServiceProcessor,
			     void (*register_sd)(void *sp, void *sd),
			     void (*tick)(void *sp, int ticks),
			     unsigned long long (*read_reg)(void *sp,
							    unsigned long long addr),
			     void (*write_reg)(void *sp, unsigned long long addr,
					       unsigned long long val)
			     ) {
    prog_name = argv[1];	  // Save for later - GDB messes wtih args
    args.argc = argc;
    args.argv = argv;
    args.interpreter_p = "console";

    gdbSP = gdbServiceProcessor;

    tick_cb = tick;
    register_sd_cb = register_sd;
    read_reg_cb = read_reg;
    write_reg_cb = write_reg;

    gdb_cmd_thread = std::thread(sim_gdb_thread);
}

void sim_emulation_add(void *gdbServiceProcessor, char *sim_program) {
    gdbSP = gdbServiceProcessor;
    add_inferior_exec(sim_program);
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
    
