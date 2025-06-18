#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAXSIZE 100

int main() {
    int sockfd;
    char dataword[MAXSIZE], divisor[MAXSIZE], buffer[MAXSIZE];
    struct sockaddr_in server_addr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9734);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

   
    printf("Enter the dataword: ");
    fgets(dataword, MAXSIZE, stdin);
    sendto(sockfd, dataword, strlen(dataword), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

    
    printf("Enter the divisor: ");
    fgets(divisor, MAXSIZE, stdin);
    sendto(sockfd, divisor, strlen(divisor), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

    
    socklen_t addr_len = sizeof(server_addr);
    recvfrom(sockfd, buffer, MAXSIZE, 0, (struct sockaddr*)&server_addr, &addr_len);

    printf("Codeword from Server = %s\n", buffer);

    close(sockfd);
    return 0;
}
