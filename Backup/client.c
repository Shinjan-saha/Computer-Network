#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main() {
int sockfd;
struct sockaddr_in address;
int result;
int i, n = 10, stream[100];
socklen_t len;

// Accepting input from the user
printf("Enter the data stream (10 integers):\n");
for (i = 0; i < n; i++) {
scanf("%d", &stream[i]);
}

// Creating a socket
sockfd = socket(AF_INET, SOCK_STREAM, 0);
if (sockfd == -1) {
perror("Error creating socket");
exit(1);
}

// Setting up the server address
address.sin_family = AF_INET;
address.sin_port = htons(9734); // Use htons to set the port number
address.sin_addr.s_addr = inet_addr("127.0.0.1"); // Localhost IP
len = sizeof(address);

// Connecting to the server
result = connect(sockfd, (struct sockaddr *)&address, len);
if (result == -1) {
perror("Error connecting to server");
close(sockfd);
exit(1);
}

// Writing data to the TCP socket
for (i = 0; i < n; i++) {
if (write(sockfd, &stream[i], sizeof(int)) == -1) {
perror("Error writing to socket");
close(sockfd);
exit(1);
}
}

// Reading modified data from the TCP socket
printf("\nThe modified data stream is:\n");
for (i = 0; i < n + 1; i++) { // Assuming the server sends back n+1 integers
if (read(sockfd, &stream[i], sizeof(int)) == -1) {
perror("Error reading from socket");
close(sockfd);
exit(1);
}
printf("%d ", stream[i]);
}

printf("\n");

// Closing the socket
close(sockfd);

return 0;
}
