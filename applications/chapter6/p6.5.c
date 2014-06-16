#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>

//打开指定的串口
int open_port(char* port){
	int fd;

	if((fd=open(port,O_RDWR | O_NOCTTY | O_NONBLOCK))==-1){
		perror("Cannot open the desired port");	
		return -1;
	}

	return fd;
}
//关闭指定串口
void close_port(int fd){
	close(fd);
}
//get the baud_rate defination according to the baud rate
int get_baud_rate(unsigned long int baud_rate){
	switch(baud_rate){
		case 0:
			return B0;
		case 50:
			return B50;
		case 75:
			return B75;
		case 110:
			return B110;
		case 134:
			return B134;
		case 150:
			return B150;
		case 200:
			return B200;
		case 300:
			return B300;
		case 600:
			return B600;
		case 1200:
			return B1200;
		case 1800:
			return B1800;
		case 2400:
			return B2400;
		case 4800:
			return B4800;
		case 9600:
			return B9600;
		case 19200:
			return B19200;
		case 38400:
		        return B38400;
	        case 57600:
		        return B57600;
	 	case 115200:
	   		return B115200;
		case 230400:
			return B230400;
		default:
			return -1;
	}
}

typedef struct port_info{
	int baud_rate;
	int port_fd;
	char parity;
	char stop_bit;
	char flow_ctrl;
	char data_bits;
}*pport_info;

int set_port(pport_info p_info)
{
	struct termios old_opt,new_opt;
	int baud_rate,parity;

	memset(&old_opt,0,sizeof(old_opt));
	memset(&new_opt,0,sizeof(new_opt));

	cfmakeraw(&new_opt);
	tcgetattr(p_info->port_fd,&old_opt);
	/*=========以下设置串口波特率=========*/
        baud_rate=get_baud_rate(p_info->baud_rate);
	
	//修改new_opt结构中的串口输入/输出波特率参数
	cfsetispeed(&new_opt,baud_rate);
	cfsetospeed(&new_opt,baud_rate);
	
	//修改控制模式，保证程序不会占用串口
	new_opt.c_cflag |=CLOCAL;
	//修改控制模式，使得能够从串口中读取输入数据
 	new_opt.c_cflag |=CREAD;

	/*===========设置数据流控制==========*/
	switch(p_info->flow_ctrl){
		case '0':
			//不使用流控制
			new_opt.c_cflag &=~CRTSCTS;
			break;
		case '1':
			//使用硬件进行流控制
			new_opt.c_cflag |=CRTSCTS;
			break;
		case '2':
			new_opt.c_cflag |= IXON | IXOFF | IXANY;
			break;
	}
	
	/*===========设置数据位============*/
	new_opt.c_cflag &=~CSIZE;
	switch(p_info->data_bits){
		case '5':
			new_opt.c_cflag |=CS5;
			break;
		case '6':
			new_opt.c_cflag |=CS6;
			break;
		case '7':
			new_opt.c_cflag |=CS7;
			break;
		case '8':
			new_opt.c_cflag |=CS8;
			break;
		default:
			new_opt.c_cflag |=CS8;
		}

	/*===========设置奇偶效验位==========*/
	switch(p_info->parity){
		case '0':
			//不使用奇偶效验
			new_opt.c_cflag &=~PARENB;
			break;
		case '1':
			//使用偶效验
			new_opt.c_cflag |=PARENB;
			new_opt.c_cflag &=~PARODD;
			break;
		case '2':
			//使用奇效验
			new_opt.c_cflag |=PARENB;
			new_opt.c_cflag |=PARODD;
			break;
	}


	/*============设置停止位===========*/
	if(p_info->stop_bit=='2')
		new_opt.c_cflag |=CSTOPB;
	else
		new_opt.c_cflag &=~CSTOPB;

	//修改输出模式，原始数据输出
	new_opt.c_oflag &=~OPOST;
	//修改控制字符，读取字符的最少个数为1
	new_opt.c_cc[VMIN]=1;
	//修改控制字符，读取第一个字符等待1*（1/10）s
	new_opt.c_cc[VTIME]=1;

	//如果发生数据溢出，接收数据，但是不再读取
	tcflush(p_info->port_fd,TCIFLUSH);
	
	int result;
	result=tcsetattr(p_info->port_fd,TCSANOW,&new_opt);

	if(result==-1){
		perror("Cannot set the serial port parameters");
		return -1;
	}

	tcgetattr(p_info->port_fd,&old_opt);
	return result;
}

int send_data(int fd,char *data,int data_len)
{
	int len;
	len=0;

	len=write(fd,data,data_len);

	if(len==data_len)
		return len;
	else{
		//如果出现溢出的情况
		tcflush(fd,TCOFLUSH);
		return -1;
	}
}

int recv_data(int fd,char *data,int data_len)
{
	int len,fs_sel;
	fd_set fs_read;

	struct timeval time;

	FD_ZERO(&fs_read);
	FD_SET(fd,&fs_read);

	time.tv_sec=10;
	time.tv_usec=0;

	//使用select实现串口的多路通信
	fs_sel=select(fd+1,&fs_read,NULL,NULL,&time);

	if(fs_sel){
		len=read(fd,data,data_len);
		return len;
	}else
		return -1;
}

int main(int argc,char* argv[])
{
	int port_fd;
	int len;
	char recv_buf[9];
	int i;

	if(argc!=3){
		printf("Usage: %s /dev/ttySn 0(send data)/1(receive data)\n",argv[0]);
		return -1;
	}

	port_fd=open_port(argv[1]);

	if(port_fd==-1){
		printf("Program Exit\n");
		return -1;
	}

	//设置串口通信参数
	struct port_info info;
	info.baud_rate=9600;
	info.data_bits=8;
 	info.flow_ctrl=2;
	info.port_fd=port_fd;
	info.stop_bit=1;
	info.parity=0;

	if(set_port(&info)==-1){
		printf("Program Exit\n");
		return -1;
	}
	
	if(strcmp(argv[2],"0")==0){
		for(i=0;i<10;i++){
			len=send_data(port_fd,"Test Data",9);

			if(len>0)
				printf("%d send data successfully\n",i);
			else 
				printf("send data failed\n");
			
			sleep(2);
		}
		close_port(port_fd);

	}else{
		while(1){
			len=recv_data(port_fd,recv_buf,9);

			if(len>0){
				for(i=0;i<len;i++)
					printf("receive data is %s\n",recv_buf);
			}else
				printf("cannot receive data\n");
			sleep(2);
		}
      }
	return 0;
}
