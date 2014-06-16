/** @start 1 */
/* strcspn.c -- implement strcspn() for architectures without it */
/** @end 1 */
/* Copyright (C) 2000, 2006 Gary V. Vaughan
  
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
/** @start 1 */

#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include <sys/types.h>

#if STDC_HEADERS
#  include <string.h>
#elif HAVE_STRINGS_H
#  include <strings.h>
#endif

#if !HAVE_STRCHR
#  ifndef strchr
#    define strchr index
#  endif
#endif

size_t
strcspn (const char *string, const char *reject)
{
  size_t count = 0;
  while (strchr (reject, *string) == 0)
    ++count, ++string;

  return count;
}
/** @end 1 */
