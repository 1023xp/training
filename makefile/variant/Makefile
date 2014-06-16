ifdef DEFINE_VRE
	VRE = “Hello World!”
else
endif

ifeq ($(OPT),define)
	VRE ?= “Hello World! First!”
endif

ifeq ($(OPT),add)
	VRE += “Kelly!”
endif

ifeq ($(OPT),recover)
	VRE := “Hello World! Again!”
endif

x = foo
y = $(x) bar
x = xyz

x1 := foo
y1 := $(x1) bar
x1 := xyz
all:
	@echo $(VRE) $(y) $(y1)
