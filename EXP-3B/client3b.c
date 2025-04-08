#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define MAXSIZE 15

int main()
{
    char val[MAXSIZE];
    int sockfd, result, len;
    struct sockaddr_in address;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(9734); // htons important!

    len = sizeof(address);
    sleep(1); // allow server to be ready

    result = connect(sockfd, (struct sockaddr *)&address, len);
    if (result == -1)
    {
        perror("Client Error!");
        exit(1);
    }

    printf("Enter the dataword: ");
    fgets(val, MAXSIZE, stdin);
    write(sockfd, val, sizeof(val));

    printf("Enter the divisor: ");
    fgets(val, MAXSIZE, stdin);
    write(sockfd, val, sizeof(val));

    read(sockfd, val, sizeof(val));
    printf("Codeword from Server = %s", val);

    close(sockfd);
    return 0;
}
