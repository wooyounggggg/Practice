#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUFF_SIZE 1024

int main(int argc, char **argv)
{
    int sock;
    int server_addr_size;
    struct sockaddr_in server_addr;
    char buff_rcv[BUFF_SIZE + 5];
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == sock)
    {
        printf("socket 생성 실패n");
        exit(1);
    }
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(4000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    sendto(sock, argv[1], strlen(argv[1]) + 1, 0, // +1: NULL까지 포함해서 전송
           (struct sockaddr *)&server_addr, sizeof(server_addr));

    server_addr_size = sizeof(server_addr);
    recvfrom(sock, buff_rcv, BUFF_SIZE, 0,
             (struct sockaddr *)&server_addr, &server_addr_size);
    printf("receive: %sn", buff_rcv);
    close(sock);

    return 0;
}