/* eval.c -- create and dispatch command vectors
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
#include "builtin.h"
#include "error.h"
#include "eval.h"
#include "sic.h"
#include "syntax.h"

static void bufferin_init  (BufferIn *in, char *command);
static void bufferout_init (BufferOut *out, size_t lim);

int
eval (Sic *sic, Tokens *tokens)
{
  Builtin *builtin;
  
  builtin = builtin_find (sic, tokens->argv[0]);

  if (!builtin)
    builtin = builtin_find (sic, "unknown");

  if (!builtin)
    {
      sic_result_clear (sic);
      sic_result_append (sic, "command \"");
      sic_result_append (sic, tokens->argv[0]);
      sic_result_append (sic, "\" not found");
      return SIC_ERROR;
    }

  /* The table expresses valid counts of arguments, not including
     the command iteslf, hence the `-1'.  */
  if (tokens->argc -1 < builtin->min ||
      (tokens->argc -1> builtin->max && builtin->max != -1))
    {
      sic_result_set (sic, "wrong number of arguments", -1);
      return SIC_ERROR;
    }

  return (*builtin->func) (sic, tokens->argc, tokens->argv);
}

int
untokenize (Sic *sic, char **pcommand, Tokens *tokens)
{
  char *command = NULL;
  int i, off;

  if (tokens->lim < 1)
    {
      for (tokens->lim = i = 0; tokens->argv[i]; ++i)
	tokens->lim += 1 + strlen (tokens->argv[i]);
    }

  command = XMALLOC (char, tokens->lim);
  
  for (off = i = 0; tokens->argv[i]; ++i)
    {
      sprintf (&command[off], "%s ", tokens->argv[i]);
      off += 1 + strlen (tokens->argv[i]);
    }
  command[off -1] = 0;
  
  *pcommand = command;

  return SIC_OKAY;
}

int
tokenize (Sic *sic, Tokens **ptokens, char **pcommand)
{
  BufferIn in;
  BufferOut out;
  List *head;
  int status = SIC_CONTINUE;

  bufferin_init (&in, *pcommand);
  bufferout_init (&out, in.buf.lim);

  /* Perform any user initialisation for syntax readers. */
  for (head = sic->syntax_init; head; head = head->next)
    (*(SyntaxInit *) head->userdata) (sic);

  /* Dispatch to handlers by syntax class of character, or
     simply copy from input to output by default. */
  while (status == SIC_CONTINUE)
    {
      SyntaxHandler *handler = syntax_handler (sic, in.buf.start[in.buf.i]);

      if (handler)
	status = (*handler) (sic, &in, &out);
      else
	out.buf.start[out.buf.i++] = in.buf.start[in.buf.i++];
    }

  /* Perform any client finalisation for syntax reader. */
  for (head = sic->syntax_finish; head; head = head->next)
    (*(SyntaxFinish *) head->userdata) (sic, &in, &out);
	
  {
    /* Can't fill ARGV on the fly incase BUF moved during realloc. */
    Tokens *tokens = XMALLOC (Tokens, 1);
    
    tokens->argv = XMALLOC (char *, 1 + out.offc);

    for (tokens->argc = 0; tokens->argc < out.offc; ++tokens->argc)
      tokens->argv[tokens->argc] = out.buf.start + out.offv[tokens->argc];

    tokens->argv[tokens->argc] = NULL;
    tokens->lim  = out.buf.lim;

    if (!*tokens->argv)
      XFREE(out.buf.start);
    XFREE (out.offv);

    *pcommand += in.buf.i;
    *ptokens = tokens;
  }

  return status;
}

static void
bufferin_init (BufferIn *in, char *command)
{
  in->buf.lim	= 1 + strlen (command);
  in->buf.start	= command;
  in->buf.i	= 0;

  in->start	= in->buf.start;
}

static void
bufferout_init (BufferOut *out, size_t lim)
{
  out->buf.lim	= lim;
  out->buf.start= XMALLOC (char, lim);
  out->buf.i	= 0;

  out->offc	= 0;
  out->offv	= NULL;
  out->argind	= out->buf.i;
}
