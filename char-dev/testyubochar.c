/*************************************************************************
     File Name: testyubochar.c
     Author: yubo
     Mail: yuzibode@126.com
     Created Time: 2016年01月18日 星期一 17时35分11秒
 ************************************************************************/
/*
 * The User-space Program for testing the LKM
 *
 */
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#define BUFFER_LENGTH 256
static char receive[BUFFER_LENGTH];
int main()
{
	int ret, fd;
	char stringToSend[BUFFER_LENGTH];
	printf("Starting device test code example,,,\n");
	fd = open("/dev/yubochar",O_RDWR);
	if (fd < 0){
		perror("Failed to open the device...\n");
		return errno;

	}
	printf("Type in a short string to the kernel module:\n");

	scanf("%[^\n]%*c", stringToSend);/* Send in a string (with space)*/
	printf("Writing the message to the device [%s].\n",stringToSend);
	ret = write(fd, stringToSend,strlen(stringToSend));
	if (ret < 0){
		perror("FAiled to the message to the device\n");
		return errno;

	}
	printf("Pres ENTER to read back from the device...\n");
	getchar();/* newline */
	printf("READing from the device...\n");
	ret = read(fd, receive,BUFFER_LENGTH);
	if (ret < 0){
		perror("Failed to read the message from the device...\n");
		return errno;
	}
	printf("The received message is: [%s]\n");
	printf("End of the program\n");
	return 0;

}

