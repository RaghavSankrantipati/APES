#ifndef TEST_DRIVER_H_
#define TEST_DRIVER_H_

#define LED_OFF 0
#define LED_ON 1

/**********************************************************************
*@Filename: test_driver.h
*
*@Description: This is a user space function calls for Led char driver
*@Author:Sai Raghavendra Sankrantipati
*@Date:11/19/2017
*@compiler:arm-linux-gnueabihf-gcc
*@Usage : compile it with any file by including test_driver.h
*@Usage : example : $gcc server.c test_driver.c -o server
	
 **********************************************************************/

/*Enum for socket and driver communication*/
typedef enum {
	write_period,
	write_duty_cycle,
	write_state,
	read_period,
	read_duty_cycle,
	read_state,
	read_all_vars,
	exit_conn
}state_variable_t;

struct user_struct{
	state_variable_t state_variable;
	int value;
};

int fd, ret;
int write_LEDdriver(state_variable_t var, int val);
int read_LEDdriver(state_variable_t var);
int read_all(void);

#endif
