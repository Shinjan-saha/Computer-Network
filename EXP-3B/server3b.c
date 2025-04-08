#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>

#define MAX 15

void *connection_handler(void *client_sockfd_ptr)
{
    int client_sockfd = *(int *)client_sockfd_ptr;
    char dataword[MAX], divisor[MAX], codeword[MAX];
    
    read(client_sockfd, dataword, sizeof(dataword));
    read(client_sockfd, divisor, sizeof(divisor));
    
    // Dummy operation: just concat for now
    snprintf(codeword, MAX, "%s%s", dataword, divisor);
    
    write(client_sockfd, codeword, sizeof(codeword));
    close(client_sockfd);
    free(client_sockfd_ptr);
    pthread_exit(NULL);
}

int main()
{
    int server_sockfd, *client_sockfd;
    struct sockaddr_in server_address, client_address;
    socklen_t client_len = sizeof(client_address);
    pthread_t thread_id;

    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(9734);

    if (bind(server_sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    listen(server_sockfd, 5);
    printf("Server listening on port 9734...\n");

    while (1) {
        client_sockfd = malloc(sizeof(int));
        *client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
        if (*client_sockfd < 0) {
            perror("Accept failed");
            free(client_sockfd);
            continue;
        }
        pthread_create(&thread_id, NULL, connection_handler, client_sockfd);
        pthread_detach(thread_id);
    }

    close(server_sockfd);
    return 0;
}
