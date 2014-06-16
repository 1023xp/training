export SHELL = /bin/bash

#
# Default set to ARM
# for x86, set ARCH=x86, CROSS_COMPILE=
#
export ARCH ?= arm
export CROSS_COMPILE ?= arm-linux-gnueabihf-

#
# shell commands
#
CPU_COUNT= $(shell grep process /proc/cpuinfo  | wc -l)
J ?= $(CPU_COUNT)
MAKE  = make -j $(J)
MKDIR = mkdir -p 2>/dev/null
ECHO  = echo
CD    = cd
MOVE  = mv
CAT   = cat
PATCH = patch
CP    = cp -frp
RM    = rm -rf
LNDIR = lndir

export TOPDIR = $(shell pwd)
export UBUNTUDIR = $(TOPDIR)/ubuntu-base/$(ARCH)

#
# source directory
#
export APPDIR = $(TOPDIR)/apps

#
# out directory
#
export OUTDIR = $(TOPDIR)/out/$(ARCH)
export OUTOBJ = $(OUTDIR)/obj
export OUTAPP = $(OUTOBJ)/apps
export IMGRFS = $(OUTDIR)/rootfs

export CC = $(CROSS_COMPILE)gcc
export CXX = $(CROSS_COMPILE)g++
export AS = $(CROSS_COMPILE)as
export CFLAGS = -I$(UBUNTUDIR)/include
export LDFLAGS = -L$(UBUNTUDIR)/usr/lib
