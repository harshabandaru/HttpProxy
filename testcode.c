#include <stdio.h>
#include <string.h>
void func(char *s)
{
	if(!strcmp(s,"Hello")){
		printf("1\n");
	}
}
int main()
{
	char host[50],*ptr,temp[50],i=0,k1=0,path[50],method[50],protocolversion[50];

	char as[70]="GET /gethostel/ip/ HTTP/1.0\nHOST: www.iitg.ernet.in\r\n";

	sscanf(as, "%s %s %s\nHOST: %s\r\n",method,path,protocolversion,host);
	printf("%s\n", method);
	printf("%s\n", path);
	printf("%s\n", protocolversion);
	printf("%s\n", host);

/*	strcpy(temp,as);
	printf("%s\n", temp);
	if ( (ptr = strstr(as, "http://")) != NULL || (ptr = strstr(as, "https://")) != NULL ) {
            for (i = 7; i < strlen(as); ++i)
            {
                
                as[k1]=temp[i];
                k1++;
            }       //ignoring 'http://'
        }

        as[k1]='\0';
        printf("%s\n", as);
	
	printf("%s\n", as);
*/	//func(as);
}