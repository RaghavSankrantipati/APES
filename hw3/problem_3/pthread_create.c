#include<pthread.h>
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>

/**********************************************************************
*@Filename:pthread_create.c
*
*@Description:This program is to calculate the time taken for a new thread 
creation using pthread_create()
*@Author:Sai Raghavendra Sankrantipati
*@Date:10/5/2017
*@compiler:GCC
**********************************************************************/

/* This function will be executed by new thread created*/
void *dummy()
{
	printf("\nThread exiting function\n");
	/*Thread exits*/
	pthread_exit(NULL);
}

void main()
{
	clock_t start, end;
	start = clock();
	pthread_t thread;

	printf("Creating a new thread\n");
	printf("Parent PID: %d\n", getpid());
	
	/*Creating a thread*/
	if(pthread_create(&thread, NULL, dummy, NULL) != 0)
		printf("Unable to create thread\n");

        end = clock();
	/*10^6 clocks per sec*/
        double total = (double)(end - start)/CLOCKS_PER_SEC;
        printf("Time taken for pthread_create : %f secs\n", total);
	pthread_join(thread, NULL);
}
