#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

/**********************************************************************
*@Filename:client.c
*
*@Description: This is client part of socket connection. 
*@Author:Sai Raghavendra Sankrantipati
*@Date:11/19/2017
*@compiler:arm-linux-gnueabihf-gcc
*@Usage : run gcc client.c -o client
	$ ./client <ip.address> <port_no>
	in my case
	$ ./client 192.168.7.2 33333
	usable only in internet domain. Doesn't work for Unix domain
*@Reference: http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html
 **********************************************************************/

#define LED_OFF 0
#define LED_ON 1

/*Enum for socket communication*/
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

/*This struct is transferred in socket communication*/
struct user_struct{
	state_variable_t state_variable; 	//Enum of type of messsage
	int value;				//Value of the message
};

/*Prints error message in stderr*/
void error(char *msg)
{
    perror(msg);
    exit(0);
}

/*Takes 2 arguments along with file name
 *ip address of server at argv[1] 
 *port number at argv[2]
 */
int main(int argc, char *argv[])
{
    int sockfd, portno, n;

    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];

    /*Check for 3 arguments in command line*/
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
   
    /*Ascii value of input port number is converted to integer*/	
    portno = atoi(argv[2]);
    /*Socket system call creates a new socket, returns file pointer*/	
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    /*Takes name as an argument retuns pointer to hostnet*/	
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    /*bzero sets all values in a buffer to zero*/
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    /*bcopy() is a predated version of POSIX, Use memcpy insteadd*/
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);

    /*It establishes connection with server*/
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");
    printf("Client: Connection established with %s\n",argv[1]);

    int count = 0;
    struct user_struct us;
    while(1){
	/*count increases by 1 for every 1 second. All if and elses are self explanatory.
	 *Once every 1 second, each of enum state_varaiable_t is sent as message*/
    	if( count == write_period){
    	    struct user_struct us = {
    	    		htons(write_period),
    	    		htons(5),
    	    };
    	}
    	else if( count == write_duty_cycle){
    	    struct user_struct us = {
    	    		htons(write_duty_cycle),
    	    		htons(99),
    	    };
    	}
    	else if( count == write_state){
    	    struct user_struct us = {
    	    		htons(write_state),
    	    		htons(LED_OFF),
    	    };
    	}
    	else if( count == read_period){
    	    struct user_struct us = {
    	    		htons(read_period),
    	    		htons(0),
    	    };
    	}
    	else if( count == read_duty_cycle){
    	    struct user_struct us = {
    	    		htons(read_duty_cycle),
    	    		htons(0),
    	    };
    	}
    	else if( count == read_state){
    	    struct user_struct us = {
    	    		htons(read_state),
    	    		htons(0),
    	    };
    	}
    	else if( count == read_all_vars){
    	    struct user_struct us = {
    	    		htons(read_all_vars),
    	    		htons(0),
    	    };
    	}
    	else if( count == exit_conn){
    	    struct user_struct us = {
    	    		htons(exit_conn),
    	    		htons(0),
    	    };
    	}

	/*write struct to socket file descriptor*/
    	n = write(sockfd, (void *) &us, sizeof(us));
    	if (n < 0)
    		error("ERROR writing to socket");
    	bzero(buffer,256);
	/*socket gives acknowledgement messagee*/
    	n = read(sockfd,buffer,255);
    	if (n < 0)
    		error("ERROR reading from socket");
    	printf("Client: %s\n",buffer);
    	sleep(1);
    	count++;
	/*Client closes after 7 loops*/    
    	if(count == exit_conn)
    		return 0;
    }
}
