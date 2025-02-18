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

void decimalToBinary(int num, char *binStr) {
    if (num == 0) {
        strcpy(binStr, "0");
        return;
    }

    char temp[MAX_TEXT] = {0};
    int i = 0;

    while (num > 0) {
        temp[i++] = (num % 2) + '0';
        num /= 2;
    }

    temp[i] = '\0';

    
    int len = strlen(temp);
    for (int j = 0; j < len; j++) {
        binStr[j] = temp[len - j - 1];
    }
    binStr[len] = '\0';
}

int main() {
    int running = 1;
    int msgid;
    struct my_msg_st some_data;
    long int msg_to_receive = 2;

    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
    if (msgid == -1) {
        fprintf(stderr, "msgget failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    while (running) {
        if (msgrcv(msgid, (void*)&some_data, MAX_TEXT, msg_to_receive, 0) == -1) {
            fprintf(stderr, "msgrcv failed with error: %d\n", errno);
            exit(EXIT_FAILURE);
        }

        if (strncmp(some_data.some_text, "end", 3) == 0) {
            printf("Terminating receiver.\n");
            running = 0;
        } else {
            int num = atoi(some_data.some_text);
            char binStr[MAX_TEXT];
            decimalToBinary(num, binStr);
            printf("Binary Representation: %s\n", binStr);
        }
    }

    if (msgctl(msgid, IPC_RMID, 0) == -1) {
        fprintf(stderr, "msgctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
