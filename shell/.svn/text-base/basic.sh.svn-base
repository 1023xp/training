#!/bin/sh
#由反引号括起来的也是一条命令，Shell先执行该命令，然后将输出结果立刻代换到当前命令行中。例如定义一个变量存放date命令的输出：
echo `date`
#命令代换也可以用$()表示：
echo $(date)

#如果一个变量叫做VARNAME，用${VARNAME}可以表示它的值，在不引起歧义的情况下也可以用$VARNAME表示它的值。通过以下例子比较这两种表示法的不同：
echo $SHELL
echo $SHELLabc
echo ${SHELL}abc

#单引号用于保持引号内所有字符的字面值，即使引号内的\和回车也不例外，但是字符串中不能出现单引号
echo '$SHELL'
echo "$SHELL"
echo '$SHELL
	  hello world'
#双引号用于保持引号内所有字符的字面值（回车也不例外），除以下情况外：
#$加变量名可以取变量的值
#反引号仍表示命令替换
#\$表示$的字面值
#\`表示`的字面值
#\"表示"的字面值
#\\表示\的字面值
echo "$SHELL
	  hello world"
echo "\$SHELL
	  hello world"

#命令test或[可以测试一个条件是否成立，如果测试结果为真，则该命令的Exit Status为0，如果测试结果为假，则命令的Exit Status为1
VAR=2
test $VAR -gt 1
echo $?

test $VAR -gt 3
echo $?

[ $VAR -gt 3 ]
echo $?
#存在Desktop目录且VAR等于abc
VAR=abc
[ -d Desktop -a $VAR = 'abc' ]
echo $?

#在Shell中用if、then、elif、else、fi这几条命令实现分支控制
echo "Is it morning? Please answer yes or no."
read YES_OR_NO
if [ "$YES_OR_NO" = "yes" ]; then
	echo "Good morning!"
elif [ "$YES_OR_NO" = "no" ]; then
	echo "Good afternoon!"
else
	echo "Sorry, $YES_OR_NO not recognized. Enter yes or no."
	exit 1
fi

#C语言的case只能匹配整型或字符型常量表达式，而Shell脚本的case可以匹配字符串和Wildcard，每个匹配分支可以有若干条命令，末尾为;;
echo "Is it morning? Please answer yes or no."
read YES_OR_NO
case "$YES_OR_NO" in
	yes|y|Yes|YES)
	echo "Good Morning!";;
	[nN]*)
	echo "Good Afternoon!";;
	*)
	echo "Sorry, $YES_OR_NO not recognized. Enter yes or no."
	exit 1;;
esac

#利用循环计算10的阶乘
factorial=1
for a in `seq 1 10`
do
       factorial=`expr $factorial \* $a`
done
echo "10! = $factorial"

#FRUIT是一个循环变量，第一次循环$FRUIT的取值是apple，第二次取值是banana，第三次取值是pear
for FRUIT in apple banana pear; do
	  echo "I like $FRUIT"
done

#while的用法和C语言类似
echo "Enter password:"
read TRY
while [ "$TRY" != "secret" ]; do
	echo "Sorry, try again"
	read TRY
done

COUNTER=1
while [ "$COUNTER" -lt 10 ]; do
	echo "COUNTER is $COUNTER"
	COUNTER=$(($COUNTER+1))
done

#$0	相当于C语言main函数的argv[0]
echo "The program $0 is now running"
#$1、$2...	这些称为位置参数（Positional Parameter），相当于C语言main函数的argv[1]、argv[2]...
echo "The first parameter is $1"
echo "The second parameter is $2"
#$@	表示参数列表"$1" "$2" ...，例如可以用在for循环中的in后面
for PARAM in $@; do
	echo "PARAM $PARAM"
done

echo "The parameter list is $@"
#位置参数可以用shift命令左移
shift
echo "The first parameter is $1"
echo "The second parameter is $2"
echo "The parameter list is $@"

#数就像是迷你脚本，调用函数时可以传任意个参数，在函数内同样是用$0、$1、$2等变量来提取参数
#函数中的位置参数相当于函数的局部变量，改变这些变量并不会影响函数外面的$0、$1、$2等变量
#函数中可以用return命令返回，如果return后面跟一个数字则表示函数的Exit Status

is_directory()
{
	DIR_NAME=$1
	if [ ! -d $DIR_NAME ]; then
		return 1
	else
		return 0
	fi
}

for DIR in "$@"; do
	if is_directory "$DIR"
	then :
	else
		echo "$DIR doesn't exist. Creating it now..."
		mkdir $DIR > /dev/null 2>&1
		if [ $? -ne 0 ]; then
			echo "Cannot create directory $DIR"
			exit 1
		fi
	fi
done

multiple_times()
{
	result=1
	while [ $# -gt 0 ]
	do
		result=$(($result * $1))
     		shift
	done
	return $result
}

multiple_times 22 35 76
echo return value: $?
