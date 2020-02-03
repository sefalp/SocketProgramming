

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include<ctype.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}


int main(int argc, char *argv[])
{
    int sockfd, portno;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    int lsize;
    int *buffer;
    int result;


    if (argc < 3)
    {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }


    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
        error("ERROR opening socket");

    server = gethostbyname(argv[1]);

    if (server == NULL)
	{
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
  	}


    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);


    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    // here the socket programming part ends !!


    FILE *f;
    f=fopen("lebron.mp3","r");
    fseek (f , 0 , SEEK_END); 
    lsize = ftell(f);
    rewind(f);	

    buffer = (int *)malloc(lsize);
	memset(buffer,0,lsize);
    
    //printf("lsize is :%d\n", lsize );

    write(sockfd,(void *)&lsize, sizeof((void *)&lsize));

    result = fread(buffer,1,lsize,f);



  int i=0;

    while(i<(lsize)/sizeof(lsize)+(lsize%4))
   {

		write(sockfd,buffer+i,sizeof(int));
	    ++i;

   }

    printf(" %d %d\n ",lsize,i);

   printf("The file was sent successfully");

    close(sockfd);
    free(buffer);
    return 0;
}
