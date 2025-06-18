#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_PORT 8888
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char data_word[BUFFER_SIZE], buffer[BUFFER_SIZE];

   
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Client: socket creation failed");
        exit(EXIT_FAILURE);
    }

   
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("Client: invalid address");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Client: connect failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    
    printf("Enter a 4-bit binary data word (e.g., 1010): ");
    if (fgets(data_word, BUFFER_SIZE, stdin) == NULL) {
        perror("Client: error reading input");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    
    data_word[strcspn(data_word, "\n")] = '\0';

   
    if (strlen(data_word) != 4) {
        fprintf(stderr, "Error: Data word must be exactly 4 bits.\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

   
    write(sockfd, data_word, strlen(data_word));
    printf("Sent data word \"%s\" to server.\n", data_word);

   
    memset(buffer, 0, BUFFER_SIZE);
    read(sockfd, buffer, BUFFER_SIZE);
    printf("Received Hamming codeword from server: %s\n", buffer);

    close(sockfd);
    return 0;
}
