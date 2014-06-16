#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>
#include <linux/soundcard.h>

//录音时间
#define LENGTH 3  

//采样频率
#define RATE 8000 

//量化位数
#define SIZE 16

// 声道数目
#define CHANNELS 2

//保存录取的音频数据 
unsigned char buf[LENGTH*RATE*SIZE*CHANNELS/8];

int main(void)
{
  //声音设备的文件描述符 
  int fd;

  int arg; 
  //用于保存ioctl的返回值
  int status; 

  // 打开声音设备
  fd = open("/dev/dsp", O_RDWR);
  
  if (fd < 0) {
    perror("Cannot open /dev/dsp device");
    return 1;
  }
 
  /*=========以下设置声卡参数========*/
  // 设置采样时的量化位数 
  arg = SIZE;
  status = ioctl(fd, SOUND_PCM_WRITE_BITS, &arg);
  
  if (status == -1){
    perror("Cannot set SOUND_PCM_WRITE_BITS ");
    return 1;
  }

  // 设置采样时的声道数目 
  arg = CHANNELS;

  status = ioctl(fd, SOUND_PCM_WRITE_CHANNELS, &arg);

  if (status == -1){
    perror("Cannot set SOUND_PCM_WRITE_CHANNELS");
    return 1;
   }

  // 设置采样时的采样频率
  arg = RATE;

  status = ioctl(fd, SOUND_PCM_WRITE_RATE, &arg);

  if (status == -1){
    perror("Cannot set SOUND_PCM_WRITE_WRITE");
    return 1;
  }

  //一直录音，直到按下Control-C停止
  while (1) {
    printf("Recording...:\n");
    status = read(fd, buf, sizeof(buf));

    if (status != sizeof(buf)){
      perror("read wrong number of bytes");
    }

    printf("Play...:\n");
    status = write(fd, buf, sizeof(buf));
    if (status != sizeof(buf))
      perror("wrote wrong number of bytes");

    // 在继续录音前等待回放结束 
    status = ioctl(fd, SOUND_PCM_SYNC, 0);

    if (status == -1)
      perror("Cannot set SOUND_PCM_SYNC");
  }

    return 0;
}

