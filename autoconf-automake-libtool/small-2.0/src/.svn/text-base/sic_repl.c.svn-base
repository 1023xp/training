/* sic_repl.c -- read commands, evaluate them and print the results
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

#include "common.h"

#ifdef HAVE_LIBREADLINE
#  include <readline/readline.h>
#  include <readline/history.h>
#endif

#include "eval.h"
#include "sic.h"

static char *getline (FILE *in, const char *prompt);

int
evalstream (Sic *sic, FILE *stream)
{
  int status = SIC_EXIT;
  char *line;

  line = getline (stream, sicstate_get (sic, "PS1"));
  
  while (line)
    {
      status = evalline (sic, &line);

      switch (status)
	{
	case SIC_INCOMPLETE:
	  {
	    char *buf = xstrdup (line);
	    int len = strlen (buf);

	    line = getline (stream, sicstate_get (sic, "PS2"));
	    len += strlen (line);

	    buf = XREALLOC (char, buf, 2 + len);
	    strcat (buf, "\n");
	    strcat (buf, line);

	    line = buf;
	  }
	  break;

	case SIC_EXIT:
	  goto exit_sic;
	  /*NOBREAK*/
      
	case SIC_ERROR:
	  fprintf (stderr, "%s.\n", sic_result_get (sic));
	  /*NOBREAK*/

	default:
	  line = getline (stream, sicstate_get (sic, "PS1"));
	  break;
	}

      sic_result_clear (sic);
    }
  
  if (!line)
    printf ("\n");
  
 exit_sic:
  return status;
}

int
evalline (Sic *sic, char **pline)
{
  int status = SIC_OKAY;
  char *command = *pline;
  
  while (status != SIC_INCOMPLETE && *command)
    {
      Tokens *tokens;

      status = tokenize (sic, &tokens, &command);

      if ((status == SIC_OKAY)
	  && tokens && tokens->argv && tokens->argv[0] && *tokens->argv[0])
	status = eval (sic, tokens);

      if (tokens)
	{
	  free (*tokens->argv);
	  XFREE (tokens->argv);
	  XFREE (tokens);
	}
    }

  if (status != SIC_INCOMPLETE)
    *pline = command;
  
  return status;
}

/** @start 1 */
static char *
getline (FILE *in, const char *prompt)
{
  static char *buf = NULL;	/* Always allocated and freed
				   from inside this function.  */
  XFREE (buf);

  buf = (char *) readline ((char *) prompt);

#ifdef HAVE_ADD_HISTORY
  if (buf && *buf)
    add_history (buf);
#endif
  
  return buf;
}
/** @end 1 */
