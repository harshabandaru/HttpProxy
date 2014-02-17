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
#include <ctype.h>
#include <netdb.h>

int inout(int in, int out)
{
  char *s;
  int i = 0;
  
  s = (char *) malloc (1000*sizeof(char));
  while(read(in, s+i, 1) != 0) {
  	//printf("in whil" );
    if (s[i] == '\n') {
      write(out, s, i+1);
      return i+1;
    }
    i++;
  }
return 0;
}

int checkvalidity(char * s)
{

}
/* isvalidip() - Uses the inet_pton() function to check whether or not 
 * the specified IP address is valid. Returns TRUE if valid and FALSE
 * otherwise.
 */
int isvalidip(char *ip)
{
    struct sockaddr_in sa;
    int valid = inet_pton(AF_INET, ip, &(sa.sin_addr));
    return valid != 0;
}

int main(int argc, char *argv[])
{
	int proxyPort;
	int optval = 1; //is 
	int sdProxyAsServer=0,newsdProxyAsServer=0;
	int sdProxyAsClient = 0,newsdProxyAsClient = 0;
	struct sockaddr_in proxyAsServ_addr;
	struct sockaddr_in proxyAsClient_addr;
	struct addrinfo hints, *res;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	char recvBuf_proxyAsServer[1025];
	char getRequest[1025];
	char *ptr; 						//temporary variable
	int i=0,j=0;
	char ipaddr[20],path[100],*host;
	if(argc != 2)
	{
		fprintf(stderr, "Usage:./proxy [port]\n" );
	}
	proxyPort = atoi(argv[1]);

	memset(&proxyAsServ_addr, '0', sizeof(proxyAsServ_addr));
	memset(&proxyAsClient_addr, '0', sizeof(proxyAsClient_addr));
	memset(&recvBuf_proxyAsServer,'0',sizeof(recvBuf_proxyAsServer));


	sdProxyAsServer = socket (AF_INET, SOCK_STREAM, 0);
	proxyAsServ_addr.sin_family = AF_INET;
    proxyAsServ_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    proxyAsServ_addr.sin_port = htons(proxyPort); 
	bind(sdProxyAsServer, (struct sockaddr *)&proxyAsServ_addr, sizeof(proxyAsServ_addr));
	listen(sdProxyAsServer,10);
	while(1)
	{
		//Server Code Starts
		newsdProxyAsServer = accept(sdProxyAsServer, (struct sockaddr*)NULL, NULL);

		recv(newsdProxyAsServer, recvBuf_proxyAsServer, sizeof(recvBuf_proxyAsServer), 0);
		
		i=0;
		j=0;		

     	
     	for (i = 4; recvBuf_proxyAsServer[i]!=' '; ++i)			//Parsing ipaddress
     	{
     		/* code */
     		(ipaddr)[j]=recvBuf_proxyAsServer[i];
     		j++;
     	}
     	(ipaddr)[j]='\0';
     	printf("%s\n", ipaddr);


     	if ( isvalidip(ipaddr) ) 
     	{ 													// when an IP is found then no paths are specified
     	    //printf("2%s\n",ipaddr );
     		sprintf(getRequest, "GET / HTTP/1.0\nHOST: %s\n\n", ipaddr); // create a get request only for the IP
     	} 
     	else 
     	{ 														// if it's not an IP, then it should be a DNS name
     		if ( (ptr = strstr(ipaddr, "/")) == NULL) 
     		{ 													// if the DNS name does not contain a slash at the end
     			//printf("3%s\n",ipaddr );
     			sprintf(getRequest, "GET / HTTP/1.0\nHOST: %s\n\n", ipaddr); // we send a request only for the dns name
     		}
     		else
     		{  													// it there is a slash right after the DNS name then there is path to a filename
     			strcpy(path, ptr);
     	        host = strtok(ipaddr, "/"); //truncate the argument to a PATH and DNS name
     			sprintf(getRequest, "GET %s HTTP/1.0\nHOST: %s\n\n", path, ipaddr);
     			//printf("4%s %s\n",ipaddr,path );
     		}
     	}     	



     	// checks if the 'http://' (or 'https://') protocol is specified
     	// if one of them is specified it is ignored them and makes the URL points to 'www.'
     	// this was done because getaddrinfo() returns an error when 'http://' is passed to it
     	/*if ( (ptr = strstr(ipaddr, "http://")) != NULL || (ptr = strstr(ipaddr, "https://")) != NULL ) {
     		*ipaddr = *ipaddr + 7; //ignoring 'http://'
     	}*/
     	//printf("%s\n", ipaddr);
     	//End of Server Code

     	//Client Code Starts

     	/*sdProxyAsClient = socket (AF_INET, SOCK_STREAM, 0);
     	proxyAsClient_addr.sin_family = AF_INET;
     	proxyAsClient_addr.sin_addr.s_addr = htonl(INADDR_ANY);
     	proxyAsClient_addr.sin_port = htons(80); 
     	bind(sdProxyAsClient, (struct sockaddr *)&proxyAsClient_addr, sizeof(proxyAsClient_addr));
     	connect(sdProxyAsClient, (struct sockaddr *)&proxyAsClient_addr, sizeof(proxyAsClient_addr));
     	sprintf(getRequest,"GET  HTTP/1.0\nHOST:%s \n\n",ipaddr);
     	write(sdProxyAsClient,getRequest,strlen(getRequest));
     	while(inout(sdProxyAsClient,sdProxyAsServer) > 0);
     	while(inout(sdProxyAsClient,1) > 0);
     	*/
     	getaddrinfo(ipaddr, "80", &hints, &res);
     	sdProxyAsClient= socket(res->ai_family, res->ai_socktype, res->ai_protocol);
     	//printf("1" );
     	bind(sdProxyAsClient, res->ai_addr, res->ai_addrlen);
     	//printf("2" );
     	connect(sdProxyAsClient, res->ai_addr, res->ai_addrlen);
     	//printf("3" );
     	//sprintf(getRequest, "GET / HTTP/1.0\nHOST: %s\n\n", ipaddr);

     	




     	//printf("4" );
     	optval = 1; //is 
        setsockopt(sdProxyAsClient, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);
        //printf("5" );
        write(sdProxyAsClient, getRequest, strlen(getRequest));
        //("6" );
        while(inout(sdProxyAsClient,newsdProxyAsServer) > 0);


     	//Client Code Ends
     	
		close(newsdProxyAsServer);

		close(newsdProxyAsClient);
		sleep(1);
	}
	return 0;
}