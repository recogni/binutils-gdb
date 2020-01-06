/* Generic simulator resume.
   Copyright (C) 1997-2019 Free Software Foundation, Inc.
   Contributed by Cygnus Support.

This file is part of GDB, the GNU debugger.

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

#include "sim-main.h"
#include "sim-assert.h"

/* Halt the simulator after just one instruction */

static void
has_stepped (SIM_DESC sd,
	     void *data)
{
  ASSERT (STATE_MAGIC (sd) == SIM_MAGIC_NUMBER);
  sim_engine_halt (sd, NULL, NULL, NULL_CIA, sim_stopped, SIM_SIGTRAP);
}

/* Generic resume - assumes the existance of sim_engine_run */

void *
sim_resume (SIM_DESC sd,
	    int step,
	    int siggnal)
{
    int local_done_flag = 0;
    
    for (SIM_DESC cur_sd = sim_state_head->sim_state_next;
	 cur_sd != NULL; cur_sd = cur_sd->sim_state_next) {
	sim_engine *engine;
	jmp_buf buf;
	int jmpval;
	int last_cpu_nr;
	int next_cpu_nr;
	int nr_cpus;

	ASSERT (STATE_MAGIC (cur_sd) == SIM_MAGIC_NUMBER);
	engine = STATE_ENGINE (cur_sd);
	/* we only want to be single stepping the simulator once */
	if (engine->stepper != NULL)
	    {
		sim_events_deschedule (cur_sd, engine->stepper);
		engine->stepper = NULL;
	    }
	if (step)
	    engine->stepper = sim_events_schedule (cur_sd, 1, has_stepped, cur_sd);

	sim_module_resume(cur_sd);

	last_cpu_nr = sim_engine_last_cpu_nr (cur_sd);
	next_cpu_nr = sim_engine_next_cpu_nr (cur_sd);
	nr_cpus = sim_engine_nr_cpus (cur_sd);

	sim_events_preprocess (cur_sd, last_cpu_nr >= nr_cpus, next_cpu_nr >= nr_cpus);
    }
      
    while ((done_flag == 0) && (local_done_flag == 0)) {
	// Resume them all - one step each at a time
	for (SIM_DESC cur_sd = sim_state_head->sim_state_next;
	     cur_sd != NULL; cur_sd = cur_sd->sim_state_next) {
	    sim_engine *engine;
	    jmp_buf buf;
	    int jmpval;

	    ASSERT (STATE_MAGIC (cur_sd) == SIM_MAGIC_NUMBER);
	    engine = STATE_ENGINE (cur_sd);

	    /* run/resume the simulator */
	    engine->jmpbuf = &buf;
	    jmpval = setjmp (buf);
	    if (jmpval == sim_engine_start_jmpval
		|| jmpval == sim_engine_restart_jmpval)
		{
		    int next_cpu_nr = sim_engine_next_cpu_nr (cur_sd);
		    int nr_cpus = sim_engine_nr_cpus (cur_sd);
		    int sig_to_deliver;

		    if (next_cpu_nr >= nr_cpus)
			next_cpu_nr = 0;

		    /* Only deliver the SIGGNAL [sic] the first time through - don't
		       re-deliver any SIGGNAL during a restart.  NOTE: A new local
		       variable is used to avoid problems with the automatic
		       variable ``siggnal'' being trashed by a long jump.  */
		    if (jmpval == sim_engine_start_jmpval)
			sig_to_deliver = siggnal;
		    else
			sig_to_deliver = 0;

#ifdef SIM_CPU_EXCEPTION_RESUME
		    {
			sim_cpu* cpu = STATE_CPU (cur_sd, next_cpu_nr);
			SIM_CPU_EXCEPTION_RESUME (cur_sd, cpu, sig_to_deliver);
		    }
#endif

		    sim_engine_run (cur_sd, next_cpu_nr, nr_cpus, sig_to_deliver);
		}
	    else
		{
		    local_done_flag = 1;
		} 
      
	    engine->jmpbuf = NULL;
	}
    }
    
    // Check for simulation termination
    for (SIM_DESC cur_sd = sim_state_head->sim_state_next;
	 cur_sd != NULL; cur_sd = cur_sd->sim_state_next) {
	sim_engine *engine = STATE_ENGINE (cur_sd);
	sim_module_suspend (cur_sd);
	if (done_flag) {
	    engine->reason = sim_stopped;
	    engine->sigrc = SIM_SIGNONE;
	} else if (engine->reason == sim_running) {
	    // One of the other preocessors stopped - treat like trap
	    engine->reason = sim_stopped;
	    engine->sigrc = SIM_SIGTRAP;
	} 
    }

  return (sd->opaque_inf);
}
