/* Non-portable functions. Viengoos version.
   Copyright (C) 2008 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/*
 * Never include this file directly; use <pthread.h> or <cthreads.h> instead.
 */

#ifndef _BITS_PTHREAD_NP_H
#define _BITS_PTHREAD_NP_H	1

#include <viengoos/addr.h>
#include <hurd/thread.h>

int pthread_setactivity_np (addr_t activity);

/* Returns the caller's activation state block.  */
struct hurd_utcb *pthread_hurd_utcb_np (void) __attribute__ ((pure));

#endif /* bits/pthread-np.h */
