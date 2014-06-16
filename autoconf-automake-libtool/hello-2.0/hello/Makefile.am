## Process this file with automake to produce Makefile.in

lib_LTLIBRARIES		= libhello.la
libhello_la_SOURCES	= hello.c
libhello_la_LDFLAGS	= -version-info 0:0:0

bin_PROGRAMS		= hello
hello_SOURCES		= main.c
hello_LDADD		= libhello.la
