#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/msg.h>

#define MAX_TEXT 512

struct my_msg_st {
    long int my_msg_type;
    char some_text[MAX_TEXT];
};

int decimalToOctal(int num) {
    int octal = 0, place = 1;
    while (num > 0) {
        int remainder = num % 8;
        octal += remainder * place;
        num /= 8;
        place *= 10;
    }
    return octal;
}

int main() {
    int running = 1;
    struct my_msg_st some_data_oct;
    int msgid;
    char buffer[MAX_TEXT];

    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
    if (msgid == -1) {
        fprintf(stderr, "msgget failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    while (running) {
        printf("Enter a decimal number (or 'end' to exit): ");
        fgets(buffer, MAX_TEXT, stdin);
        
        buffer[strcspn(buffer, "\n")] = 0;

        if (strncmp(buffer, "end", 3) == 0) {
            strcpy(some_data_oct.some_text, "end");
            some_data_oct.my_msg_type = 2;
        } else {
            int num = atoi(buffer);
            int octal = decimalToOctal(num);
            snprintf(some_data_oct.some_text, MAX_TEXT, "%d", octal);
            some_data_oct.my_msg_type = 2;
        }

        if (msgsnd(msgid, (void*)&some_data_oct, MAX_TEXT, 0) == -1) {
            fprintf(stderr, "msgsnd failed\n");
            exit(EXIT_FAILURE);
        }

        if (strncmp(buffer, "end", 3) == 0) {
            running = 0;
        }
    }

    exit(EXIT_SUCCESS);
}
