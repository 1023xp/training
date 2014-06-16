/* syntax.h -- create and administer syntax tables
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
#ifndef SIC_SYNTAX_H
#define SIC_SYNTAX_H 1

#include <sic/common.h>

/** @start 1 */
BEGIN_C_DECLS

/** @end 1 */
struct sic;

typedef struct {
  char *start;          /* start of buffered string */
  size_t i;             /* offset to next unused char */
  size_t lim;           /* upper limit for I */
} Buffer;

typedef struct {
  Buffer buf;           /* the buffer details */
  char *start;          /* start of the current command */
} BufferIn;

typedef struct {
  Buffer buf;           /* the buffer details */
  int *offv;            /* vector of offsets */
  size_t offc;          /* count of offsets in OFFV */
  size_t argind;        /* offset to start of current argument */
} BufferOut;

/** @start 1 */
typedef int SyntaxHandler (struct sic *sic, BufferIn *in,
                           BufferOut *out);

/** @end 1 */
typedef int SyntaxInit    (struct sic *sic);
typedef int SyntaxFinish  (struct sic *sic, BufferIn *in,
                           BufferOut *out);

typedef struct syntaxnode SyntaxTable;

/** @start 1 */
typedef struct syntax {
  SyntaxHandler *handler;
  char *ch;
} Syntax;

extern int syntax_install (struct sic *sic, Syntax *table);
extern SyntaxHandler *syntax_handler (struct sic *sic, int ch);

END_C_DECLS
/** @end 1 */
#endif /* !SIC_SYNTAX_H */
