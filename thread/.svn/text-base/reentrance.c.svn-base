/* non-reentrant function */
char *strtoupper(char *string)
{
                static char buffer[MAX_STRING_SIZE];
                int index;

                for (index = 0; string[index]; index++)
                                buffer[index] = toupper(string[index]);
                buffer[index] = 0

                return buffer;
}

/* reentrant function (a better solution) */
char *strtoupper_r(char *in_str, char *out_str)
{
                int index;

                for (index = 0; in_str[index]; index++)
                out_str[index] = toupper(in_str[index]);
                out_str[index] = 0

                return out_str;
}

/* non-reentrant function */
char lowercase_c(char *string)
{
                static char *buffer;
                static int index;
                char c = 0;

                /* stores the string on first call */
                if (string != NULL) {
                                buffer = string;
                                index = 0;
        }

                /* searches a lowercase character */
                for (; c = buffer[index]; index++) {
                                if (islower(c)) {
                                                index++;
                                                break;
                }
        }
                return c;
}

/* reentrant function */
char reentrant_lowercase_c(char *string, int *p_index)
{
                char c = 0;

                /* no initialization - the caller should have done it */

                /* searches a lowercase character */
                for (; c = string[*p_index]; (*p_index)++) {
                                if (islower(c)) {
                                                (*p_index)++;
                                                break;
                  }
        }
                return c;
}

----------

char *my_string;
char my_char;
int my_index;
...
my_index = 0;
while (my_char = reentrant_lowercase_c(my_string, &my_index)) {
        ...
}

-----------------------

/* thread-unsafe function */
int increment_counter()
{
                static int counter = 0;

                counter++;
                return counter;
}

/* pseudo-code threadsafe function */
int increment_counter();
{
                static int counter = 0;
        static lock_type counter_lock = LOCK_INITIALIZER;

        pthread_mutex_lock(counter_lock);
                counter++;
        pthread_mutex_unlock(counter_lock);
                return counter;
}


----------

/*In the following piece of C code, the function is thread-safe, but not reentrant:
*/

#include <pthread.h>
 
int increment_counter ()
{
	static int counter = 0;
	static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
 
	pthread_mutex_lock(&mutex);
 
	// only allow one thread to increment at a time
	++counter;
	// store value before any other threads increment it further
	int result = counter;	
 
	pthread_mutex_unlock(&mutex);
 
	return result;
}
