/* sic.h -- funxtion prototypes for sic shell application
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
#ifndef SIC_H
#define SIC_H 1

#include <sic/common.h>
#include <sic/sic.h>
#include <sic/builtin.h>

BEGIN_C_DECLS

extern Syntax syntax_table[];
extern Builtin builtin_table[];
extern Syntax syntax_table[];

extern int evalstream    (Sic *sic, FILE *stream);
extern int evalline      (Sic *sic, char **pline);
extern int source        (Sic *sic, const char *path);
extern int syntax_init   (Sic *sic);
extern int syntax_finish (Sic *sic, BufferIn *in, BufferOut *out);

END_C_DECLS

#endif /* !SIC_H */
/** @end 1 */
