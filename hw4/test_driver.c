#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

#include "test_driver.h"

/**********************************************************************
*@Filename: test_driver.c
*
*@Description: This is a user space function calls for Led char driver
*@Author:Sai Raghavendra Sankrantipati
*@Date:11/19/2017
*@compiler:arm-linux-gnueabihf-gcc
*@Usage : compile it with any file by including test_driver.h
*@Usage : example : $gcc server.c test_driver.c -o server
	
*@Reference: http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html
 **********************************************************************/

/** @brief This writes a user_struct to led driver 
 *  @param state_variable_t gives info of where to write value
 *  @param update value of state_variable
 */
int write_LEDdriver(state_variable_t var, int val){
	struct user_struct *us;
	us = (struct user_struct *)malloc(sizeof(struct user_struct));
	us->state_variable = var;
	us->value = val;
	ret = write(fd, us, sizeof(*us)); // Send the string to the LKM

	if (ret < 0){
	      perror("Failed to write the message to the device.");
	      return errno;
	}
	return 1;
}

/** @brief This reads a user_struct from led driver 
 *  @param reads value of state_variable
 */

int read_LEDdriver(state_variable_t var){
	struct user_struct *us;
	us = (struct user_struct *)malloc(sizeof(struct user_struct));
	us->state_variable = var;
	us->value = 0;
	ret = read(fd, us, sizeof(struct user_struct));        // Read the response from the LKM
	if (ret < 0){
		perror("Failed to read the message from the device.");
	    return errno;
	}

	if( var == read_period ){
		printf("User LED 3: Period = %d \n", us->value);
	} else if (var == read_duty_cycle){
		printf("User LED 3: duty cycle = %d \n", us->value);
	} else if(var == read_state){
		printf("User LED 3: state = %d \n", us->value);
	}
	return us->value;
}

//@brief this reads all the private variables of driver
int read_all(void){
	read_LEDdriver(read_period);
	read_LEDdriver(read_duty_cycle);
	read_LEDdriver(read_state);
}

