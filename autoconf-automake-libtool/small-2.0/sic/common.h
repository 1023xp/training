/* common.h -- Process this file with configure to produce common.h
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
#ifndef SIC_COMMON_H
#define SIC_COMMON_H 1

#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>
#include <sys/types.h>

#if STDC_HEADERS
#  include <stdlib.h>
#  include <string.h>
#elif HAVE_STRINGS_H
#  include <strings.h>
#endif /*STDC_HEADERS*/

#if HAVE_UNISTD_H
#  include <unistd.h>
#endif

/** @end 1 */
#if HAVE_SYS_WAIT_H
#  include <sys/wait.h>
#endif
#ifndef WIFEXITED
#  define WIFEXITED(stat)       (((stat) & 0xff) == 0)
#endif
#ifndef WEXITSTATUS
#  define WEXITSTATUS(stat)     ((unsigned)(stat) >> 8)
#endif
#ifndef WIFSTOPPED
#  define WIFSTOPPED(stat)      (((stat) & 0xff) == 0x7f)
#endif
#ifndef WSTOPSIG
#  define WSTOPSIG(stat)        WEXITSTATUS(stat)
#endif
#ifndef WIFSIGNALED
#  define WIFSIGNALED(stat)     (!WIFEXITED(stat) && !WIFSTOPPED(stat))
#endif
#ifndef WTERMSIG
#  define WTERMSIG(stat)        ((stat) & 0x7f)
#endif

/** @start 1 */
#if HAVE_ERRNO_H
#  include <errno.h>
#endif /*HAVE_ERRNO_H*/
#ifndef errno
/* Some systems #define this! */
extern int errno;
#endif

/** @end 1 */
/** @start 4 */
#ifdef __cplusplus
#  define BEGIN_C_DECLS         extern "C" {
#  define END_C_DECLS           }
#else
#  define BEGIN_C_DECLS
#  define END_C_DECLS
#endif

/** @end 4 */
#ifdef __GNUC__
#  ifndef const
#    define const       __const
#  endif
#  ifndef signed
#    define signed      __signed
#  endif
#  ifndef volatile
#    define volatile    __volatile
#  endif
#else
#  ifdef __STDC__
#    undef  signed
#    define signed
#    undef  volatile
#    define volatile
#  endif
#endif

#ifdef __STDC__
#define STR(x)          #x
#define CONC(x, y)      x##y
#else
#define STR(x)          "x"
#define CONC(x, y)      x/**/y
#endif

/** @start 2 */
#ifndef EXIT_SUCCESS
#  define EXIT_SUCCESS  0
#  define EXIT_FAILURE  1
#endif
/** @end 2 */
#if !HAVE_BZERO && HAVE_MEMSET
# define bzero(buf, bytes)      ((void) memset (buf, 0, bytes))
#endif

#if !HAVE_STRCHR
#  define strchr index
#endif

#if !HAVE_STRRCHR
#  define strrchr rindex
#endif

/** @start 4 */
#define XCALLOC(type, num)                                  \
        ((type *) xcalloc ((num), sizeof(type)))
#define XMALLOC(type, num)                                  \
        ((type *) xmalloc ((num) * sizeof(type)))
#define XREALLOC(type, p, num)                              \
        ((type *) xrealloc ((p), (num) * sizeof(type)))
#define XFREE(stale)                            do {        \
        if (stale) { free (stale);  stale = 0; }            \
                                                } while (0)

BEGIN_C_DECLS

extern void *xcalloc    (size_t num, size_t size);
extern void *xmalloc    (size_t num);
extern void *xrealloc   (void *p, size_t num);
extern char *xstrdup    (const char *string);
extern char *xstrerror  (int errnum);

/** @end 4 */
#if !HAVE_BASENAME
extern char *basename   (const char *path);
#endif

#if !HAVE_STRCSPN
extern size_t strcspn   (const char *string, const char *accept);
#endif

#if !HAVE_STRERROR
extern char *strerror   (int err);
#endif

#if !HAVE_STRSIGNAL
extern char *strsignal  (int signo);
#endif

#if !HAVE_STRSPN
extern size_t strspn    (const char *string, const char *reject);
#endif

#if !HAVE_WAITPID
extern pid_t waitpid    (pid_t pid, int *pstatus, int options);
#endif
/** @start 4 */
END_C_DECLS
/** @end 4 */
/** @start 1 */
#endif /* !SIC_COMMON_H */
/** @end 1 */
