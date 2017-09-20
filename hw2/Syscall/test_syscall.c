#include<stdio.h>
#include<linux/kernel.h>
#include<sys/syscall.h>
#include<unistd.h>
#include<stdlib.h>
/**************************************************************************************
*@Filename:sort_buff.c
*
*@Description: A system call implementation that takes src address and destination address which are specific size buffers. This sorts the array from largest to smallest
*@Author:Sai Raghavendra Sankrantipati
*@Date:19/07/2017
*@compiler:gcc
**************************************************************************************/

void main()
{
	int32_t size = 300;
	int32_t start[size], final[size];
	
	for(int m=0; m<size; m++)
	{
		start[m] = rand();
		printf(" %d__ ", start[m]);
	}

	/* System Call implementation*/

	if(syscall(333, start, &size, final)==0)
	printf("\nSystem call executed\n");

	for (int m=0; m<size; m++)
	{
		printf("%d__ ", final[m]);
	}
}
