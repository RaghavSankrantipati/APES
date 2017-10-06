#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<signal.h>


/**********************************************************************
*@Filename:file_io.c
*
*@Description:In this program two new threads are created using pthred_create.
parent thread will open a file and write data into it.
Child 1 will process the file and count number of words, lines and characters
child 2 will display all the processed information
kill command is used to activate child1 and child2
*@Author:Sai Raghavendra Sankrantipati
*@Date:10/5/2017
*@compiler:GCC
*@Usage: 
1. Compile using gcc -pthread file_io.c -o file_io
2. Give the file name as command line argument
	./file_io hello.txt  
	hello.txt is available in same folder
3. It asks to type info for the file. After typing end it with a tab and press enter.
   It takes spaces, newlines etc., but not tabs. Anything after a tab will be discarded.
4. Use kill -10 'pid' to activate child 1 and kill -12 'pid' to activate child 2.
     pid can be found using ps (look for file executable created)
 **********************************************************************/

FILE *file;

/*This mutex provides exclusion of stats from child1 and child2*/
pthread_mutex_t mutex;

/*This struct stores count of words, charcters and lines*/
struct stats
{
	int word_count;
	int char_count;
	int line_count;
};

enum data{
	sigusr1,
	sigusr2,
};


struct stats stat;


char file_name[50];

/*these variables are changed in signam handler once signal is caught*/
static volatile sig_atomic_t sig_usr1 = 0;
static volatile sig_atomic_t sig_usr2 = 0;

static void sig_handler(int signal)
{
	switch(signal)
	{
		case SIGUSR1:
			printf("\nCaught SIGUSR1 \n"); 
			sig_usr1 = 1;
			break;
		case SIGUSR2:
			printf("\nCaught SIGUSR2");
			sig_usr2 = 1;
			break;
		default:
			printf("\n Unknown Signal");
			break;
	}
}
		

void *analyze()
{
	
	pthread_mutex_lock(&mutex);
	printf("Waiting for SIGUSR1.. \n");
	stat.word_count = 1;
	stat.char_count = 0;
	stat.line_count = 1;
	
	/*Waiting for SIGUSR1*/
	while(sig_usr1 == 0);
	printf("Analyzing.. \n");
	char x;
	file = fopen(file_name, "r");

	while((x = fgetc(file)) != EOF)
	{
		/*A word is identified by a space*/
		if(x == ' ' || x == '\n')
			stat.word_count++;
		/*If its not a space nor a new line its a character*/
		if(x != ' ' && x != '\n')
			stat.char_count++;
		/*newline*/
		if(x =='\n')
			stat.line_count++;
	}

	fclose(file);
	
	pthread_mutex_unlock(&mutex);


	printf("\nWaiting for SIGUSR2.. \n");
	pthread_exit(NULL);
}

void *printStats()
{	

	/*waiting for SIGUSR2*/
	while(sig_usr2 == 0);
	
	pthread_mutex_lock(&mutex);
	/*Printing word, character and line count*/
	printf("\nPrinting file stats:\nWords = %d\nCharacters = %d \nLines = %d \n", stat.word_count, stat.char_count, stat.line_count);
	pthread_mutex_unlock(&mutex);

	pthread_exit(NULL);
}

void main(int argc, char *argv[])
{
	char para[1000];

	/*copy filename recieved form argv[1]*/
	strcpy(file_name, argv[1]);
	file = fopen(argv[1], "w");
	printf("Write to a file : \n");
	/*This scan writes to para[1000]. It takes spaces and newlines but not tab*/
   	scanf("%[^\t]s", para);
        
	/*Writing to the file*/	
	fprintf(file, "%s", para);
	fclose(file);

	pthread_mutex_init(&mutex, NULL);
	/*Initialising signal handler*/
	struct sigaction sig;
	sig.sa_flags = SA_SIGINFO;
	sigemptyset(&sig.sa_mask);
	sig.sa_handler = sig_handler;
	/*adding sigusr1 and sigusr2 to the signal handler*/
	if(sigaction(SIGUSR1, &sig, NULL) == -1)
	{
		perror("sigaction");
		printf("unable to sigaction 1");
	}
	
	if(sigaction(SIGUSR2, &sig, NULL) == -1)
	{
		perror("sigaction");
		printf("unable to sigaction 2");
	}
	
	pthread_t thread1, thread2;
	/*initialising child threads for analyze and printstats functions*/	
	pthread_create(&thread1, NULL, analyze, NULL);
	pthread_create(&thread2, NULL, printStats, NULL);
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	pthread_mutex_destroy(&mutex);
	
}
