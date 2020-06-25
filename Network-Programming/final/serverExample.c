#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#define ALL -1
struct cliInfo
{
    int sockno;
};
int clients[100];
int n = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int parser(char *);
void sendToTargetUser(char *, int);
void sendAll(char *, int);
int findMessageStart(char *);
void *recvmg(void *);
int main(int argc, char *argv[])
{
    struct sockaddr_in my_addr, their_addr;
    int sockfd;
    int their_sock;
    socklen_t their_addr_size;
    int portno;
    pthread_t sendt, recvt;
    char msg[500];
    int len;
    struct cliInfo cl;
    if (argc > 2)
    {
        printf("too many arguments");
        exit(1);
    }
    portno = atoi(argv[1]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(my_addr.sin_zero, '\0', sizeof(my_addr.sin_zero));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(portno);
    my_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    their_addr_size = sizeof(their_addr);

    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(my_addr)) != 0)
    {
        perror("binding error");
        close(sockfd);
        exit(1);
    }

    if (listen(sockfd, 5) != 0) // listening for incoming connection requests and return 0 on success,
    {
        perror("listening error");
        close(sockfd);
        exit(1);
    }

    while (1)
    {
        if ((their_sock = accept(sockfd, (struct sockaddr *)&their_addr, &their_addr_size)) < 0)
        {
            perror("accept error");
            close(sockfd);
            exit(1);
        }
        pthread_mutex_lock(&mutex);
        printf("A new client connected.\n");
        cl.sockno = their_sock;
        clients[n] = their_sock;
        n++;
        pthread_create(&recvt, NULL, recvmg, &cl);
        pthread_mutex_unlock(&mutex);
    }
    return 0;
}

int parser(char *msg)
{
    char firstChar = msg[findMessageStart(msg)];
    if (firstChar >= '1' && firstChar <= '9')
        return firstChar - '1';
    return ALL;
}
void sendToTargetUser(char *msg, int target)
{
    pthread_mutex_lock(&mutex);
    if (send(clients[target], msg, strlen(msg), 0) < 0)
        perror("send error");
    pthread_mutex_unlock(&mutex);
}
void sendAll(char *msg, int curr)
{
    int i;
    pthread_mutex_lock(&mutex);
    for (i = 0; i < n; i++)
        if (clients[i] != curr)
            if (send(clients[i], msg, strlen(msg), 0) < 0)
            {
                perror("send error");
                continue;
            }
    pthread_mutex_unlock(&mutex);
}
int findMessageStart(char *msg)
{
    int i = 0;
    while (msg[i] != ':')
        i++;
    return i + 1;
}
void *recvmg(void *sock)
{
    struct cliInfo cl = *((struct cliInfo *)sock);
    char msg[500];
    int len;
    while ((len = recv(cl.sockno, msg, 500, 0)) > 0)
    {
        msg[len] = '\0';
        if (parser(msg) == ALL)
            sendAll(msg, cl.sockno);
        else
            sendToTargetUser(msg, msg[findMessageStart(msg)] - '1');

        memset(msg, '\0', sizeof(msg));
    }
    pthread_mutex_lock(&mutex);
    printf(" Client disconnected.\n");
    pthread_mutex_unlock(&mutex);
}