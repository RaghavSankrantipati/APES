#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

/**********************************************************************
*@Filename:fork_exec.c
*
*@Description:This program is to calculate the time taken for a new process 
creation using fork_exec()
*@Author:Sai Raghavendra Sankrantipati
*@Date:10/5/2017
*@compiler:GCC
**********************************************************************/

int main()
{
	pid_t child_pid;
	int status = 0;
	clock_t start1, end1, total, start2, end2;

	start1 = clock();
	printf("pid before fork : %d\n", (int)getpid());
	printf("Creating a new  process using fork\n");
	/*calling fork*/
	child_pid = fork();
	end1 = clock();
	/*If for fails*/
	if(child_pid < 0 )
	{
		perror("fork failure");
	}
	/*If for is successful*/
	else if(child_pid == 0)
	{
		printf("pid after fork : %d\n", (int)getpid());
		printf("calling exec for bin/date\n");
		/*exec only returns if there is an error*/
		if(execl("/bin/date", "date", 0, NULL) == -1)
			perror("Error in EXEC:");
	}
	
	else
	{
		wait(&status);		
		printf("After exec\n");
		double total = (double)(end1-start1)/CLOCKS_PER_SEC;
	        printf("Time taken for fork: %fsecs\n", total);

        }
	return 0;
}

