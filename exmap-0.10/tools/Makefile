
# Some of what we build goes into shared libs, so play safe
CFLAGS=-fPIC

TS_OBJ = mapper.o
EXES += mapper
OBJS += $(TS_OBJ)

SA_OBJ = sharedarray.o
SHLIBS += libsharedarray.so
OBJS += $(SA_OBJ)

SM_OBJ = sharedarraymain.o
EXES += sharedarray
OBJS += $(SM_OBJ)

AL_OBJ = allocer.o
EXES += allocer
OBJS += $(AL_OBJ)

ML_OBJ = memload.o
EXES += memload
OBJS += $(ML_OBJ)

MI_OBJ = mapit.o
EXES += mapit
OBJS += $(MI_OBJ)

GPS_OBJ = getpagesize.o
EXES += getpagesize
OBJS += $(GPS_OBJ)
EXTRA_DEL_FILES += pagesize.h

EXTRA_DEL_FILES += *~

build: $(EXES) $(SHLIBS)

mapper.o: pagesize.h

mapper: $(TS_OBJ)
	$(CC) -o mapper $(TS_OBJ)

allocer: $(AL_OBJ)
	$(CC) -o allocer $(AL_OBJ)

memload: $(ML_OBJ)
	$(CC) -o memload $(ML_OBJ)

mapit: $(MI_OBJ)
	$(CC) -o mapit $(MI_OBJ)

libsharedarray.so: $(SA_OBJ)
	$(CC) -shared -o libsharedarray.so $(SA_OBJ)

sharedarray.o: pagesize.h

sharedarray: $(SM_OBJ) libsharedarray.so pagesize.h
	$(CC) -o sharedarray $(SM_OBJ) -L. -lsharedarray

getpagesize: $(GPS_OBJ)
	$(CC) -o getpagesize $(GPS_OBJ)

pagesize.h: getpagesize
	./getpagesize > pagesize.h

clean:
	rm -f $(OBJS) $(EXES) $(SHLIBS) $(EXTRA_DEL_FILES)
