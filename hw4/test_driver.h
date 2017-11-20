#ifndef TEST_DRIVER_H_
#define TEST_DRIVER_H_

#define LED_OFF 0
#define LED_ON 1

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
