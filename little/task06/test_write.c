/*
 *     File Name: test_write.c
 *     Author: Bo Yu
 *     Mail: tsu.yubo@gmail.com
 *     Created Time: 2017年03月03日 星期五 00时13分11秒
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

#define BUF_LEN 256

static char string_to_send[256] = {0};

int main()
{
	int ret, fd;

	printf("starting test task 06...\n");
	fd = open("/dev/eudyptula", O_RDWR);
	if (fd < 0) {
		perror("Failed to open the device\n");
		return errno;
	}


	printf("Type in your id into kernel\n");

	scanf("%[^\n]%*c", string_to_send);
	/*	^表示”非”，即读入其后面的字符就结束读入。
	 *	这样想读入一行字符串带空格的字符直接用:
	 *	scanf("%[^\n]%*c",str);
         */

	printf("Writing the message to the device is [%s], the len of write is %d\n",string_to_send, strlen(string_to_send));

	ret = write(fd, string_to_send, strlen(string_to_send));

	if(ret != strlen(string_to_send)){

		printf("write into kernel %d bytes \n", ret);
		return errno;
	}

	else
		printf("success for matching of write! the ret is %d\n", ret);
	close(fd);


}

