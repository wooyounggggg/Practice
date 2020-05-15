#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUFF_SIZE 4096

int main(void)
{
    int seq = 1;
    int serverSocket;
    int client_addr_size;

    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    char buff_rcv[BUFF_SIZE];
    char buff_snd[BUFF_SIZE];

    serverSocket = socket(PF_INET, SOCK_DGRAM, 0);

    if (-1 == serverSocket)
    {
        printf("socket 생성 실패n");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(4000);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (-1 == bind(serverSocket, (struct sockaddr *)&server_addr, sizeof(server_addr)))
    {
        printf("bind() 실행 에러n");
        exit(1);
    }

    while (strcmp(buff_rcv, "0x1A") != 0)
    {
        client_addr_size = sizeof(client_addr);
        recvfrom(serverSocket, buff_rcv, BUFF_SIZE, 0,
                 (struct sockaddr *)&client_addr, &client_addr_size);
        printf("%ld byte data (seq %d) received.\n", strlen(buff_rcv), seq);
        sendto(serverSocket, buff_snd, strlen(buff_snd) + 1, 0, // +1: NULL까지 포함해서 전송
               (struct sockaddr *)&client_addr, sizeof(client_addr));
        seq++;
    }
    close(serverSocket);
}