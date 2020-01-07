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
int gdb_inferior = 1;

int gdb_thread_start = 0;
int gdb_thread_end = 0;
int gdb_em_start = 0;
int gdb_em_fini = 0;
int gdb_em_start_end = 0;
int gdb_em_fini_end = 0;

static
void sim_gdb_thread(void) {
    char cmd[160];
    gdb_thread_start++;
    if (done_flag) {
	// Some simulations never really use GDB
	gdb_thread_end++;
	return;
    }
    
    if (gdb_inferior == 1) {
	gdb_main(&args);			// Initialize GDB
	execute_command("target sim", 0);
    }

    strcpy(cmd, "add-inferior -exec ");
    strcat(cmd, prog_name1);
    execute_command(cmd, 0);

    gdb_inferior++;

    sprintf(cmd, "inferior %i", gdb_inferior);
    execute_command(cmd, 0);

    strcpy(cmd, "load ");
    strcat(cmd, prog_name1);
    execute_command(cmd, 0);

    strcpy(cmd, "starti ");
    strcat(cmd, prog_name1);
    execute_command(cmd, 0);

    strcpy(cmd, "add-inferior -exec ");
    strcat(cmd, prog_name2);
    execute_command(cmd, 0);

    gdb_inferior++;

    sprintf(cmd, "inferior %i", gdb_inferior);
    execute_command(cmd, 0);

    strcpy(cmd, "load ");
    strcat(cmd, prog_name2);
    execute_command(cmd, 0);

    strcpy(cmd, "starti ");
    strcat(cmd, prog_name2);
    execute_command(cmd, 0);

    if (done_flag) {
	// Last-minute check
    } else if (batch_option) {
	 execute_command("continue", 0);  // let 'er rip
    } else {
	 command_loop();                // Process commands
    }

    // GDB program finished - just loop until systemC finishes
    while (done_flag == 0) {
	tick_cb(gdbSP, 0x7f);
    }
    
    // Cleanup
    execute_command("inferior 1", 0);
    sprintf(cmd, "exit-inferior %i", gdb_inferior - 1);
    execute_command(cmd, 0);
    sprintf(cmd, "remove-inferior %i", gdb_inferior - 1);
    execute_command(cmd, 0);
    
    sprintf(cmd, "exit-inferior %i", gdb_inferior);
    execute_command(cmd, 0);
    sprintf(cmd, "remove-inferior %i", gdb_inferior);
    execute_command(cmd, 0);
    gdb_thread_end++;
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
    gdb_em_start++;
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
    done_flag = 0;

    gdb_cmd_thread = std::thread(sim_gdb_thread);
    std::this_thread::yield();
    gdb_em_start_end++;
}

void sim_emulation_fini(void) {
    gdb_em_fini++;
    done_flag = 1;
    if (gdb_cmd_thread.joinable()) {
	gdb_cmd_thread.join();
    }
    gdb_em_fini_end++;
}
    
int sim_emulation_mem_write(uint32_t addr, uint32_t *val, int size) {
    return 1;
}

int sim_emulation_mem_read(uint32_t addr, uint32_t *val, int size) {
    return 1;
}
    
