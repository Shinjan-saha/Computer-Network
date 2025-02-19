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
    struct my_msg_st some_data;
    int msgid;
    char buffer[MAX_TEXT];
    char binStr[MAX_TEXT], octalStr[MAX_TEXT], hexStr[MAX_TEXT];
    
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
            strcpy(some_data.some_text, "end");
            some_data.my_msg_type = 2;
            msgsnd(msgid, (void*)&some_data, MAX_TEXT, 0);
            some_data.my_msg_type = 3;
            msgsnd(msgid, (void*)&some_data, MAX_TEXT, 0);
            some_data.my_msg_type = 4;
            msgsnd(msgid, (void*)&some_data, MAX_TEXT, 0);
            running = 0;
        } else {
            int num = atoi(buffer);
            decimalToBinary(num, binStr);
            snprintf(octalStr, MAX_TEXT, "%o", num);
            snprintf(hexStr, MAX_TEXT, "%X", num);
            
            some_data.my_msg_type = 2;
            strcpy(some_data.some_text, binStr);
            msgsnd(msgid, (void*)&some_data, MAX_TEXT, 0);
            
            some_data.my_msg_type = 3;
            strcpy(some_data.some_text, octalStr);
            msgsnd(msgid, (void*)&some_data, MAX_TEXT, 0);
            
            some_data.my_msg_type = 4;
            strcpy(some_data.some_text, hexStr);
            msgsnd(msgid, (void*)&some_data, MAX_TEXT, 0);
        }
    }
    exit(EXIT_SUCCESS);
}
