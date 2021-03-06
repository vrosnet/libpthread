/* pthread_key_create.  Hurd version.
   Copyright (C) 2002 Free Software Foundation, Inc.
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

#include <pthread.h>
#include <stdlib.h>
#include <assert.h>

#include <pt-internal.h>

pthread_mutex_t __pthread_key_lock;

void (**__pthread_key_destructors) (void *arg);
int __pthread_key_size;
int __pthread_key_count;
int __pthread_key_invalid_count;

int
pthread_key_create (pthread_key_t *key, void (*destructor) (void *))
{
  /* Where to look for the next key slot.  */
  static int index;

  __pthread_key_lock_ready ();

  __pthread_mutex_lock (&__pthread_key_lock);

 do_search:
  /* Use the search hint and try to find a free slot.  */
  for (; index < __pthread_key_count
	 && __pthread_key_destructors[index] != PTHREAD_KEY_INVALID;
       index ++)
    ;

  /* See if we actually found a free element.  */
  if (index < __pthread_key_count)
    {
      assert (__pthread_key_destructors[index] == PTHREAD_KEY_INVALID);
      assert (__pthread_key_invalid_count > 0);

      __pthread_key_invalid_count --;
      __pthread_key_destructors[index] = destructor;
      *key = index ++;

      __pthread_mutex_unlock (&__pthread_key_lock);
      return 0;
    }

  assert (index == __pthread_key_count);

  /* No space at the end.  */
  if (__pthread_key_size == __pthread_key_count)
    {
      /* See if it is worth looking for a free element.  */
      if (__pthread_key_invalid_count > 4
	  && __pthread_key_invalid_count > __pthread_key_size / 8)
	{
	  index = 0;
	  goto do_search;
	}


      /* Resize the array.  */
      {
	void *t;
	int newsize;

	if (__pthread_key_size == 0)
	  newsize = 8;
	else
	  newsize = __pthread_key_size * 2;

	t = realloc (__pthread_key_destructors,
		     newsize * sizeof (*__pthread_key_destructors));
	if (! t)
	  {
	    __pthread_mutex_unlock (&__pthread_key_lock);
	    return ENOMEM;
	  }

	__pthread_key_size = newsize;
	__pthread_key_destructors = t;
      }
    }

  __pthread_key_destructors[index] = destructor;
  *key = index;

  index ++;
  __pthread_key_count ++;

  __pthread_mutex_unlock (&__pthread_key_lock);
  return 0;
}
