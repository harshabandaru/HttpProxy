/***************************************************************************
 *  Soufiane Naciri							   *
 *  snaciri@utk.edu					   		   *
 *  ECE 453								   *
 *  11/30/2009								   *
 ***************************************************************************
 *  client.c 								   *
 *    This program is a simple http client written in C. It accepts	   *
 *    input argument from the command line in the following format:	   *
 *      > ./client [hostname or IP] [port number]			   *
 *    The program then sends a HTTP GET request for the host specified	   *
 *    and outputs the stream results of the server on the standard output. *
 *									   *
 ***************************************************************************/

#include <stdio.h>
#include <ctype.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

typedef enum {FALSE, TRUE} bool;

/* inout() - This function prints out the results of the file descriptor 
 * after finishing sending the request to the server
 * this function was borrowed from my CS 360 class lecture material
 * http://www.cs.utk.edu/~huangj/cs360/360/notes/Sockets/serve1.c
 */
int inout(int in, int out)
{
  char *s;
  int i = 0;
  
  s = (char *) malloc (1000*sizeof(char));
  while(read(in, s+i, 1) != 0) {
    if (s[i] == '\n') {
      write(out, s, i+1);
      return i+1;
    }
    i++;
  }
return 0;
}

/* isvalidip() - Uses the inet_pton() function to check whether or not 
 * the specified IP address is valid. Returns TRUE if valid and FALSE
 * otherwise.
 */
bool isvalidip(char *ip)
{
    struct sockaddr_in sa;
    int valid = inet_pton(AF_INET, ip, &(sa.sin_addr));
    return valid != 0;
}

/* send_request() - Contains the principal code the for the program.
 * The function creates a HTTP GET string to send to the server
 * and returns everything on a file descriptor to pass it to main().
 */
int send_request(char *dns, char *port){

	int sockfd, bindfd;					 // socket and bind file descriptors
        char *ptr, *host, path[100];
	char getrequest[1024];
        struct addrinfo hints, *res;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
		printf("1%s\n",dns );
	// conditions to differentiate between an IP address and a hostname
        if ( isvalidip(dns) ) 
        { // when an IP is found then no paths are specified
        	printf("2%s\n",dns );
		sprintf(getrequest, "GET / HTTP/1.0\nHOST: %s\n\n", dns); // create a get request only for the IP
        } else { // if it's not an IP, then it should be a DNS name
		if ( (ptr = strstr(dns, "/")) == NULL) { // if the DNS name does not contain a slash at the end
			printf("3%s\n",dns );
			 sprintf(getrequest, "GET / HTTP/1.0\nHOST: %s\n\n", dns); // we send a request only for the dns name
			}
		else {  // it there is a slash right after the DNS name then there is path to a filename
			strcpy(path, ptr);
        		host = strtok(dns, "/"); //truncate the argument to a PATH and DNS name
			sprintf(getrequest, "GET %s HTTP/1.0\nHOST: %s\n\n", path, dns);
			printf("4%s %s\n",dns,path );
		}
	}
	// gets linked list of results of a specified hostname
	if ( getaddrinfo(dns, port, &hints, &res) != 0 ){
		fprintf(stderr, "Host or IP not valid\n"); //quits program if the hostname was not found
		exit(0);
		}


	// creates a socket from hostname results and passes everything to a file descriptor
	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

	// bind the socket to the port passed to getaddrinfo()
        bindfd = bind(sockfd, res->ai_addr, res->ai_addrlen);

	// establish a connection and quits if there is a connection error
	if ( connect(sockfd, res->ai_addr, res->ai_addrlen) != 0 ){
		fprintf(stderr, "Connection error\n");
		exit(0);
		}

        int optval = 1; //is 
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);

	// writes the HTTP request to the socked file descriptor
	write(sockfd, getrequest, strlen(getrequest));

return sockfd; //returns the results to main
}

/* main() - a program is better when it's modular. The main function 
 * here only error checks the input and calls a function to send the
 * request and another other to print out the results
 */
int main(int argc, char **argv){

	int port, sockfd;
	char *host, *ptr;

        if ( argc != 3) {
                fprintf(stderr, "usage: hostname port\n");
                exit(0);
        }

    port = atoi(argv[2]);
	host = argv[1];

	// checks if the 'http://' (or 'https://') protocol is specified
	// if one of them is specified it is ignored them and makes the URL points to 'www.'
	// this was done because getaddrinfo() returns an error when 'http://' is passed to it
	if ( (ptr = strstr(host, "http://")) != NULL || (ptr = strstr(host, "https://")) != NULL ) {
		host = host + 7; //ignoring 'http://'
	}

	// the maximum number of ports should not exceed 655536 
        if ( port > 65536 || port < 0 ){ 
                fprintf(stderr, "Invalid port number\n");
                exit(0);
        }

	// making the socket and sending the GET request
        sockfd = send_request(host, argv[2]);		//argv[2] is port

	// print out the results
        while(inout(sockfd, 1) > 0);

	close(sockfd);

	return 0;
}
