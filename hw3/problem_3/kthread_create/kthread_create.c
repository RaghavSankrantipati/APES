#include<linux/kernel.h>
#include<linux/sched.h>
#include<linux/module.h>
#include<linux/jiffies.h>
#include<linux/kthread.h>
#include<linux/time.h>

/**********************************************************************
*@Filename:kthread_create.c
*
*@Description:This program is to calculate the time taken for a new thread 
creation using kthread_create()
*@Author:Sai Raghavendra Sankrantipati
*@Date:10/5/2017
*@compiler:GCC
**********************************************************************/

MODULE_AUTHOR("Sai Raghavendra Sankrantipati");
MODULE_DESCRIPTION("To profile kthread_create()");

static struct task_struct *kthread;

/* This function will be executed by new thread created*/
static int some_fn(void *unused)
{
	printk("Thread is executing function\n");
	return 0;
}

static int init_kthread(void)
{
	printk("Initilising Module\n");
	/*timespec to members tv_sec and tv_nsec*/
	struct timespec start, end, diff;
	/*returns time of a day in timespec*/
	getnstimeofday(&start);
	/* Kthread returns 0 on successful thread creation*/
	kthread = kthread_create(some_fn, NULL, "my_kthread");
	if (kthread)
	{
		getnstimeofday(&end);
		diff = timespec_sub(end, start);
		printk("Thread created succesfully \n");
		/*diff.tv_sec should be zero*/	
		printk("Total time taken :%ld ns\n", diff.tv_nsec);
		/*Instead of two functions kthread_create and wake_up_process
                  kthread_run can be used*/
		wake_up_process(kthread);
	}
	else 
		printk("Unable to create thread");
}

static void cleanup_kthread(void)
{
	printk("Exiting module\n");
}

module_init(init_kthread);
module_exit(cleanup_kthread);
