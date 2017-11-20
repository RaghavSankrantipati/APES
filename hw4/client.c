#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

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

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;

    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);


    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");
    printf("Client: Connection established with %s\n",argv[1]);

    int count = 0;
    struct user_struct us;
    while(1){


    	printf("count = %d\n",count );
    	if( count == write_period){
    		printf("count = %d\n",count );
    	    struct user_struct us = {
    	    		htons(write_period),
    	    		htons(5),
    	    };
    	}
    	else if( count == write_duty_cycle){
    		printf("count = %d\n",count );
    	    struct user_struct us = {
    	    		htons(write_duty_cycle),
    	    		htons(99),
    	    };
    	}
    	else if( count == write_state){
    		printf("count = %d\n",count );
    	    struct user_struct us = {
    	    		htons(write_state),
    	    		htons(LED_OFF),
    	    };
    	}
    	else if( count == read_period){
    		printf("count = %d\n",count );
    	    struct user_struct us = {
    	    		htons(read_period),
    	    		htons(0),
    	    };
    	}
    	else if( count == read_duty_cycle){
    		printf("count = %d\n",count );
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

    	n = write(sockfd, (void *) &us, sizeof(us));
    	if (n < 0)
    		error("ERROR writing to socket");
    	bzero(buffer,256);
    	n = read(sockfd,buffer,255);
    	if (n < 0)
    		error("ERROR reading from socket");
    	printf("Client: %s\n",buffer);
    	sleep(1);
    	count++;

    	if(count == exit_conn)
    		return 0;
    }
}
