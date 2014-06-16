/* list.h -- maintain lists of types with forward pointer fields
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
#ifndef SIC_LIST_H
#define SIC_LIST_H 1

#include <sic/common.h>

BEGIN_C_DECLS

typedef struct list {
  struct list *next;    /* chain forward pointer*/
  void *userdata;       /* incase you want to use raw `List's */
} List;

extern List *list_new       (void *userdata);
extern List *list_cons      (List *head, List *tail);
extern List *list_tail      (List *head);
extern size_t list_length   (List *head);

END_C_DECLS

#endif /* !SIC_LIST_H */
/** @end 1 */
