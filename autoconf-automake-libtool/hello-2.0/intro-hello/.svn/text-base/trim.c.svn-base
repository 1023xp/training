#include <string.h>

#define WHITESPACE_STR  " \f\n\r\t\v"

/**
 * Remove whitespace characters from both ends of a copy of
 *  '\0' terminated STRING and return the result.
 **/
char *
trim (char *string)
{
  char *result = 0;

  /* Ignore NULL pointers.  */
  if (string)
    {
      char *ptr = string;

      /* Skip leading whitespace.  */
      while (strchr (WHITESPACE_STR, *ptr))
        ++ptr;

      /* Make a copy of the remainder.  */
      result = strdup (ptr);

      /* Move to the last character of the copy.  */
      for (ptr = result; *ptr; ++ptr)
        /* NOWORK */;
      --ptr;

      /* Remove trailing whitespace.  */
      for (--ptr; strchr (WHITESPACE_STR, *ptr); --ptr)
          *ptr = '\0';
   }

  return result;
}

