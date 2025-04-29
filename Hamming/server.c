/*
 * server.c
 * UDP server that receives a 4-bit data word, computes the Hamming(7,4) codeword,
 * and sends the codeword back to the client.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8888
#define BUFFER_SIZE 1024

// Function to encode a 4-bit dataword using Hamming(7,4) code.
// The input string "data" must be exactly 4 characters long ('0' or '1').
// Returns a pointer to a statically allocated string holding the 7-bit codeword.
char* hamming_encode(const char *data) {
    // Ensure the data word is 4 bits.
    static char codeword[8];  // 7 bits plus the null terminator
    if (strlen(data) != 4) {
        fprintf(stderr, "Error: Data word must be exactly 4 bits.\n");
        exit(EXIT_FAILURE);
    }

    // Convert input characters to bit values (0 or 1)
    int d[4];
    for (int i = 0; i < 4; i++) {
        if (data[i] != '0' && data[i] != '1') {
            fprintf(stderr, "Error: Data word must contain only 0 and 1.\n");
            exit(EXIT_FAILURE);
        }
        d[i] = data[i] - '0';
    }

    // Compute parity bits using XOR (⊕)
    int p1 = d[0] ^ d[1] ^ d[3];  // parity for positions 1 (covers d0, d1, d3)
    int p2 = d[0] ^ d[2] ^ d[3];  // parity for position 2 (covers d0, d2, d3)
    int p3 = d[1] ^ d[2] ^ d[3];  // parity for position 4 (covers d1, d2, d3)

    // Arrange bits into the codeword: positions 1 to 7.
    // Positions:  1    2    3    4    5    6    7
    // Bits:     p1   p2    d0   p3    d1   d2   d3
    codeword[0] = p1 + '0';
    codeword[1] = p2 + '0';
    codeword[2] = d[0] + '0';
    codeword[3] = p3 + '0';
    codeword[4] = d[1] + '0';
    codeword[5] = d[2] + '0';
    codeword[6] = d[3] + '0';
    codeword[7] = '\0';

    return codeword;
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(client_addr);
    ssize_t num_bytes;

    // Create a UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Server: socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Zero out the server address structure
    memset(&server_addr, 0, sizeof(server_addr));

    // Fill in the server address information.
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;  // Accept any incoming address
    server_addr.sin_port = htons(PORT);

    // Bind the socket to the port
    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Server: bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    
    printf("UDP Server is running on port %d...\n", PORT);

    // Wait for incoming messages in an infinite loop.
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        num_bytes = recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0,
                             (struct sockaddr*)&client_addr, &addr_len);
        if (num_bytes < 0) {
            perror("Server: recvfrom failed");
            continue;
        }
        
        buffer[num_bytes] = '\0';
        printf("Received data word \"%s\" from client.\n", buffer);

        // Compute Hamming code (7,4) for the incoming data
        char *codeword = hamming_encode(buffer);
        printf("Computed Hamming codeword: %s\n", codeword);

        // Send the codeword back to the client
        if (sendto(sockfd, codeword, strlen(codeword), 0,
                   (struct sockaddr*)&client_addr, addr_len) < 0) {
            perror("Server: sendto failed");
        }
    }

    close(sockfd);
    return 0;
}
