/* builtin.c -- installing and removing sic builtin commands
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

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include "builtin.h"

struct builtintab {
  struct builtintab *next;
  Builtin *table;
};

int
builtin_install (Sic *sic, Builtin *table)
{
  int status = SIC_ERROR;
  
  if (table)
    {
      BuiltinTab *new = XMALLOC (BuiltinTab, 1);
      new->next = sic->builtins;
      new->table = table;
      sic->builtins = new;
      status = SIC_OKAY;
    }

  return status;
}

int
builtin_remove (Sic *sic, Builtin *table)
{
  int status = SIC_ERROR;
  
  if (sic->builtins && table)
    {
      BuiltinTab *stale = NULL;

      if (sic->builtins->table == table)
	{
	  stale = sic->builtins;
	  sic->builtins = sic->builtins->next;
	  status = SIC_OKAY;
	}
      else
	{
	  BuiltinTab *p;
	  
	  for (p = sic->builtins; p->next; p = p->next)
	    if (p->next->table == table)
	      {
		stale = p->next;
		p->next = p->next->next;
		status = SIC_OKAY;
		break;
	      }
	}

      XFREE (stale);
    }
  
  return status;
}

Builtin *
builtin_find (Sic *sic, const char *name)
{
  if (sic->builtins)
    {
      BuiltinTab *p;
      for (p = sic->builtins; p; p = p->next)
	{
	  int i;
	  for (i = 0; p->table[i].name; ++i)
	    if (strcmp (p->table[i].name, name) == 0)
	      return &p->table[i];
	}
    }

  return NULL;
}
