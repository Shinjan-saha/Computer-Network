#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    int client_sockfd, server_sockfd;
    socklen_t client_len;  
    int server_len;
    int stream[100];
    int i, j, k, count = 0, length = 0;

    struct sockaddr_in server_address;
    struct sockaddr_in client_address;

    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = 9734;
    server_len = sizeof(server_address);

    bind(server_sockfd, (struct sockaddr *)&server_address, server_len);
    listen(server_sockfd, 5);
    printf("Server started.\n");

    while(1)
    {
        printf("Server waiting\n");
        client_len = sizeof(client_address);  
        client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);  

        
        for(i = 0; i < 10; i++)
        {
            read(client_sockfd, &stream[i], sizeof(int));
        }

        
        for(i = 0; i < 10; i++)
        {
            if(stream[i] == 0 && stream[i+1] == 1 && stream[i+2] == 1 && stream[i+3] == 1 &&
               stream[i+4] == 1 && stream[i+5] == 1)
            {
                for(j = 5 + count; j > i; j--)
                {
                    stream[j+1] = stream[j];
                }
                stream[i+6] = 0;
                count++;
            }
        }

        
        for(i = 0; i < 10 + count; i++)
        {
            write(client_sockfd, &stream[i], sizeof(int));
        }
    }
}
