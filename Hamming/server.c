

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8888
#define BUFFER_SIZE 1024


char* hamming_encode(const char *data) {
   
    static char codeword[8];  
    if (strlen(data) != 4) {
        fprintf(stderr, "Error: Data word must be exactly 4 bits.\n");
        exit(EXIT_FAILURE);
    }

    
    int d[4];
    for (int i = 0; i < 4; i++) {
        if (data[i] != '0' && data[i] != '1') {
            fprintf(stderr, "Error: Data word must contain only 0 and 1.\n");
            exit(EXIT_FAILURE);
        }
        d[i] = data[i] - '0';
    }

    
    int p1 = d[0] ^ d[1] ^ d[3];  
    int p2 = d[0] ^ d[2] ^ d[3];  
    int p3 = d[1] ^ d[2] ^ d[3]; 

    
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

   
    memset(&server_addr, 0, sizeof(server_addr));

    // Fill in the server address information.
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;  // Accept any incoming address
    server_addr.sin_port = htons(PORT);

    
    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Server: bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    
    printf("UDP Server is running on port %d...\n", PORT);

    
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

       
        char *codeword = hamming_encode(buffer);
        printf("Computed Hamming codeword: %s\n", codeword);

        
        if (sendto(sockfd, codeword, strlen(codeword), 0,
                   (struct sockaddr*)&client_addr, addr_len) < 0) {
            perror("Server: sendto failed");
        }
    }

    close(sockfd);
    return 0;
}
