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
bool batch_option;

static
void sim_gdb_thread(void) {
    gdb_main(&args);			// Initialize GDB
    gdbsim_target_open(NULL, 0);  	// "target sim" comand
    target_load(prog_name1, 0);		// Load
    starti_command(prog_name1, 0);	// Start program1
    add_inferior_exec(prog_name2);
    inferior_command("2", 0);           // Switch to inferior 2
    target_load(prog_name2, 0);		// Load
     if (batch_option) {
	 run_command(prog_name2, 0);	// run program 2	
    } else {
	 starti_command(prog_name2, 0);	// Start program 2
	 command_loop();                // Process commands
    }
}

void sim_emulation_start_gdb(int argc, char **argv, bool batch_mode,
			     char *progname2,
			     void *gdbServiceProcessor,
			     void (*register_sd)(void *sp, void *sd),
			     sim_emulation_rupts_t (*tick)(void *sp, int ticks),
			     uint32_t (*read_reg)(void *sp, uint32_t addr),
			     void (*write_reg)(void *sp, uint32_t addr,
					       uint32_t val)
			     ) {
    prog_name1 = argv[1];	  // Save for later - GDB messes wtih args
    args.argc = argc;
    args.argv = argv;
    batch_option = batch_mode;
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
    
int sim_emulation_mem_write(uint32_t addr, uint32_t *val, int size) {
    return 1;
}

int sim_emulation_mem_read(uint32_t addr, uint32_t *val, int size) {
    return 1;
}
    
