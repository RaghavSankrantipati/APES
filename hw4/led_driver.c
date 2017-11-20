#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <asm/uaccess.h>

#define DEVICE_NAME "led_dev"
#define CLASS_NAME "led_class"
#define LED_OFF 0
#define LED_ON 1
#define LED3 56

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sai Raghavendra Sankrantipati");
MODULE_DESCRIPTION("A simple char driver for On board LEDs");

static int major_num;
static int period = 10;
static int duty_cycle = 50;
static int state = 0;
static int read;
static struct class * ledcharClass = NULL;
static struct device * ledcharDevice = NULL;
static struct timer_list led_timer;

static int led_open(struct inode *inodep, struct file *filep);
static int led_read(struct file *filep, char * buffer, size_t len, loff_t * offset);
static ssize_t led_write(struct file *filep, const char __user * buffer, size_t len, loff_t * offset);
static int led_release(struct inode *inodep, struct file *filep);

typedef enum {
	write_period,
	write_duty_cycle,
	write_state,
	read_period,
	read_duty_cycle,
	read_state
}state_variable_t;

struct user_struct{
	state_variable_t state_variable;
	int value;
};

static struct file_operations fops = {
   .open = led_open,
   .read = led_read,
   .write = led_write,
   .release = led_release,
};

static int led_open(struct inode *inodep, struct file *filep){
	printk(KERN_INFO "LED DRIVER: Device opened\n");
	return 0;
}

static ssize_t led_read(struct file *filep,char * buffer, size_t len, loff_t * offset){
	struct user_struct *ud;
	printk(KERN_INFO "LED DRIVER: Reading from device\n");
	ud = (struct user_struct *)kmalloc(1, GFP_KERNEL);
	if(copy_from_user(ud, buffer, len))
		return -EFAULT;

	read = ud->state_variable;
	if(read == read_period){
		ud->value = (period/10);
	} else if (read == read_duty_cycle){
		ud->value = duty_cycle;
	} else if (read == read_state){
		ud->value = state;
	}

	if(copy_to_user(buffer, ud, len))
		return -EFAULT;
	kfree(ud);
	return 0;
}

static ssize_t led_write(struct file *filep, const char __user * buffer, size_t len, loff_t * offset){
	struct user_struct *ud;
	printk(KERN_INFO "LED DRIVER: writing to device\n");
	ud = (struct user_struct *)kmalloc(1, GFP_KERNEL);
	if(copy_from_user(ud, buffer, len))
		return -EFAULT;

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

static int led_release(struct inode *inodep, struct file *filep){
    printk(KERN_INFO "LED DRIVER: Device successfully closed\n");
    return 0;
}

void led_timer_callback( unsigned long data ){
	printk(KERN_INFO "LED_DRIVER: In timer Callback \n");
	if(state == LED_OFF){
		mod_timer( &led_timer, jiffies + msecs_to_jiffies(duty_cycle * period));
		gpio_set_value(LED3, LED_ON);
		printk(KERN_INFO "LED DRIVER: LED ON");
		state = LED_ON;
	} else if(state == LED_ON){
		mod_timer( &led_timer, jiffies + msecs_to_jiffies((100-duty_cycle) * period));
		gpio_set_value(LED3, LED_OFF);
		printk(KERN_INFO "LED DRIVER: LED OFF");
		state = LED_OFF;
	}
}

static int __init led_init(void) {
	printk(KERN_INFO "LED DRIVER: Initialising LED driver.. \n");

	major_num = register_chrdev(0, DEVICE_NAME, &fops);
	if(major_num < 0){
		printk(KERN_ALERT "LED DRIVER: Failed to register major number\n");
		return major_num;
	}
	printk(KERN_INFO "LED DRIVER: Successfully registered major number\n");

	ledcharClass = class_create(THIS_MODULE, CLASS_NAME);
	if(IS_ERR(ledcharClass)){
		unregister_chrdev(major_num, DEVICE_NAME);
		printk(KERN_ALERT "LED DRIVER: Failed to create class\n");
		return PTR_ERR(ledcharClass);
	}
	printk(KERN_INFO "LED DRIVER: Successfully registered class\n");

	ledcharDevice = device_create(ledcharClass, NULL, MKDEV(major_num, 0), NULL, DEVICE_NAME);
	if (IS_ERR(ledcharDevice)){
		class_destroy(ledcharClass);
		unregister_chrdev(major_num, DEVICE_NAME);
		printk(KERN_ALERT "LED DRIVER: Failed to create the device\n");
		return PTR_ERR(ledcharDevice);
	}
	printk(KERN_INFO "LED DRIVER: Successfully created device\n");

	gpio_is_valid(LED3);
	gpio_direction_output(LED3, false);
	gpio_set_value(LED3, LED_ON);
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

module_init(led_init);
module_exit(led_exit);
