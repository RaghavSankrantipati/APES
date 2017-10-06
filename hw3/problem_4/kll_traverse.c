#include<linux/kernel.h>
#include<linux/sched.h>
#include<linux/module.h>
#include<linux/list.h>

/**********************************************************************
*@Filename:kthread_create.c
*
*@Description:program that can traverse among kernel processes.
It prints Thread's name, PID, process status, number of chilren.
creation using kthread_create()
*@Author:Sai Raghavendra Sankrantipati
*@Date:10/5/2017
*@compiler:GCC
**********************************************************************/

MODULE_AUTHOR("Sai Raghavendra Sankrantipati");
MODULE_DESCRIPTION("To traverse among all the kernel processes");
static int init_traverse(void)
{
	printk(KERN_INFO "Initialising Module\n");
	/*Task struct stores details of all the processes*/
	struct task_struct *task;
	struct task_struct *parent;
	struct list_head *list;
	uint16_t children = 0;
	/*It iterates through all the processes using task*/
	for_each_process(task)
	{
		/*list_for_each iterates over list*/
		list_for_each(list, &(task->children))
		{
			children++;
		}
		parent = task->parent;	
		/*task->comm gives the name of process*/	
		printk("%s | %d(pid) | %d(Parent pid) | %d(state) | %d(children)", task->comm, task->pid, parent->pid, task->state, children);
		children = 0;
	}

	return 0;
}

static void cleanup_traverse(void)
{
	printk(KERN_INFO "Exiting Module\n");
} 

module_init(init_traverse);
module_exit(cleanup_traverse);
