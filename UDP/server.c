#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <errno.h>

#define PORT 9734
#define BUFSIZE 1024

// Function to perform modulo-2 division (CRC calculation)
char* compute_crc(const char *data, const char *divisor) {
    int n = strlen(data);
    int k = strlen(divisor);
    int total = n + k - 1;

    // Create a working copy: dataword followed by (k-1) zeros.
    char *augmented = malloc(total + 1);
    if (!augmented) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    strcpy(augmented, data);
    memset(augmented + n, '0', k - 1);
    augmented[total] = '\0';

    // Perform modulo-2 division using XOR
    for (int i = 0; i < n; i++) {
        if (augmented[i] == '1') { // Only if bit is 1, perform XOR with divisor
            for (int j = 0; j < k; j++) {
                augmented[i+j] = (augmented[i+j] == divisor[j]) ? '0' : '1';
            }
        }
    }

    // The remainder is in the last k-1 bits.
    char *remainder = malloc(k);
    if (!remainder) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    strncpy(remainder, augmented + n, k - 1);
    remainder[k - 1] = '\0';

    // The codeword is the original data concatenated with the remainder.
    char *codeword = malloc(n + k);
    if (!codeword) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    strcpy(codeword, data);
    strcat(codeword, remainder);

    free(augmented);
    free(remainder);
    return codeword;
}

// Thread function to handle each client
void* handle_client(void *arg) {
    int client_sock = *(int *)arg;
    free(arg);
    char buffer[BUFSIZE];
    int bytes_read;

    // Expecting two lines: first line is dataword, second line is divisor.
    // Clear buffer.
    memset(buffer, 0, BUFSIZE);
    // Read the dataword
    bytes_read = read(client_sock, buffer, BUFSIZE - 1);
    if (bytes_read <= 0) {
        close(client_sock);
        pthread_exit(NULL);
    }
    buffer[bytes_read] = '\0';

    // Tokenize the input. Assume dataword and divisor are separated by newline.
    char *dataword = strtok(buffer, "\n");
    char *divisor = strtok(NULL, "\n");

    if (!dataword || !divisor) {
        fprintf(stderr, "Invalid input received from client.\n");
        close(client_sock);
        pthread_exit(NULL);
    }
    
    printf("Received dataword: %s\n", dataword);
    printf("Received divisor: %s\n", divisor);

    // Compute the CRC codeword.
    char *codeword = compute_crc(dataword, divisor);
    printf("Sending codeword: %s\n", codeword);

    // Send the codeword back to the client.
    write(client_sock, codeword, strlen(codeword));
    close(client_sock);
    free(codeword);
    pthread_exit(NULL);
}

int main() {
    int server_sock, *client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(struct sockaddr_in);

    // Create a TCP socket.
    if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up the server address.
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    memset(&(server_addr.sin_zero), 0, 8);

    // Bind the socket to the specified port.
    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) < 0) {
        perror("bind");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections.
    if (listen(server_sock, 10) < 0) {
        perror("listen");
        close(server_sock);
        exit(EXIT_FAILURE);
    }
    
    printf("Server listening on port %d...\n", PORT);

    // Main accept loop.
    while (1) {
        client_sock = malloc(sizeof(int));
        if ((*client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &addr_len)) < 0) {
            perror("accept");
            free(client_sock);
            continue;
        }
        printf("Connection accepted from %s\n", inet_ntoa(client_addr.sin_addr));

        // Create a new thread for each client connection.
        pthread_t tid;
        if (pthread_create(&tid, NULL, handle_client, client_sock) != 0) {
            perror("pthread_create");
            close(*client_sock);
            free(client_sock);
            continue;
        }
        pthread_detach(tid);  // Automatically reclaim thread resources.
    }
    
    close(server_sock);
    return 0;
}
