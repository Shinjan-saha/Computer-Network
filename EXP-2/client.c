#include <sys/types. h> 
#include<sys/socket .h>
#include <stdio.h> 
#include <sys/un.h> 
#include <unistd.h> 
#include <stdlib.h> 




int main()
{
int sockfd;
int len;
struct sockaddr_un
address; int result;
int stream[5], i;
sockfd = socket(AF_UNIX,
SOCK_STREAM,0);
address.sun_family = AF_UNIX; strcpy(address.sun_path, "server_socket");
len = sizeof(address);
result = connect(sockfd, (struct sockaddr *)&address, len);
   for(i=0;i<5;i++) {
printf("Enter a no. "); scanf("%d",&stream[i ]);
}
//Connect to the server if(result == -1)
{
perror("oops:
client1"); exit(1); }
for(i=0;i<5;i++) {
write(sockfd, &stream[i], sizeof(int)); }
for(i=0;i<5;i++) {
read(sockfd, &stream[i], sizeof(int)); }
//read and write via sockfd for(i=0;i<5;i++)
 
{
printf("char from server = %d\n", stream[i]);
}
close(sockfd);
// close the socket connection
exit(0); }