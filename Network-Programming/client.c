#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#define BUFF_SIZE 1024
int i = 0;
int main(void)
{
    int client_socket;
    struct sockaddr_in server_addr;
    char msg[BUFF_SIZE] = "";
    char buff[BUFF_SIZE + 5];
    client_socket = socket(PF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        printf("socket 생성 실패\n");
        exit(1);
    }
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(4000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        printf("접속 실패\n");
        exit(1);
    }
    while (strcmp(msg, "bye") != 0)
    {
        printf("[client_send] ");
        scanf("%s", msg);
        write(client_socket, msg, strlen(msg) + 1); // +1: NULL까지 포함해서 전송
        read(client_socket, buff, BUFF_SIZE + 1);   /*  */
        if (strcmp(msg, "bye") != 0)
            printf("[server_recv] %s\n", buff);
    }
    // close(client_socket);
    return 0;
}
