libtoolize
#Libtool comes with a tool called libtoolize that will install libtool's supporting files into a package. Running this command will install `ltmain.sh'. You should execute it before aclocal and automake. 
aclocal
#Because `configure.in' contains macro invocations which are not known to autoconf itself--AM_INIT_AUTOMAKE being a case in point, it is necessary to collect all of the macro definitions for autoconf to use when generating `configure'. This is done using the aclocal program, so called because it generates `aclocal.m4'. If you were to examine the contents of `aclocal.m4', you would find the definition of the AM_INIT_AUTOMAKE macro contained within. 
autoconf
touch NEWS README AUTHORS ChangeLog
autoheader
automake --add-missing
#The `--add-missing' option copies some boilerplate files from your Automake installation into the current directory.
