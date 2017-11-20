#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

#include "test_driver.h"

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

int read_all(void){
	read_LEDdriver(read_period);
	read_LEDdriver(read_duty_cycle);
	read_LEDdriver(read_state);
}

