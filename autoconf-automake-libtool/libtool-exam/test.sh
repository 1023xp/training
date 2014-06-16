#生成各自的o文件
libtool --mode=compile gcc -g -O -c hello.c
libtool --mode=compile gcc -g -O -c fun.c
libtool --mode=compile gcc -g -O -c main.c
#连接成动态库文件
libtool --mode=link gcc -g -O -o libhello.la hello.lo fun.lo -rpath /usr/local/lib -lm 
#连接生成可执行文件test
libtool --mode=link gcc -g -O -o test main.o libhello.la -lm
#安装动态库
libtool --mode=install cp libhello.la /usr/local/lib/
libtool -n --mode=finish /usr/local/lib
libtool --mode=install cp test /usr/local/bin/
libtool -n --mode=finish /usr/local/bin
