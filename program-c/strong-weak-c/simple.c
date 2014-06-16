/* 
 * References to the weakref function are converted into references of the
 * target name. If the target name is not defined in the current translation
 * unit and it is not referenced directly or otherwise in a way that requires
 * a definition of the target, for example if it is only referenced by using
 * weakref functions, the reference is weak. 
 */

static __attribute__ ((weakref("test"))) void foo();

int main()
{
        if (foo)
               foo();
}

