#include <stdio.h>

extern char *trim ();
extern void free ();

void
hello (char *who)
{
  char *trimmed = trim (who);
  printf ("Hello, %s!\n", trimmed);
  free (trimmed);
}

