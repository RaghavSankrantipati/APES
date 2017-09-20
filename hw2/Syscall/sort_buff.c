#include<linux/linkage.h>
#include<linux/kernel.h>
#include<linux/slab.h>
#include<linux/uaccess.h>
#include<linux/module.h>
#include <linux/syscalls.h>
#include <linux/errno.h>
#include <uapi/asm-generic/errno-base.h>
/**************************************************************************************
*@Filename:sort_buff.c
*
*@Description: A system call that takes src address and destination address which are specific size buffers. This sorts the array from largest to smallest
*@Author:Sai Raghavendra Sankrantipati
*@Date:19/07/2017
*@compiler:gcc
**************************************************************************************/

/*system call for buffer sorting*/
SYSCALL_DEFINE3(sort_buff, int32_t *, src, int32_t *, size, int32_t *, dest)
{

	int32_t *kern, len, temp, m, n;
	pr_info("\nSort_buff Syscall began");

	printk("\nsize of the buffer is %d", *size);
	
	len = *size;
	kern = (int32_t *)kmalloc(sizeof(int32_t)*len, GFP_KERNEL);
	
	if(kern == NULL)
		return ENOMEM;


	printk("\n printing source buffer");

	if(copy_from_user(kern, src, len*4))
		return -EFAULT;
	
	/* Bubble sort Algorithm*/
	for(m=0; m<len; m++)
	{
		printk("%d. %d\n", m, kern[m]);
	}
	for(m=0; m<len; m++)
	{
		for(n=0; n<len; n++)
		{
			if(kern[n]<kern[n+1])
			{
				temp = kern[n];
				kern[n] = kern[n+1];
				kern[n+1] = temp;
			}
		}
	}


	if(copy_to_user(dest, kern, len*4))
		return -EFAULT;

	printk("exiting syscall");
	printk("printing sorted buffer");
	for(m=0; m<len; m++)
	{
		printk("%d. %d\n", m, kern[m]);
	}
	kfree(kern);	
	return 0;
}
