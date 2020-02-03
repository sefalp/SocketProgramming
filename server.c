
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>


void error(const char *msg)
{
    perror(msg);
    exit(1);
}


int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     int *buffer;
     struct sockaddr_in serv_addr, cli_addr;


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

     listen(sockfd,5);

     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd,
                 (struct sockaddr *) &cli_addr,
                 &clilen);

     if (newsockfd < 0)
          error("ERROR on accept");

    
        

// dosyaya yazma kısmı

            FILE *fp;
            fp = fopen("bel.txt","a");

             
            int lsize;
           read(newsockfd,(void *)&lsize, sizeof((void *)&lsize));
            
            

            buffer = (int *)malloc(lsize);
            memset(buffer,0,lsize);
               
                   int i=0;
              while(i<(lsize)/sizeof(lsize)+(lsize%4))
             {

          		read(newsockfd,buffer+i,sizeof(int));
          		fwrite(buffer+i,sizeof(int),1,fp);
          	    ++i;

             }
               printf(" %d %d\n ",lsize,i);

        printf("The file was received successfully\n");


     close(newsockfd);
     close(sockfd);
     return 0;
}

