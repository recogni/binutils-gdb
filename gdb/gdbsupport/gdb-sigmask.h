/* sigprocmask wrapper for gdb

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

#ifndef GDBSUPPORT_GDB_SIGMASK_H
#define GDBSUPPORT_GDB_SIGMASK_H

#include <signal.h>

#ifndef HAVE_SIGPROCMASK
#ifdef HAVE_PTHREAD_SIGMASK
#error pthead_sigmask available without sigprocmask - please report
#endif
#endif

#ifdef HAVE_SIGPROCMASK

#ifdef HAVE_PTHREAD_SIGMASK
#define gdb_sigmask pthread_sigmask
#else
#define gdb_sigmask sigprocmask
#endif

#endif


#endif /* GDBSUPPORT_GDB_SIGMASK_H */
