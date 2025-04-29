/*
 * client.c
 * UDP client that reads a 4-bit binary data word from the user,
 * sends it to the server, receives the Hamming(7,4) codeword, and displays it.
 */

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
    char data_word[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(server_addr);
    ssize_t num_bytes;

    // Create a UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Client: socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Zero out the server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    // Set the server IP address here.
    // For example, if the server runs on localhost, use 127.0.0.1.
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("Client: invalid address/ Address not supported");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Prompt the user for a 4-bit data word.
    printf("Enter a 4-bit binary data word (e.g., 1010): ");
    if (fgets(data_word, BUFFER_SIZE, stdin) == NULL) {
        perror("Client: error reading input");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    // Remove newline character if present.
    data_word[strcspn(data_word, "\n")] = '\0';

    // Validate length of data word.
    if (strlen(data_word) != 4) {
        fprintf(stderr, "Error: Data word must be exactly 4 bits.\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Send the data word to the server.
    if (sendto(sockfd, data_word, strlen(data_word), 0,
               (struct sockaddr*)&server_addr, addr_len) < 0) {
        perror("Client: sendto failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Data word \"%s\" sent to the server.\n", data_word);

    // Receive the Hamming codeword from the server.
    memset(buffer, 0, BUFFER_SIZE);
    num_bytes = recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0,
                         (struct sockaddr*)&server_addr, &addr_len);
    if (num_bytes < 0) {
        perror("Client: recvfrom failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    buffer[num_bytes] = '\0';

    // Display the received codeword.
    printf("Received Hamming codeword from server: %s\n", buffer);

    close(sockfd);
    return 0;
}
