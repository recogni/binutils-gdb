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
char *prog_name1, *prog_name2;

static
void sim_gdb_thread(void) {
    gdb_main(&args);			// Initialize GDB
    gdbsim_target_open(NULL, 0);  	// "target sim" comand
    target_load(prog_name1, 0);		// Load
    starti_command(prog_name1, 0);	// Start program1
    add_inferior_exec(prog_name2);
    inferior_command("2", 0);           // Switch to inferior 2
    target_load(prog_name2, 0);		// Load
    starti_command(prog_name2, 0);	// Start program 2
    command_loop();			// Does not return until gdb exits
}

void sim_emulation_start_gdb(int argc, char **argv,
			     char *progname2,
			     void *gdbServiceProcessor,
			     void (*register_sd)(void *sp, void *sd),
			     void (*tick)(void *sp, int ticks),
			     unsigned long long (*read_reg)(void *sp,
							    unsigned long long addr),
			     void (*write_reg)(void *sp, unsigned long long addr,
					       unsigned long long val)
			     ) {
    prog_name1 = argv[1];	  // Save for later - GDB messes wtih args
    args.argc = argc;
    args.argv = argv;
    args.interpreter_p = "console";

    prog_name2 = progname2;

    gdbSP = gdbServiceProcessor;

    tick_cb = tick;
    register_sd_cb = register_sd;
    read_reg_cb = read_reg;
    write_reg_cb = write_reg;

    gdb_cmd_thread = std::thread(sim_gdb_thread);
}

void sim_emulation_fini(void) {
}
    
int sim_emulation_mem_write(unsigned long long addr, unsigned long long *val, int size) {
    return 1;
}

int sim_emulation_mem_read(unsigned long long addr, unsigned long long *val, int size) {
    return 1;
}
    
