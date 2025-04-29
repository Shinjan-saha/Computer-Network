#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main() {
int server_sockfd, client_sockfd;
socklen_t server_len, client_len;
struct sockaddr_in server_address, client_address;
int stream[100], i, j, count = 0;

// Creating a socket
server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
if (server_sockfd == -1) {
perror("Error creating socket");
exit(1);
}

// Setting up the server address
server_address.sin_family = AF_INET;
server_address.sin_addr.s_addr = inet_addr("127.0.0.1"); // Localhost IP
server_address.sin_port = htons(9734); // Port number
server_len = sizeof(server_address);

// Binding the socket to the address
if (bind(server_sockfd, (struct sockaddr *)&server_address, server_len) == -1) {
perror("Error binding socket");
close(server_sockfd);
exit(1);
}

// Listening for incoming connections
if (listen(server_sockfd, 5) == -1) {
perror("Error listening for connections");
close(server_sockfd);
exit(1);
}

printf("Server started.\n");

while (1) {
printf("Server waiting...\n");

// Accepting a client connection
client_len = sizeof(client_address);
client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
if (client_sockfd == -1) {
perror("Error accepting connection");
continue; // Continue to wait for the next connection
}

// Reading the data stream from the client
for (i = 0; i < 10; i++) {
if (read(client_sockfd, &stream[i], sizeof(int)) == -1) {
perror("Error reading from socket");
close(client_sockfd);
continue;
}
}

// Performing bit-stuffing operation
for (i = 0; i < 10; i++) {
if (stream[i] == 0 && stream[i + 1] == 1 && stream[i + 2] == 1 &&
stream[i + 3] == 1 && stream[i + 4] == 1 && stream[i + 5] == 1) {
for (j = 9 + count; j > i + 5; j--) { // Shift right to make space for stuffed bit
stream[j + 1] = stream[j];
}
stream[i + 6] = 0; // Insert stuffed bit
count++;
}
}

// Writing the modified data back to the client
for (i = 0; i < 10 + count; i++) {
if (write(client_sockfd, &stream[i], sizeof(int)) == -1) {
perror("Error writing to socket");
break;
}
}

close(client_sockfd); // Close the connection with the current client
}

close(server_sockfd); // Close the server socket when done
return 0;
}
