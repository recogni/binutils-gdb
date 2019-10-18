/* Thread pool

   Copyright (C) 2019 Free Software Foundation, Inc.

   This file is part of GDB.

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

#include "common-defs.h"
#include "gdbsupport/thread-pool.h"
#include "gdbsupport/alt-stack.h"
#include "gdbsupport/block-signals.h"
#include <algorithm>

/* On the off chance that we have the pthread library on a Windows
   host, but std::thread is not using it, avoid calling
   pthread_setname_np on Windows.  */
#ifndef _WIN32
#ifdef HAVE_PTHREAD_SETNAME_NP
#include <pthread.h>
#endif
#endif

namespace gdb
{

thread_pool thread_pool::g_thread_pool;

thread_pool::thread_pool ()
  : m_tasks_cv (new std::condition_variable)
{
}

thread_pool::~thread_pool ()
{
  /* Because this is a singleton, we don't need to clean up.  The
     threads are detached so that they won't prevent process exit.
     And, cleaning up here would be actively harmful in at least one
     case -- see the comment by m_tasks_cv.  */
}

void
thread_pool::set_thread_count (size_t num_threads)
{
  std::lock_guard<std::mutex> guard (m_tasks_mutex);

  /* If the new size is larger, start some new threads.  */
  if (m_count < num_threads)
    {
      /* Ensure that signals used by gdb are blocked in the new
	 threads.  */
      block_signals blocker;
      for (size_t i = m_count; i < num_threads; ++i)
	{
	  std::thread thread (&thread_pool::thread_function, this);
#ifndef _WIN32 /* See the comment at the top of the file.  */
#ifdef HAVE_PTHREAD_SETNAME_NP
	  pthread_setname_np (thread.native_handle (), "gdb worker");
#endif
#endif
	  thread.detach ();
	}
    }
  /* If the new size is smaller, terminate some existing threads.  */
  if (num_threads < m_count)
    {
      for (size_t i = num_threads; i < m_count; ++i)
	m_tasks.emplace ();
      m_tasks_cv->notify_all ();
    }

  m_count = num_threads;
}

std::future<void>
thread_pool::post_task (std::function<void ()> func)
{
  std::packaged_task<void ()> t (func);
  std::future<void> f = t.get_future ();

  if (m_count == 0)
    {
      /* Just execute it now.  */
      t ();
    }
  else
    {
      std::lock_guard<std::mutex> guard (m_tasks_mutex);
      m_tasks.emplace (std::move (t));
      m_tasks_cv->notify_one ();
    }
  return f;
}

void
thread_pool::thread_function ()
{
  /* Ensure that SIGSEGV is delivered to an alternate signal
     stack.  */
  gdb::alternate_signal_stack signal_stack;

  while (true)
    {
      task t;

      {
	/* We want to hold the lock while examining the task list, but
	   not while invoking the task function.  */
	std::unique_lock<std::mutex> guard (m_tasks_mutex);
	while (m_tasks.empty ())
	  m_tasks_cv->wait (guard);
	t = std::move (m_tasks.front());
	m_tasks.pop ();
      }

      if (!t.has_value ())
	break;
      (*t) ();
    }
}

}
