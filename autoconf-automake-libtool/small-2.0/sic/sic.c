/* sic.c -- create and maintain sic ADT
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
#include "sic.h"
#include "syntax.h"

#define CHUNK_SIZE	256

static SicState *sicstate_find (Sic *sic, const char *key);
	
Sic *
sic_new (void)
{
  Sic *new = XCALLOC (Sic, 1);
  return new;
}

Sic *
sic_delete (Sic *stale)
{
  free (stale->result);
  free (stale);

  return 0;
}

int
sicstate_set (Sic *sic, const char *key, void *value, 
	      void (*delete) (void *value))
{
  SicState *state = sicstate_find (sic, key);

  if (state)
    {
      if (state->delete)
	(*state->delete) (state->data);
    }
  else
    {
      state = XMALLOC (SicState, 1);

      state->next = sic->state;
      sic->state  = state;

      state->key  = xstrdup (key);
    }

  state->data   = value;
  state->delete = delete;

  return 0;
}

void *
sicstate_get (Sic *sic, const char *key)
{
  SicState *state = sicstate_find (sic, key);
  return state ? state->data : NULL;
}

int
sicstate_clear (Sic *sic, const char *key)
{
  SicState *state = sic->state;
  SicState *stale = NULL;

  if (!state)
    return -1;

  if (strcmp (state->key, key) == 0)
    {
      stale = state;
      sic->state = state->next;
    }
  else
    for (; state->next; state = state->next)
      if (strcmp (state->next->key, key) == 0)
	{
	  stale = state->next;
	  state->next = stale->next;
	  break;
	}

  if (!stale)
    return -1;

  if (stale->delete)
    (*stale->delete) (stale->data);
  XFREE (stale->key);
  XFREE (stale);

  return 0;
}

Sic *
sic_result_clear (Sic *sic)
{
  sic->len = 0;
  if (sic->result)
    sic->result[0] = 0;
  else
    sic->result = 0;
  
  return sic;
}

const char *
sic_result_get (Sic *sic)
{
  return sic->result;
}

Sic *
sic_result_set (Sic *sic, const char *value, size_t len)
{
  if (len < 0) len = strlen (value);
  
  sic_result_clear (sic);
  
  if (len < CHUNK_SIZE)
    {
      sic->lim = CHUNK_SIZE;
      sic->result = XREALLOC (char, sic->result, sic->lim);
    }
  
  return sic_result_append (sic, value);
}

Sic *
sic_result_append (Sic *sic, const char *value)
{
  size_t len;
  
  if (!value) return sic;

  len = strlen (value);
  if (sic->len + len >= sic->lim)
    {
      sic->lim += CHUNK_SIZE;
      sic->result = XREALLOC (char, sic->result, sic->lim);
    }

  strncpy (&sic->result[sic->len], value, len);
  sic->len += len;
  sic->result[sic->len] = 0;

  return sic;
}

static SicState *
sicstate_find (Sic *sic, const char *key)
{
  SicState *state;

  for (state = sic->state; state; state = state->next)
    if (strcmp (state->key, key) == 0)
      break;

  return state;
}
	
