/* RISC-V simulator.

   Copyright (C) 2005-2015 Free Software Foundation, Inc.
   Contributed by Mike Frysinger.

   This file is part of simulators.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* This file contains the main glue logic between the sim core and the target
   specific simulator.  Normally this file will be kept small and the target
   details will live in other files.

   For more specific details on these functions, see the gdb/remote-sim.h
   header file.  */

#include "config.h"

#include "libiberty.h"
#include "bfd.h"
#include "elf-bfd.h"

#include "main.h"

#include "sim-main.h"
#include "sim-module.h"
#include "gdb/remote-sim.h"
#include "sim-options.h"
#include "sim-emulation.h"

void *gdbSP = NULL;
void (*register_sd_cb)(void *, void *) = NULL;
sim_emulation_rupts_t (*tick_cb)(void *, int) = NULL;
uint32_t (*read_reg_cb)(void *sp, uint32_t addr) = NULL;
void (*write_reg_cb)(void *sp, uint32_t addr, uint32_t val) = NULL;
int done_flag;

/* List of SDs */
struct sim_state *sim_state_head = NULL;


/* This function is the main loop.  It should process ticks and decode+execute
   a single instruction.

   Usually you do not need to change things here.  */
  int tick_cnt = 0;

void
sim_engine_run (SIM_DESC sd,
		int next_cpu_nr, /* ignore  */
		int nr_cpus, /* ignore  */
		int siggnal) /* ignore  */
{
  SIM_CPU *cpu;
  sim_engine *engine = STATE_ENGINE(sd);
  
  SIM_ASSERT (STATE_MAGIC (sd) == SIM_MAGIC_NUMBER);
  cpu = STATE_CPU (sd, 0);

  if ((sd == sim_state_head->sim_state_next) && (done_flag == 0) &&
      (sd->tick_cb != NULL)) {
      // Tick the systemC clock
      SIM_DESC rupt_sd = sd;
      sim_emulation_rupts_t rupts;
      rupts = sd->tick_cb(sd->gdbSP, 1);
      tick_cnt++;
      for (int i = 0; i < 4; i++) {
	  rupt_sd->ext_rupt = rupts.proc[i];
	  rupt_sd = rupt_sd->sim_state_next;
	  if (rupt_sd == NULL) {
	      break;
	  } 
      } 
  } 
  
  if (cpu) {
      step_once (cpu);
      if (sim_events_tick (sd)) {
	  sim_events_process (sd);
      }
  }
      
  if (done_flag) {
      engine->reason = sim_exited;
      engine->sigrc = 0;
  }

}

/* Initialize the simulator from scratch.  This is called once per lifetime of
   the simulation.  Think of it as a processor reset.

   Usually all cpu-specific setup is handled in the initialize_cpu callback.
   If you want to do cpu-independent stuff, then it should go at the end (see
   where memory is initialized).  */

static void
free_state (SIM_DESC sd)
{
  if (STATE_MODULES (sd) != NULL)
    sim_module_uninstall (sd);
  sim_cpu_free_all (sd);
  sim_state_free (sd);
}

static void
alloc_mem (SIM_DESC sd)
{
  asection *s;
  SIM_ASSERT (STATE_MAGIC (sd) == SIM_MAGIC_NUMBER);

  /* Flash (executable space) - 2M (probably lots more)*/
  sim_core_attach(sd, NULL, 0, access_read_write_exec, 0, 0x10400000, 0x200000,
		  0, NULL, NULL);
  /* Data - 64K (???) */
  sim_core_attach(sd, NULL, 0, access_read_write, 0, 0x20000000, 0x40000,
		  0, NULL, NULL);
  /* External registers */
  sim_core_attach(sd, NULL, 0, access_read_write, 0, 0x40000000, 0x10000,
		  0, (struct hw*) 0xffffffff, NULL);
  return;
}

#ifndef SIM_CLOSE_HOOK
# define SIM_CLOSE_HOOK(sd, quitting)
#endif

void
sim_close (SIM_DESC sd, int quitting)
{
  SIM_CLOSE_HOOK (sd, quitting);

  /* If cgen is active, close it down.  */
#ifdef CGEN_ARCH
# define cgen_cpu_close XCONCAT2 (CGEN_ARCH,_cgen_cpu_close)
  cgen_cpu_close (CPU_CPU_DESC (STATE_CPU (sd, 0)));
#endif

  /* Shut down all registered/active modules.  */
  sim_module_uninstall (sd);

  /* Ensure that any resources allocated through the callback
     mechanism are released.  */
  sim_io_shutdown (sd);

  /* Break down all of the cpus.  */
  sim_cpu_free_all (sd);

  /* Unlink */
  if (sim_state_head == sd) {
      sim_state_head = sd->sim_state_next;
  } else {
      // Walk the chain to find the last
      SIM_DESC esd;
      for (esd = sim_state_head; esd->sim_state_next != NULL;
	   esd = esd->sim_state_next) {
	  if (esd->sim_state_next == sd) {
	      esd->sim_state_next = sd->sim_state_next;
	      break;
	  } 
      }
  } 
  sd->sim_state_next = NULL;

  /* Finally break down the sim state itself.  */
  sim_state_free (sd);
}


SIM_DESC
sim_open (SIM_OPEN_KIND kind, host_callback *callback,
	  struct bfd *abfd, char * const *argv)
{
  char c;
  int i;
  SIM_DESC sd = sim_state_alloc (kind, callback);

  /* Link them up back to front */
  sd->sim_state_next = NULL;
  if (sim_state_head == NULL) {
      sim_state_head = sd;
  } else {
      // Walk the chain to find the last
      SIM_DESC esd;
      for (esd = sim_state_head; esd->sim_state_next != NULL;
	   esd = esd->sim_state_next) {
      }
      esd->sim_state_next = sd;
  } 

  /* The cpu data is kept in a separately allocated chunk of memory.  */
  if (sim_cpu_alloc_all (sd, 1, /*cgen_cpu_max_extra_bytes ()*/0) != SIM_RC_OK)
    {
      free_state (sd);
      return 0;
    }

  if (sim_pre_argv_init (sd, argv[0]) != SIM_RC_OK)
    {
      free_state (sd);
      return 0;
    }

  /* XXX: Default to the Virtual environment.  */
  if (STATE_ENVIRONMENT (sd) == ALL_ENVIRONMENT)
    STATE_ENVIRONMENT (sd) = VIRTUAL_ENVIRONMENT;

  /* getopt will print the error message so we just have to exit if this fails.
     FIXME: Hmmm...  in the case of gdb we need getopt to call
     print_filtered.  */
  if (sim_parse_args (sd, argv) != SIM_RC_OK)
    {
      free_state (sd);
      return 0;
    }

  /* Allocate the memory blocks */
  alloc_mem(sd);
  
  /* Check for/establish the a reference program image.  */
  if (sim_analyze_program(sd,
	       (STATE_PROG_ARGV (sd) != NULL
		? *STATE_PROG_ARGV (sd)
		: NULL), abfd) != SIM_RC_OK)
      {
	  free_state (sd);
	  return 0;
      }

  /* Establish any remaining configuration options.  */
  if (sim_config (sd) != SIM_RC_OK)
    {
      free_state (sd);
      return 0;
    }

  if (sim_post_argv_init (sd) != SIM_RC_OK)
    {
      free_state (sd);
      return 0;
    }

  /* CPU specific initialization.  */
  for (i = 0; i < MAX_NR_PROCESSORS; ++i)
    {
      SIM_CPU *cpu = STATE_CPU (sd, i);

      initialize_cpu (sd, cpu, i);
    }

  return sd;
}

static bfd_vma
riscv_get_symbol (SIM_DESC sd, const char *sym)
{
  long symcount = STATE_PROG_SYMS_COUNT (sd);
  asymbol **symtab = STATE_PROG_SYMS (sd);
  int i;

  for (i = 0;i < symcount; ++i)
    {
      if (strcmp (sym, bfd_asymbol_name (symtab[i])) == 0)
	{
	  bfd_vma sa;
	  sa = bfd_asymbol_value (symtab[i]);
	  return sa;
	}
    }

  /* Symbol not found.  */
  return 0;
}

/* Prepare to run a program that has already been loaded into memory.

   Usually you do not need to change things here.  */

SIM_RC
sim_create_inferior (SIM_DESC sd, struct bfd *abfd,
		     char * const *argv, char * const *env,
		     void *opaque_inf)
{
  SIM_CPU *cpu = STATE_CPU (sd, 0);
  sim_cia addr;
  Elf_Internal_Phdr *phdr;
  int i, phnum;

  /* Copy the systemC callback info out of the static variables */
  sd->gdbSP = gdbSP;
  sd->tick_cb = tick_cb;
  sd->read_reg_cb = read_reg_cb;
  sd->write_reg_cb = write_reg_cb;

  sd->opaque_inf = opaque_inf;

  /* Tell systemc where to find us */
  register_sd_cb(sd->gdbSP, (void *) sd);
  
  /* Set the PC.  */
  if (abfd != NULL)
    addr = bfd_get_start_address (abfd);
  else
    addr = 0;
  sim_pc_set (cpu, addr);
  phdr = elf_tdata (abfd)->phdr;
  phnum = elf_elfheader (abfd)->e_phnum;

  /* Try to find _end symbol, and set it to the end of brk.  */
  trace_load_symbols (sd);
  cpu->endbrk = riscv_get_symbol (sd, "_end");

  /* If not found, set end of brk to end of all section.  */
  if (cpu->endbrk == 0)
    {
      for (i = 0; i < phnum; i++)
	{
	  if (phdr[i].p_paddr + phdr[i].p_memsz > cpu->endbrk)
	    cpu->endbrk = phdr[i].p_paddr + phdr[i].p_memsz;
	}
    }

  /* Standalone mode (i.e. `run`) will take care of the argv for us in
     sim_open() -> sim_parse_args().  But in debug mode (i.e. 'target sim'
     with `gdb`), we need to handle it.  */
  if (STATE_OPEN_KIND (sd) == SIM_OPEN_DEBUG)
    {
      freeargv (STATE_PROG_ARGV (sd));
      STATE_PROG_ARGV (sd) = dupargv (argv);
    }

  initialize_env (sd, (void *)argv, (void *)env);

  if (sim_load(sd,
	       (STATE_PROG_ARGV (sd) != NULL
		? *STATE_PROG_ARGV (sd)
		: NULL), abfd, 0) != SIM_RC_OK)
      {
	  return 0;
      }

  return SIM_RC_OK;
}
