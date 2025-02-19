#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main() 
{ 
    int sockfd; 
    int len; 
    struct sockaddr_un address; 
    int result; 
    int stream[5], i; 

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);  
    address.sun_family = AF_UNIX; 
    strcpy(address.sun_path, "server_socket"); 

    len = sizeof(address); 
    result = connect(sockfd, (struct sockaddr *)&address, len); 

    if(result == -1) 
    { 
        perror("oops: client1"); 
        exit(1); 
    } 

    for(i = 0; i < 5; i++) 
    { 
        printf("Enter 5 numbers one line after another: "); 
        scanf("%d", &stream[i]); 
    } 

    for(i = 0; i < 5; i++) 
    { 
        write(sockfd, &stream[i], sizeof(int)); 
    } 

    for(i = 0; i < 5; i++) 
    { 
        read(sockfd, &stream[i], sizeof(int)); 
    } 

    printf("Sorted numbers from server:\n");
    for(i = 0; i < 5; i++) 
    { 
        printf("%d\n", stream[i]); 
    } 

    close(sockfd); 
    exit(0); 
}
