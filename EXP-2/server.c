#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>

int main() 
{ 
    int server_sockfd, client_sockfd; 
    int server_len, client_len; 
    struct sockaddr_un server_address; 
    struct sockaddr_un client_address; 

    unlink("server_socket"); 

    server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0); 
    server_address.sun_family = AF_UNIX; 
    strcpy(server_address.sun_path, "server_socket"); 
    server_len = sizeof(server_address); 

    bind(server_sockfd, (struct sockaddr *)&server_address, server_len); 
    listen(server_sockfd, 5); 

    while(1) 
    { 
        int stream[5]; 
        printf("server waiting\n"); 
        int i, j; 

        client_len = sizeof(client_address); 
        client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len); 

        for(i = 0; i < 5; i++) 
        { 
            read(client_sockfd, &stream[i], sizeof(int)); 
        } 

        for(i = 0; i < 5; i++) 
        { 
            for(j = i + 1; j < 5; j++) 
            { 
                if(stream[i] > stream[j]) 
                { 
                    int a = stream[i]; 
                    stream[i] = stream[j]; 
                    stream[j] = a; 
                } 
            } 
        } 

        for(i = 0; i < 5; i++) 
        { 
            write(client_sockfd, &stream[i], sizeof(int)); 
        } 

        close(client_sockfd); // close the socket connection 
    } 
}
