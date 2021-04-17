#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define F_MAX 100
#define READ_SIZE 11
#define SA struct sockaddr 

void error_helper(int code)         //function to handle errors
{
    switch (code)
    {
    case 1:
        printf("No port number specified\n");
        exit(1);
        break;
    case 2:
        printf("Error creating a socket\n");
        exit(2);
        break;
    case 3:
        printf("Could not bind.\n");
        exit(3);
        break;
    case 4:
        printf("Could not listen.\n");
        exit(4);
        break;
    case 5:
        printf("Could not read from client.\n");
        exit(5);
        break;
    case 6:
        printf("Could not send to client.\n");
        exit(6);
        break;
    case 7:
        printf("Could not complete handshake with client.\n");
        exit(7);
        break;
    
    default:
        break;
    }
    exit(7);
}


int main(int argc, char *argv[])
{
    if(argc<2)              // no port number specified 
    {
        error_helper(1);
    }

    int port_no = atoi(argv[1]);

    printf("%d\n", port_no);
    struct sockaddr_in servaddr;
    int sockfd;
    int connectfd;
    FILE *fp;
    char FILE_NAME[F_MAX];
    char FILE_CONTENTS[READ_SIZE];


    if((sockfd = socket(AF_INET, SOCK_STREAM, 0))<0)      //creating a tcp socket 
    {
        error_helper(2);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);       //respond to any client
    servaddr.sin_port = htons(port_no);

    if(bind(sockfd, (SA*)&servaddr, sizeof(servaddr))!=0)        //binding and listening to incoming connections 
    {
        error_helper(3);
    }

    if((listen(sockfd, 0))<0)
    {
        error_helper(4);
    }

    while(1)
    {
        printf("waiting for a client!\n");
        fflush(stdout);

        if((connectfd = accept(sockfd, (struct sockaddr *)NULL, NULL))<0)           //fd which allows us to talk to the client
        {
            printf("could not connect to client!\n");
            fflush(stdout);
            continue;

            // error_helper(7);
        }

        printf("connected to a client!\n");
        fflush(stdout);

        printf("just testing something\n");
        close(connectfd);

        while(1)
        {
            memset(FILE_NAME, 0, F_MAX);
            memset(FILE_CONTENTS, 0, READ_SIZE);
            struct sockaddr_in addr;
            socklen_t addr_len;

            if((read(connectfd, FILE_NAME, sizeof(FILE_NAME)))<0)
            {
                error_helper(5);
            }
            if(strcmp("exit",FILE_NAME)==0)
            {
                break;
            }
            printf("the filename to be opened is : %s\n",FILE_NAME);
            fp = fopen(FILE_NAME, "r");
            if(fp == NULL)
            {
                printf("file was not found. sending empty string.\n");
                fflush(stdout);
                if((write(connectfd, FILE_CONTENTS, sizeof(FILE_CONTENTS)))<0)
                {
                    error_helper(6);
                }
                continue;
            }
            printf("file was found.\n");
            fflush(stdout);
            fgets(FILE_CONTENTS, READ_SIZE, fp);
            fclose(fp);
            if((write(connectfd, FILE_CONTENTS, sizeof(FILE_CONTENTS)))<0)
            {
                error_helper(6);
            }

        }
        printf("client has exited.\n");
        fflush(stdout);
    }
    

    return 0;
}
