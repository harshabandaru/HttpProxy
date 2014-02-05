#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 


int main(int argc, char *argv[])
{
	int port,nbytes=0,check_rec;
	port=atoi(argv[1]);
	int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr; 

    char sendBuff[1025];
    char recvBuff[1025]="jklafdalsfas";

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(recvBuff, '0', sizeof(recvBuff));
    memset(sendBuff, '0', sizeof(sendBuff)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port); 

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

    listen(listenfd, 10); 

    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 

       // ticks = time(NULL);
       // snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
       // write(connfd, sendBuff, strlen(sendBuff)); 
       // recv(listenfd, recvBuff, sizeof(recvBuff), 0);
       // snprintf(sendBuff, sizeof(sendBuff), "sfdgsfgsd");
        //sendBuff="fdsfasfsaf";
        // write(connfd, sendBuff, strlen(sendBuff));
        
       
     	check_rec=recv(connfd,recvBuff,sizeof(recvBuff),0);
     	if(check_rec<0)
     	{
     		snprintf(sendBuff, sizeof(sendBuff), "er");
        //sendBuff="fdsfasfsaf";
         write(connfd, sendBuff, strlen(sendBuff));
     	}
     	else
     	{
     		snprintf(sendBuff, sizeof(recvBuff), "%s",recvBuff);
        //sendBuff="fdsfasfsaf";
         write(connfd, sendBuff, strlen(sendBuff));
     	}	
     	puts(recvBuff);
        close(connfd);
        sleep(1);
     }
}