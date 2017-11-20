/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "test_driver.h"

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, clilen;
     struct user_struct us;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *) &serv_addr,
    		sizeof(serv_addr)) < 0)
            error("ERROR on binding");
    printf("Server: Socket binded on port no: %d\n", portno);
    listen(sockfd,5);
    printf("Server: Socket listening on  port no: %d\n", portno);
    clilen = sizeof(cli_addr);

	int ret, state_var, val;
	fd = open("/dev/led_dev", O_RDWR);
	if (fd < 0){
		perror("Failed to open the device...");
	    return errno;
	}

    while(1){
    	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    	printf("Server: Accepted connection\n");
    	 if (newsockfd < 0)
    		 error("ERROR on accept");
    	 bzero(buffer,256);
    	 n = read(newsockfd, (struct user_struct *) &us, sizeof(us));
    	 if (n < 0)
    		 error("ERROR reading from socket");

    	 state_var = ntohs(us.state_variable);
    	 val = ntohs(us.value);

    	 if( state_var == write_period)
    	 {
    		 printf("Server: Updating User LED3 period to %d secs\n", val);
    		 n = write_LEDdriver(write_period, val);
        	 if (n != 1)
        		 error("ERROR writing to driver");
        	 n = write(newsockfd,"Upating LED's period",20);
        	 if (n < 0)
        		 error("ERROR writing to socket");
    	 }
    	 else if ( state_var ==  write_duty_cycle)
    	 {
    		 printf("Server: Updating User LED3 duty cycle to %d percent\n", val);
    		 n = write_LEDdriver(write_duty_cycle, val);
        	 if (n != 1)
        		 error("ERROR writing to driver");
        	 n = write(newsockfd,"Updating LED's duty cycle",30);
        	 if (n < 0)
        		 error("ERROR writing to socket");
    	 }
    	 else if ( state_var ==  write_state)
    	 {
    		 printf("Server: Updating User LED3 state to %d \n", val);
    		 n = write_LEDdriver(write_state, val);
        	 if (n != 1)
        		 error("ERROR writing to driver");
        	 n = write(newsockfd,"Upating LED's state",20);
        	 if (n < 0)
        		 error("ERROR writing to socket");
    	 }
    	 else if ( state_var ==  read_period)
    	 {
    		 printf("Server: Sending User LED3's period\n");
    		 n = read_LEDdriver(read_period);
    		 sprintf(buffer, "Period of USER LED 3: %d secs", n);
        	 n = write(newsockfd, buffer,30);
        	 if (n < 0)
        		 error("ERROR writing to socket");
    	 }
    	 else if ( state_var ==  read_duty_cycle)
    	 {
    		 printf("Server: Sending User LED3's duty cycle\n");
    		 n = read_LEDdriver(read_duty_cycle);
    		 sprintf(buffer, "Duty cycle of USER LED 3: %d percent", n);
        	 n = write(newsockfd, buffer,40);
        	 if (n < 0)
        		 error("ERROR writing to socket");
    	 }
    	 else if ( state_var ==  read_state)
    	 {
    		 printf("Server: Sending User LED3's state\n");
    		 n = read_LEDdriver(read_state);
    		 sprintf(buffer, "State of USER LED 3: %d", n);
        	 n = write(newsockfd, buffer,30);
        	 if (n < 0)
        		 error("ERROR writing to socket");
    	 }
    	 else if ( state_var ==  read_all_vars)
    	 {
    		 printf("Server: Sending User LED3's period, duty cycle, state\n");
    		 read_all();
        	 n = write(newsockfd,"Printing  LED's private variables",60);
        	 if (n < 0)
        		 error("ERROR writing to socket");
    	 }
    	 else if ( state_var ==  exit_conn)
    	 {
    		 printf("Server: Received close server command\n");
    		 printf("Server: Closing connections..\n");
        	 n = write(newsockfd,"Server closed",60);
        	 if (n < 0)
        		 error("ERROR writing to socket");
    		 return 0;
    	 }

     }
}
