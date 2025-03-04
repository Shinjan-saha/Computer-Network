

#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    int sockfd;
    int len;
    struct sockaddr_in address;
    int result;
    int i, stream[100];

   
    printf("Enter the data stream\n");
    for(i = 0; i < 10; i++)
    {
        scanf("%d", &stream[i]);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = 9734;
    len = sizeof(address);
    result = connect(sockfd, (struct sockaddr *)&address, len);
    if(result == -1)
    {
        perror("oops : client");
        exit(1);
    }

    for(i = 0; i < 10; i++)
    {
        write(sockfd, &stream[i], sizeof(int));
    }

   
    for(i = 0; i < 10; i++)
    {
        read(sockfd, &stream[i], sizeof(int));
    }

    printf("\nThe modified data stream is\n");
    for(i = 0; i < 10; i++)  
    {
        printf("%d ", stream[i]);
    }

    exit(0);
}
