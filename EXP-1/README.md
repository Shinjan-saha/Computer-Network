# IPC (Message queue)

# Objective:-
### Take a decimal number from user. Convert it to different bases (e.g.: 2,8,16 etc.) and send those values to message queue. Write three separate programs to read and display the binary, octal and hex value from the message queue distinctively.


## Note:- This Part is for 3 Sender 1 Reciever

# Binary Sender Code:-

```bash 
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

int decimalToBinary(int num) {
    long long int bin = 0;
    int place = 1;
    
    while (num > 0) {
        int remainder = num % 2;
        bin += remainder * place;
        num /= 2;
        place *= 10;
    }
    return bin;
}

int main() {
    int running = 1;
    struct my_msg_st some_data_bin;
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
            strcpy(some_data_bin.some_text, "end");
            some_data_bin.my_msg_type = 2;
        } else {
            int num = atoi(buffer);  
            int binary = decimalToBinary(num);
            snprintf(some_data_bin.some_text, MAX_TEXT, "%d", binary);
            some_data_bin.my_msg_type = 2;
        }

        if (msgsnd(msgid, (void*)&some_data_bin, MAX_TEXT, 0) == -1) {
            fprintf(stderr, "msgsnd failed\n");
            exit(EXIT_FAILURE);
        }

        if (strncmp(buffer, "end", 3) == 0) {
            running = 0;
        }
    }

    exit(EXIT_SUCCESS);
}

```


## Steps to Execute:-
```bash
gcc sender1.c -o test
```

```bash
./test
```


# Octal Sender Code:-

```bash 
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

```


## Steps to Execute:-
```bash
gcc sender2.c -o test2
```

```bash
./test2
```

# Hexadecimal Sender Code:-

```bash 
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

void decimalToHexadecimal(int num, char *hexStr) {
    snprintf(hexStr, MAX_TEXT, "%X", num);
}

int main() {
    int running = 1;
    struct my_msg_st some_data_hex;
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
            strcpy(some_data_hex.some_text, "end");
            some_data_hex.my_msg_type = 2;
        } else {
            int num = atoi(buffer);
            decimalToHexadecimal(num, some_data_hex.some_text);
            some_data_hex.my_msg_type = 2;
        }

        if (msgsnd(msgid, (void*)&some_data_hex, MAX_TEXT, 0) == -1) {
            fprintf(stderr, "msgsnd failed\n");
            exit(EXIT_FAILURE);
        }

        if (strncmp(buffer, "end", 3) == 0) {
            running = 0;
        }
    }

    exit(EXIT_SUCCESS);
}

```


## Steps to Execute:-
```bash
gcc sender3.c -o test3
```

```bash
./test3
```



# Reciever Code:-

```bash
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
            printf("Binary Representation: %s\n", some_data.some_text);
        }
    }

    if (msgctl(msgid, IPC_RMID, 0) == -1) {
        fprintf(stderr, "msgctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}

```

## Steps to Execute:-
```bash
gcc reciever1.c -o rest
```

```bash
./rest
```


# Working:-

##  Binary Sender Terminal:-
<img src="./img/sender.png">



## Binary Reciever Terminal:-
<img src="./img/reciever.png">


##  Octal Sender Terminal:-
<img src="./img/octal-sender.png">



## Octal Reciever Terminal:-
<img src="./img/octal-reciever.png">


##  Hexadecimal Sender Terminal:-
<img src="./img/hexadecimal-sender.png">



## Hexadecimal Reciever Terminal:-
<img src="./img/hexadecimal-reciever.png">


## Note:- This Part is for 1 Sender 3 Reciever

# Sender Code:-

```bash
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
```

## Steps to Execute:-
```bash
gcc sender11.c -o send1
```

```bash
./send1
```

# Binary Reciever Code:-

```bash
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
    long int msg_to_receive = 2;

    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
    while (running) {
        msgrcv(msgid, (void*)&some_data, MAX_TEXT, msg_to_receive, 0);
        if (strncmp(some_data.some_text, "end", 3) == 0) running = 0;
        else printf("Binary Representation: %s\n", some_data.some_text);
    }
    exit(EXIT_SUCCESS);
}
```

## Steps to Execute:-
```bash
gcc reciever12.c -o rest12
```

```bash
./rest12
```

# Octal Reciever Code:-

```bash
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

```

## Steps to Execute:-
```bash
gcc reciever13.c -o rest13
```

```bash
./rest13
```

# Hexadecimal Reciever Code:-

```bash
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
    long int msg_to_receive = 4;

    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
    while (running) {
        msgrcv(msgid, (void*)&some_data, MAX_TEXT, msg_to_receive, 0);
        if (strncmp(some_data.some_text, "end", 3) == 0) running = 0;
        else printf("Hexadecimal Representation: %s\n", some_data.some_text);
    }
    exit(EXIT_SUCCESS);
}
```

## Steps to Execute:-
```bash
gcc reciever11.c -o rest11
```

```bash
./rest11
```

# Working:-

<img src="./img/senderlogicchnaged.png">

<br >

# PrintOut Layout for Practical Copy;-

<img src="./img/print-layout-exp1.png">