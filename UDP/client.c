#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9734
#define BUFSIZE 1024
#define MAX_BIN_LENGTH 100  // Limit each input to 99 characters (plus '\0')

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char dataword[MAX_BIN_LENGTH], divisor[MAX_BIN_LENGTH];
    char send_buffer[BUFSIZE], recv_buffer[BUFSIZE];
    int bytes_received;

    // Create a TCP socket.
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up server address.
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    memset(&(server_addr.sin_zero), 0, 8);

    // Connect to the server.
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) < 0) {
        perror("connect");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Get input from the user.
    printf("Enter the dataword (binary string, max %d characters): ", MAX_BIN_LENGTH - 1);
    if (!fgets(dataword, sizeof(dataword), stdin)) {
        fprintf(stderr, "Error reading dataword.\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    dataword[strcspn(dataword, "\n")] = '\0';  // Remove newline

    printf("Enter the divisor (binary string, max %d characters): ", MAX_BIN_LENGTH - 1);
    if (!fgets(divisor, sizeof(divisor), stdin)) {
        fprintf(stderr, "Error reading divisor.\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    divisor[strcspn(divisor, "\n")] = '\0';    // Remove newline

    // Check if combined length fits into send_buffer.
    if ((strlen(dataword) + strlen(divisor) + 2 + 1) > BUFSIZE) { // 2 for newlines, 1 for null terminator
        fprintf(stderr, "Error: Input strings are too long.\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Prepare the message to send: dataword and divisor separated by newline.
    snprintf(send_buffer, sizeof(send_buffer), "%s\n%s\n", dataword, divisor);

    // Send the message to the server.
    if (write(sockfd, send_buffer, strlen(send_buffer)) < 0) {
        perror("write");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Receive the codeword from the server.
    bytes_received = read(sockfd, recv_buffer, BUFSIZE - 1);
    if (bytes_received < 0) {
        perror("read");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    recv_buffer[bytes_received] = '\0';
    printf("Received codeword: %s\n", recv_buffer);

    close(sockfd);
    return 0;
}
