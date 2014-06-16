/* sic.c -- read commands, evaluate them and print the results
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

#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include <signal.h>

#include "sic.h"

static int sic_init  (Sic *sic);

/** @start 1 */
int
main (int argc, char * const argv[])
{
  int result = EXIT_SUCCESS;
  Sic *sic = sic_new ();
  
  /* initialise the system */
  if (sic_init (sic) != SIC_OKAY)
      sic_fatal ("sic initialisation failed");
  signal (SIGINT, SIG_IGN);
  setbuf (stdout, NULL);

  /* initial symbols */
  sicstate_set (sic, "PS1", "] ", NULL);
  sicstate_set (sic, "PS2", "- ", NULL);
  
  /* evaluate the input stream */
  evalstream (sic, stdin);

  exit (result);
}
/** @end 1 */

static int
sic_init (Sic *sic)
{
  int status = SIC_ERROR;

  /* Push syntax initialisation and finishing functions onto their
     respective stacks. */
  sic->syntax_init   = list_cons (list_new (syntax_init),
				  sic->syntax_init);
  sic->syntax_finish = list_cons (list_new (syntax_finish),
				  sic->syntax_finish);

  /* Initialise the `builtin' functions. */
  status = builtin_install (sic, builtin_table);

  /* Initialise the `syntax' tables. */
  if (status == SIC_OKAY)
    status = syntax_install (sic, syntax_table);

  return status;
}
