#所有的源文件--  .c文件列表
SRCS = $(wildcard *.c)
#
#所有的.o文件列表
OBJS =$(SRCS:.c=.o)
#
CFLAGS =-I./include

test: $(OBJS)
	gcc -o $@ $^
#
#生成依赖信息
MAKEDEPEND = gcc -MM    
                    
#生成.o的对自己目录中.h .c的依赖信息到.depend文件中
.depend:
	gcc -MM  $(SRCS) $(CFLAGS) > $@

#引入了.o文件对.c和.h的依赖情况。以后.h被修改也会重新make
-include .depend    

$(OBJS) : %.o:%.c
	$(CC) -c $< -o $@ $(CFLAGS)

#忽略命令的出错，我们可以在Makefile的命令行前加一个减号“-”（在Tab键之后），标记为不管命令出不出错都认为是成功
clean:
	-rm *.o
	-rm test
