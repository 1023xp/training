/* error.c -- display formatted error diagnostics of varying severity
   Copyright (C) 2000, 2006 Gary V. Vaughan
  
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

#include "common.h"
#include "error.h"

static void error (int exit_status, const char *mode, 
		   const char *message);

/** @end 1 */
/** @start 2 */
const char *program_name = NULL;

void
set_program_name (const char *path)
{
  if (!program_name)
    program_name = xstrdup (basename (path));
}

/** @end 2 */
/** @start 1 */
static void
error (int exit_status, const char *mode, const char *message)
{
  fprintf (stderr, "%s: %s: %s.\n", program_name, mode, message);

  if (exit_status >= 0)
    exit (exit_status);
}

void
sic_warning (const char *message)
{
  error (-1, "warning", message);
}

void
sic_error (const char *message)
{
  error (-1, "ERROR", message);
}

void
sic_fatal (const char *message)
{
  error (EXIT_FAILURE, "FATAL", message);
}
/** @end 1 */
