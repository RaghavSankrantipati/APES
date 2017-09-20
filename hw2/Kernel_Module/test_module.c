#include <linux/module.h>	
#include <linux/kernel.h>	
#include <linux/init.h>	
#include <linux/timer.h>	

/**********************************************************************
*@Filename:test_module.c
*
*@Description:A kernel module that triggers every 500ms and count is displayed in kernel log
*@Author:Sai Raghavendra Sankrantipati
*@Date:09/19/2017
*@compiler:GCC

**********************************************************************/
#define AUTHOR "Sai Raghavendra Sankrantipati"

MODULE_LICENSE("GPL");
MODULE_AUTHOR(AUTHOR);
MODULE_DESCRIPTION("Practicing kernel module programming");

/* Structure of timer list*/
static struct timer_list test_timer;
/*It counts the number of times callback function is called */
static int count=0;

/*timer call back function. It displays count in kernel log*/
void test_timer_callback( unsigned long data )
{
  	printk( "In timer callback" );
	count++;
	printk("%d \n", count);	

        mod_timer( &test_timer, jiffies + msecs_to_jiffies(500));

}

/*Init function*/
static int init_test_module(void)
{
	printk(KERN_INFO "Entered in the module\n");
	
	setup_timer( &test_timer, test_timer_callback, 0 );
	mod_timer( &test_timer, jiffies + msecs_to_jiffies(500));

	return 0;
}

/*Clean up function. Clears memory*/
static void cleanup_test_module(void)
{
	del_timer( &test_timer );
	printk(KERN_INFO "Exiting Module\n");
}

module_init(init_test_module);
module_exit(cleanup_test_module);
