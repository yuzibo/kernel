/*
 *     File Name: task_test.c
 *     Author: Bo Yu
 *     Mail: tsu.yubo@gmail.com
 *     Created Time: 2017年02月27日 星期一 06时26分11秒
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define BUF_LEN 256

static char re_string[256];
static char string_to_send[256];

int main()
{
	int ret, fd;

	printf("starting test task 06...\n");
	fd = open("/dev/eudyptula", O_RDWR);
	if (fd < 0) {
		perror("Failed to open the device\n");
		return errno;
	}

	printf("Reading from the device...\n");
	ret = read(fd, re_string, BUF_LEN);
	if (ret < 0) {
		perror("Failed to read the device... \n");
		return errno;
	}
	printf("The received message is: [%s]\n", re_string);

	close(fd);


}
