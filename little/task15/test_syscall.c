/*
 *     File Name: test_syscall.c
 *     Author: Bo Yu
 *     Mail: tsu.yubo@gmail.com
 *     Created Time: Wed Feb 20 13:39:12 2019
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>

int main()
{
	int res = syscall(387, 1, 2);
	printf("The res is %d\n", res);
	res = syscall(387, 0xe233f6, 0x1cc9c6);
	printf("The res is %d\n", res);
}

