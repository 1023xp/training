/* xstrerror.c -- strerror wrapper with bound checking
   Fri Jun 16 18:30:00 1995  Pat Rankin  <rankin@eql.caltech.edu>
   This code is in the public domain.
*/

#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>

extern char *strerror ();

/* If strerror returns NULL, we'll format the number into a static buffer.  */
#define ERRSTR_FMT "undocumented error #%d"
static char xstrerror_buf[sizeof ERRSTR_FMT + 20];

/* Like strerror, but result is never a null pointer.  */
char *
xstrerror (int errnum)
{
  char *errstr = strerror (errnum);

  /* If `errnum' is out of range, result might be NULL.  We'll fix that.  */
  if (!errstr)
    {
      sprintf (xstrerror_buf, ERRSTR_FMT, errnum);
      errstr = xstrerror_buf;
    }

  return errstr;
}
