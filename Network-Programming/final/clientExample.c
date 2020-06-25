#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

int isEndMsg(char *);
void *recvmg(void *);
int sockfd;
int main(int argc, char *argv[])
{
    struct sockaddr_in their_addr;
    int their_sock;
    int their_addr_size;
    int portno;
    pthread_t sendt, recvt;
    char msg[500];
    char username[100];
    char res[600];
    int len;

    if (argc > 3)
    {
        printf("too many arguments");
        exit(1);
    }
    portno = atoi(argv[2]);
    strcpy(username, argv[1]);

    // create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(their_addr.sin_zero, '\0', sizeof(their_addr.sin_zero));
    their_addr.sin_family = AF_INET;
    their_addr.sin_port = htons(portno);
    their_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sockfd, (struct sockaddr *)&their_addr, sizeof(their_addr)) < 0)
    {
        perror("connection not established");
        close(sockfd);
        exit(1);
    }
    pthread_create(&recvt, NULL, recvmg, &sockfd);
    printf("Start chatting:\n");
    while (fgets(msg, 500, stdin) > 0)
    {
        strcpy(res, username);
        strcat(res, ":");
        strcat(res, msg);
        len = write(sockfd, res, strlen(res));
        if (len < 0)
        {
            perror("message not sent");
            exit(1);
        }
        memset(msg, '\0', sizeof(msg));
        memset(res, '\0', sizeof(res));
    }
    pthread_join(recvt, NULL);
    close(sockfd);
}

int isEndMsg(char *msg)
{
    int i = 0;

    while (msg[i] != '\0')
        i++;
    if (i < 4)
        return 0;
    return msg[i - 4] == 'b' && msg[i - 3] == 'y' && msg[i - 2] == 'e';
}

void *recvmg(void *sock)
{
    int their_sock = *((int *)sock);
    char msg[500];
    int len;
    while ((len = recv(their_sock, msg, 500, 0)) > 0)
    {
        msg[len] = '\0';
        if (isEndMsg(msg))
        {
            printf("chat end\n");
            exit(1);
        }
        fputs(msg, stdout);
        memset(msg, '\0', sizeof(msg));
    }
}