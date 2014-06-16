/** @start 1 */
/* readline.c -- implement readline() for architectures without it */
/** @end 1 */
/*   Copyright (C) 2000, 2006 Gary V. Vaughan
  
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
  
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
  
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
   MA 02110-1301, USA.
*/

#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>

#if STDC_HEADERS || HAVE_STDDEF_H
#  include <stddef.h>
#endif	/* !__STDC__ */

/** @start 1 */
#ifndef BUFSIZ
#  define BUFSIZ 256
#endif

char *
readline (char *prompt)
{
  int lim = BUFSIZ;
  int i = 0;
  int isdone = 0;
  char *buf;
  
  printf (prompt);

  buf = (char *) malloc (lim);
      
  while (!isdone)
    {
      int c = getc (stdin);

      switch (c)
	{
	case EOF:
	  isdone = 1;
	  break;

	case '\n':
	  isdone = 1;
	  break;
	  
	default:
	  if (i == lim)
	    {
	      lim *= 2;
	      buf = (char *) realloc (buf, lim);
	    }
	  buf[i++] = (char) c;
	  break;
	}
    }
  buf[i] = 0;

  return *buf ? buf : NULL;
}
/** @end 1 */
