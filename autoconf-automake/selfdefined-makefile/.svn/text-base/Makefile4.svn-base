BIN = test
CC = gcc
INCS =
LIBS =
SUBDIRS = main init
#
#
#所有的源文件--  .c文件列表
SRCS = $(wildcard *.c ./main/*.c ./init/*.c)
#
#所有的.o文件列表
OBJS =$(SRCS:.c=.o)

#
#生成依赖信息
MAKEDEPEND = gcc -MM    
CFLAGS = $(INCS)
                    
all: $(BIN)

#生成.o的对自己目录中.h .c的依赖信息到.depend文件中
.depend:
	$(MAKEDEPEND) $(SRCS) > $@

#重新生成依赖信息
depend:
	rm .depend
	$(MAKE) .depend

$(OBJS) : %.o:%.c
	$(CC) -c $< -o $@ $(CFLAGS)

$(BIN) : $(OBJS)
	$(CC) -o $(BIN) $(OBJS) $(LIBS)

#引入了.o文件对.c和.h的依赖情况。以后.h被修改也会重新make
include .depend    
.PHONY:clean cleanall

#清除所有目标文件以及生成的最终目标文件
clean:            
	rm $(BIN) $(OBJS)

cleanall:
	rm .depend
	rm $(BIN) $(OBJS)
