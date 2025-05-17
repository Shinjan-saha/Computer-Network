#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <termios.h>
#include <stdlib.h>

void *recv_thread(void *a);
void *send_thread(void *a);
int cfd;
pthread_t sample;

int main() {
    struct sockaddr_in cl, ser;
    int n;
    pthread_t snd, rcv;

    cfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&cl, 0, sizeof(cl));
    memset(&ser, 0, sizeof(ser));

    cl.sin_family = AF_INET;
    inet_aton("10.10.104.220", &(cl.sin_addr)); // Client IP
    cl.sin_port = htons(8760);

    ser.sin_family = AF_INET;
    inet_aton(" 192.168.1.5", &(ser.sin_addr)); // Server IP
    ser.sin_port = htons(8760);

    if (connect(cfd, (struct sockaddr *)&ser, sizeof(ser)) < 0) {
        printf("Unable to establish connection!\n");
        exit(0);
    }

    pthread_create(&snd, NULL, send_thread, NULL);
    pthread_create(&rcv, NULL, recv_thread, NULL);

    pthread_join(snd, NULL);
    pthread_join(rcv, NULL);
    return 0;
}

void *send_thread(void *a) {
    char str[100];
    while (1) {
        fgets(str, 100, stdin);
        write(cfd, str, strlen(str) + 1);
        if (strcmp(str, "Bye\n") == 0) {
            close(cfd);
            exit(0);
        }
    }
}

void *recv_thread(void *a) {
    int n;
    char str[100];
    sample = pthread_self();

    while (1) {
        n = read(cfd, str, sizeof(str));
        if (n <= 0) {
            printf("Connection closed by server\n");
            close(cfd);
            exit(0);
        }
        printf("Server : %s", str);
        fflush(stdout);
    }
}
