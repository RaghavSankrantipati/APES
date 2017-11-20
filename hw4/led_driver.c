#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <asm/uaccess.h>

/**********************************************************************
*@Filename:led_driver.c
*
*@Description: A  char driver for onboard led3
*@Author:Sai Raghavendra Sankrantipati
*@Date:11/19/2017
*@compiler:arm-linux-gnueabihf-gcc
*@Usage : run make
	$sudo insmod led_driver.ko
	 check kern.log for driver logs
	$dmesg
	$sudo rmmod led_driver.ko
*@Reference: http://derekmolloy.ie/writing-a-linux-kernel-module-part-2-a-character-device/
*@Reference: Used comments directly from source as they are produced using Doxygen for better understanding
 **********************************************************************/

#define DEVICE_NAME "led_dev"
#define CLASS_NAME "led_class"
#define LED_OFF 0
#define LED_ON 1
#define LED3 56

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sai Raghavendra Sankrantipati");
MODULE_DESCRIPTION("A simple char driver for On board LEDs");

static int major_num;
static int period = 100;
static int duty_cycle = 50;
static int state = 0;
static int read;
static struct class * ledcharClass = NULL;
static struct device * ledcharDevice = NULL;
static struct timer_list led_timer;

/*prototype functions for driver*/
static int led_open(struct inode *inodep, struct file *filep);
static int led_read(struct file *filep, char * buffer, size_t len, loff_t * offset);
static ssize_t led_write(struct file *filep, const char __user * buffer, size_t len, loff_t * offset);
static int led_release(struct inode *inodep, struct file *filep);

/*enum for user space messages*/
typedef enum {
	write_period,
	write_duty_cycle,
	write_state,
	read_period,
	read_duty_cycle,
	read_state
}state_variable_t;

/*message structure*/
struct user_struct{
	state_variable_t state_variable;
	int value;
};

/*file_operations lists callback functions*/
static struct file_operations fops = {
   .open = led_open,
   .read = led_read,
   .write = led_write,
   .release = led_release,
};

/*It is called each time device is opened*/
static int led_open(struct inode *inodep, struct file *filep){
	printk(KERN_INFO "LED DRIVER: Device opened\n");
	return 0;
}

/** @brief This function is called whenever device is being read from user space i.e. data is
 *  being sent from the device to the user. In this case is uses the copy_to_user() function to
 *  send the buffer string to the user and captures any errors.
 *  @param filep A pointer to a file object (defined in linux/fs.h)
 *  @param buffer The pointer to the buffer to which this function writes the data
 *  @param len The length of the b
 *  @param offset The offset if required
 */
static ssize_t led_read(struct file *filep,char * buffer, size_t len, loff_t * offset){
	struct user_struct *ud;
	printk(KERN_INFO "LED DRIVER: Reading from device\n");
	ud = (struct user_struct *)kmalloc(1, GFP_KERNEL);

	//// copy_from_user has the format ( * to, *from, size) and returns 0 on success
	if(copy_from_user(ud, buffer, len))
		return -EFAULT;

	//Check for the type of value to be sent
	read = ud->state_variable;
	//If asks for period or duty cycle or state
	if(read == read_period){
		ud->value = (period/10);
	} else if (read == read_duty_cycle){
		ud->value = duty_cycle;
	} else if (read == read_state){
		ud->value = state;
	}

	// copy_to_user has the format ( * to, *from, size) and returns 0 on success
	if(copy_to_user(buffer, ud, len))
		return -EFAULT;
	kfree(ud);
	return 0;
}

/** @brief This function is called whenever the device is being written to from user space i.e.
 *  data is sent to the device from the user. The data is copied to the message[] array in this
 *  LKM using the sprintf() function along with the length of the string.
 *  @param filep A pointer to a file object
 *  @param buffer The buffer to that contains the string to write to the device
 *  @param len The length of the array of data that is being passed in the const char buffer
 *  @param offset The offset if required
 */
static ssize_t led_write(struct file *filep, const char __user * buffer, size_t len, loff_t * offset){
	struct user_struct *ud;
	printk(KERN_INFO "LED DRIVER: writing to device\n");
	ud = (struct user_struct *)kmalloc(1, GFP_KERNEL);
	//// copy_from_user has the format ( * to, *from, size) and returns 0 on success
	if(copy_from_user(ud, buffer, len))
		return -EFAULT;

	//Check for which value to be written. Is it for period or duty cycle or status
	if(ud->state_variable == write_period){
		period = ud->value * 10;
		printk(KERN_INFO "LED DRIVER: Updating period: %d secs", ud->value);
	} else if (ud->state_variable == write_duty_cycle){
		duty_cycle = ud->value;
		printk(KERN_INFO "LED DRIVER: Updating duty cycle: %d percent", duty_cycle);
	} else if(ud->state_variable == write_state){
		state = ud->value;
		printk(KERN_INFO "LED DRIVER: Updating LED state");
		gpio_set_value(LED3, state);
	}

	kfree(ud);
	return 0;
}

/** @brief The device release function that is called whenever the device is closed/released by
 *  the userspace program
 *  @param inodep A pointer to an inode object (defined in linux/fs.h)
 *  @param filep A pointer to a file object (defined in linux/fs.h)
 */
static int led_release(struct inode *inodep, struct file *filep){
    printk(KERN_INFO "LED DRIVER: Device successfully closed\n");
    return 0;
}

/** @brief This is a call back of timer
 *  @param data is not used
 */
void led_timer_callback( unsigned long data ){
	printk(KERN_INFO "LED_DRIVER: In timer Callback \n");
	/*If led is on*/
	if(state == LED_OFF){
		/*duty_cycle * period gives milliseconds of on period of LED*/
		mod_timer( &led_timer, jiffies + msecs_to_jiffies(duty_cycle * period));
		/*Turn on led*/
		gpio_set_value(LED3, LED_ON);
		printk(KERN_INFO "LED DRIVER: LED ON");
		//Change state to LED_ON
		state = LED_ON;
	} else if(state == LED_ON){
		/*100 - duty_cycle * period gives milliseconds of off period of LED*/
		mod_timer( &led_timer, jiffies + msecs_to_jiffies((100-duty_cycle) * period));
		/*Turn off led*/
		gpio_set_value(LED3, LED_OFF);
		printk(KERN_INFO "LED DRIVER: LED OFF");
		//Change state to LED_OFF	
		state = LED_OFF;
	}
}

/** @brief The LKM initialization function
 *  The static keyword restricts the visibility of the function to within this C file. The __init
 *  macro means that for a built-in driver (not a LKM) the function is only used at initialization
 *  time and that it can be discarded and its memory freed up after that point.
 *  @return returns 0 if successful
 */
static int __init led_init(void) {
	printk(KERN_INFO "LED DRIVER: Initialising LED driver.. \n");

	// Try to dynamically allocate a major number for the device -- more difficult but worth it
	major_num = register_chrdev(0, DEVICE_NAME, &fops);
	if(major_num < 0){
		printk(KERN_ALERT "LED DRIVER: Failed to register major number\n");
		return major_num;
	}
	printk(KERN_INFO "LED DRIVER: Successfully registered major number\n");

	 // Register the device class
	ledcharClass = class_create(THIS_MODULE, CLASS_NAME);
	if(IS_ERR(ledcharClass)){
		unregister_chrdev(major_num, DEVICE_NAME);
		printk(KERN_ALERT "LED DRIVER: Failed to create class\n");
		return PTR_ERR(ledcharClass);
	}
	printk(KERN_INFO "LED DRIVER: Successfully registered class\n");

	// Register the device driver
	ledcharDevice = device_create(ledcharClass, NULL, MKDEV(major_num, 0), NULL, DEVICE_NAME);
	if (IS_ERR(ledcharDevice)){
		class_destroy(ledcharClass);
		unregister_chrdev(major_num, DEVICE_NAME);
		printk(KERN_ALERT "LED DRIVER: Failed to create the device\n");
		return PTR_ERR(ledcharDevice);
	}
	printk(KERN_INFO "LED DRIVER: Successfully created device\n");

	// Is the GPIO a valid GPIO number (e.g., the BBB has 4x32 but not all available)
	gpio_is_valid(LED3);
	// Set the gpio to be in output mode and false
	gpio_direction_output(LED3, false);
	//set value  on GPIO 56. 
	gpio_set_value(LED3, LED_ON);
	//Timer setup
	setup_timer( &led_timer, led_timer_callback, 0 );
	mod_timer( &led_timer, jiffies + msecs_to_jiffies(500));
	return 0;
}

static void __exit led_exit(void){
	gpio_unexport(LED3);
	del_timer(&led_timer);
	device_destroy(ledcharClass, MKDEV(major_num, 0));
	class_unregister(ledcharClass);
	class_destroy(ledcharClass);
	unregister_chrdev(major_num, DEVICE_NAME);
	printk(KERN_INFO "LED DRIVER: Goodbye..\n");
}
/** @brief A module must use the module_init() module_exit() macros from linux/init.h, which
 *  identify the initialization function at insertion time and the cleanup function (as
 *  listed above)
 */
module_init(led_init);
module_exit(led_exit);
