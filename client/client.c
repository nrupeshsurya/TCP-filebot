#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>


#define MAX_SIZE 100
#define READ_SIZE 11

int main(int argc, char *argv[])
{
    long arg; 
    char temp;
    if(argc < 3)
    {
        printf("please enter the parameters ip address, port number.\n");
        exit(1);
    }

    char *address = argv[1];
    printf("%s\n",address);
    int port_no = atoi(argv[2]);
    char file_name [MAX_SIZE];
    char content [READ_SIZE];

    struct sockaddr_in servaddr;
    int sockfd;
    int connectfd;
    FILE* fp;

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0))<0)
    {
        printf("error in creating a socket.\n");
        exit(2);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port_no);
    servaddr.sin_addr.s_addr = inet_addr(address);

    // if( (arg = fcntl(sockfd, F_GETFL, NULL)) < 0) { 
    // fprintf(stderr, "Error fcntl(..., F_GETFL) (%s)\n", strerror(errno)); 
    // exit(0); 
    // } 
    // arg |= O_NONBLOCK; 
    // if( fcntl(sockfd, F_SETFL, arg) < 0) { 
    //     fprintf(stderr, "Error fcntl(..., F_SETFL) (%s)\n", strerror(errno)); 
    //     exit(0); 
    // } 

    int res = connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    if(res<0)
    {
        printf("%d \n",res);
        if(errno == EINPROGRESS)
        {
            printf("well this is awkward\n");
        }
        printf("hi.\n");

        exit(3);
    }

    while (1)
    {
        memset(file_name, 0, MAX_SIZE);
        memset(content, 0, READ_SIZE);
        printf("enter the filename to be sent from server and press enter: ");
        fgets(file_name, MAX_SIZE, stdin);

        if(strcmp("exit\n",file_name)==0)
        {
            if(write(sockfd, "exit", strlen("exit"))<0)
            {
                printf("error in sending file.\n");
                exit(4);
            }
            break;
        }

        for(int i=0;i<MAX_SIZE;i++)
        {
            if(file_name[i]=='\n')
            {
                file_name[i] = '\0';
                break;
            }
        }

        if(write(sockfd, file_name, strlen(file_name))<0)
        {
            printf("error in sending file.\n");
            exit(4);
        }

        if(read(sockfd, content, READ_SIZE)<1)
        {
            printf("error in reading.\n");
            exit(5);
        }
        if(strcmp("",content)!=0)
        {
            // printf("%s\n",content);
            fp = fopen(file_name, "w");
            fprintf(fp,"%s",content);
            fclose(fp);
            printf("File has been created and content has been updated!\n");
        }
        else
        {
            printf("The file doesn't exist in the client!\n");
        }

    }
    

    return 0;
}
