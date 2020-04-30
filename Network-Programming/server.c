#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>

#define PORT_NUM 4000
#define BUFF_SIZE 1024
int i = 0;
int main(void)
{
    int server_socket;
    int client_socket;
    int client_addr_size;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    char buff_rcv[BUFF_SIZE + 5];
    char buff_snd[BUFF_SIZE + 5];
    char serverMsg[BUFF_SIZE];
    char serverIP[20];
    char clientIP[20];
    int serverPort;
    server_socket = socket(PF_INET, SOCK_STREAM, 0);
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT_NUM);
    // server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    strcpy(serverIP, inet_ntoa(server_addr.sin_addr));
    serverPort = ntohs(server_addr.sin_port);
    client_addr_size = sizeof(client_addr);
    if (-1 == bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)))
    {
        printf("bind() 실행 에러\n");
        exit(1);
    }
    if (-1 == listen(server_socket, 5))
    {
        printf("listen() 실행 실패\n");
        exit(1);
    }
    printf("Server Started. IP:%s Port:%d\n", serverIP, serverPort);
    printf("waiting connection...\n");
    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_size);
    if (client_socket == -1)
    {
        printf("connection error\n");
        exit(1);
    }
    /* printf("Connected from %s:%d...\n", inet_ntoa(client_addr.sin_addr),
           ntohs(client_addr.sin_port)); */
    printf("connected\n");
    strcpy(clientIP, inet_ntoa(client_addr.sin_addr));
    while (1)
    {
        read(client_socket, buff_rcv, BUFF_SIZE);
        if (strcmp(buff_rcv, "bye") == 0)
        {
            printf("[%s->recv] %s\n", clientIP, buff_rcv);
            printf("connection with client end.\n\n");
            close(client_socket);
            printf("waiting connection...\n");
            client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_size);
            if (client_socket == -1)
            {
                printf("connection error\n");
                exit(1);
            }
            printf("connected\n");
            strcpy(clientIP, inet_ntoa(client_addr.sin_addr));
            continue;
        }

        printf("[client %s->recv] %s\n", clientIP, buff_rcv);
        printf("[server %s->send] ", serverIP);
        scanf("%s", serverMsg);
        write(client_socket, serverMsg, strlen(serverMsg) + 1); // +1: NULL까지 포함해서 전송
        if (strcmp(serverMsg, "bye") == 0)
        {
        }
        // sprintf(buff_snd, "%d : %s", strlen(buff_rcv), buff_rcv);
        // printf("check\n");
    }
}
