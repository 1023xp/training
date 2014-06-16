CXX=g++
LD=g++
JUTILDIR=../jutil

#CXXFLAGS += -pg
#LDFLAGS += -pg

# For coverage testing
# CXXFLAGS += -fprofile-arcs -ftest-coverage
# LDFLAGS += -lgcov

EXMAP_OBJ=Exmap.o Range.o Elf.o

CXXFLAGS += -g -Wall -I$(JUTILDIR)
LDFLAGS += -lpcre -ljutil -L$(JUTILDIR)

GTKCXXFLAGS = `pkg-config --cflags gtkmm-2.4`
GTKLDFLAGS = `pkg-config --libs gtkmm-2.4`

# ------------------------------------------------------------

CL_OBJ =  exmtool.o $(EXMAP_OBJ)
OBJS += $(CL_OBJ)
EXES += exmtool

ET_OBJ = elftool.o Elf.o Range.o
OBJS += $(ET_OBJ)
EXES += elftool

SP_OBJ = showproc.o $(EXMAP_OBJ)
OBJS += $(SP_OBJ)
EXES += showproc

GEM_OBJ =  gexmap.o $(EXMAP_OBJ)
OBJS += $(GEM_OBJ)
EXES += gexmap

# ------------------------------------------------------------

TR_OBJ = t_range.o Range.o
OBJS += $(TR_OBJ)
TESTS +=  t_range

TE_OBJ = t_elf.o Elf.o Range.o
OBJS += $(TE_OBJ)
TESTS +=  t_elf

TP_OBJ = t_pcre.o
OBJS += $(TP_OBJ)
TESTS +=  t_pcre

TX_OBJ = t_exmap.o $(EXMAP_OBJ)
OBJS += $(TX_OBJ)
TESTS +=  t_exmap

TA_OBJ = t_artsd.o $(EXMAP_OBJ)
OBJS += $(TA_OBJ)
TESTS += t_artsd

# ------------------------------------------------------------

EXES += $(TESTS)

EXTRA_DEL_FILES += *~

build: $(EXES) $(SHLIBS)

TAGS:
	etags *.cpp *.hpp

# Goes into html dir, so always makes
doc:
	doxygen

test: $(TESTS) $(EXES) $(SHLIBS)
	$(JUTILDIR)/trun $(TESTS)

gexmap: $(GEM_OBJ)
	$(LD) -o gexmap $(GEM_OBJ) $(LDFLAGS) $(GTKLDFLAGS)

gexmap.o: gexmap.cpp
	$(CC) $(CXXFLAGS) $(GTKCXXFLAGS) -c $<

exmtool: $(CL_OBJ)
	$(LD) -o exmtool $(CL_OBJ) $(LDFLAGS) 

elftool: $(ET_OBJ)
	$(LD) -o elftool $(ET_OBJ) $(LDFLAGS) 

showproc: $(SP_OBJ)
	$(LD) -o showproc $(SP_OBJ) $(LDFLAGS) 

t_range: $(TR_OBJ)
	$(LD) -o t_range $(TR_OBJ) $(LDFLAGS) 

t_elf: $(TE_OBJ)
	$(LD) -o t_elf $(TE_OBJ) $(LDFLAGS) 

t_pcre: $(TP_OBJ)
	$(LD) -o t_pcre $(TP_OBJ) $(LDFLAGS) 

t_exmap: $(TX_OBJ)
	$(LD) -o t_exmap $(TX_OBJ) $(LDFLAGS) 

t_artsd: $(TA_OBJ)
	$(LD) -o t_artsd $(TA_OBJ) $(LDFLAGS) 

clean: cleantags cleandoc
	rm -f $(OBJS) $(EXES) $(SHLIBS) $(EXTRA_DEL_FILES)

cleantags:
	rm -f TAGS

cleandoc:
	rm -rf ./html
