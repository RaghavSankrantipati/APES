#include<pthread.h>
#include<stdio.h>

/**********************************************************************
*@Filename:pthread_example.c
*
*@Description: This program uses some of pthread API function.
create, join, exit, self, getstacksize, setstacksize, mutex_init,
mutex_destroy, mutex_lock, mutex_unlock, cond_init, cond_destroy,
cond_signal, cond_wait

Two threads, child 1 and child 2 are created, one to send signal and other
 waits until it recieves a signal. A global variable status is initialized to 0.
In cond_signal function a thread changes it's value to 20 and signals 
a thread waiting in cond_wait function. In cond_wait function, child 2 
thread again changes status variable.

Took reference from https://computing.llnl.gov/tutorials/pthreads/
*@Author:Sai Raghavendra Sankrantipati
*@Date:10/5/2017
*@compiler:GCC
**********************************************************************/

int status = 0;

pthread_mutex_t mutex;
pthread_cond_t cond;

/*cond_signal is given in this function*/
void *cond_signal()
{
	printf("\nIn cond_signal function \nThread id = %ld\n", pthread_self());
	/*thread acquires a lock on mutex variable*/
	pthread_mutex_lock(&mutex);
	
	status = 20;
	printf("Status before sending signal: %d\n", status); 
	/*Signal another thread waiting for a wakeup call*/
	pthread_cond_signal(&cond);
	/*thread releases lock on mutex variable*/
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}

/*thread waits until a cond_signal is recieved*/

void *cond_wait()
{
	size_t sz;
	pthread_attr_t attr;	
	/*thread acquires a lock on mutex variable*/
	pthread_mutex_lock(&mutex);
	/*blocks calling thread until the specified condition is signalled*/
	pthread_cond_wait(&cond, &mutex);
	printf("\nIn cond_wait function \nThread id = %ld\n", pthread_self());
	status++;
	printf("status after recieving signal: %d\n", status);
	/*thread releases lock on mutex variable*/
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}

void main()
{
	pthread_t thread, thread1;
	void *status;
	size_t sz;

	/*Initialize pthread attributes*/
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	/*returns stacksize in second argument*/
	pthread_attr_getstacksize(&attr, &sz);
	


	printf("Default stack size = %li\n", sz);
	sz = sz + 1;

	/*sets stacksize by taking as argument*/
	pthread_attr_setstacksize (&attr, sz);
	
	/*This causes all threads created with attr in joinable state*/
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	/*initilize mutex object*/
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);

	/*This creates threads with attr as attributes and cond_signal
	cond_wait functions */
	pthread_create(&thread1, &attr, cond_signal, NULL);
	pthread_create(&thread, &attr, cond_wait, NULL);
	
	/*Destroy attribute object*/
	pthread_attr_destroy(&attr);

	/*It blocks the calling thread untill the specified thread terminates*/
	pthread_join(thread1, &status);
	pthread_join(thread, &status);


	/*destroy mutex attributes*/
	pthread_mutex_destroy(&mutex);
	
	/*It terminates thread*/
	pthread_exit(NULL);
}
