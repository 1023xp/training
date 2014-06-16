# @start 1
# configure.in -- Process this file with autoconf to produce configure
# @end 1
# Copyright (C) 2000, 2006 Gary V. Vaughan
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
# MA 02110-1301, USA.

# @start 2

AC_PREREQ([2.59])

AC_INIT([sic], [0.5.1], [gary@gnu.org])
AC_CONFIG_HEADERS([config.h:config-h.in])
AC_CONFIG_SRCDIR([sic/eval.c])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([1.9 foreign])

AC_PROG_CC
AC_PROG_RANLIB

AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([errno.h stddef.h stdlib.h string.h strings.h unistd.h])

AC_C_CONST
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_C_VOLATILE

SIC_WITH_READLINE

SIC_VAR_SYS_ERRLIST
SIC_VAR_SYS_SIGLIST

AC_CHECK_FUNCS([bzero memset], [break])
AC_CHECK_FUNCS([calloc strchr strrchr])
AC_REPLACE_FUNCS([basename strspn strcspn strerror strsignal waitpid])

save_LIBS="$LIBS"
test x"${with_readline-no}" != xno && LIBS="$LIBS -lreadline"
AC_REPLACE_FUNCS([readline])
AC_CHECK_FUNCS([add_history])
LIBS="$save_LIBS"

AC_CONFIG_FILES([Makefile replace/Makefile sic/Makefile src/Makefile],
	[test -z "$CONFIG_HEADERS" || echo timestamp > sic/stamp-h.in])
AC_OUTPUT
# @end 2
