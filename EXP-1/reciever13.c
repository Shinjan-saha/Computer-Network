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

int main() {
    int running = 1;
    int msgid;
    struct my_msg_st some_data;
    long int msg_to_receive = 3;

    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
    while (running) {
        msgrcv(msgid, (void*)&some_data, MAX_TEXT, msg_to_receive, 0);
        if (strncmp(some_data.some_text, "end", 3) == 0) running = 0;
        else printf("Octal Representation: %s\n", some_data.some_text);
    }
    exit(EXIT_SUCCESS);
}
