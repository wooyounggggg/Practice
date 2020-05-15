#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#define BUFF_SIZE 4096

typedef struct Packet
{
    char data[4096];
    int data_size;
    int data_seq;
} Packet;
int main(int argc, char **argv)
{
    int rd_size;
    int fd;
    int serverSocket;
    int server_addr_size;
    struct sockaddr_in server_addr;
    Packet packet;
    packet.data_seq = 1;
    char buff_rcv[BUFF_SIZE + 1];
    char buff_send[BUFF_SIZE + 1];
    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == serverSocket)
    {
        printf("socket 생성 실패n");
        exit(1);
    }
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(4000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (0 < (fd = open("./input.txt", O_RDONLY)))
    {
        while (0 < (packet.data_size = read(fd, packet.data, BUFF_SIZE))) //  4 byte씩
        {
            packet.data[packet.data_size] = '\0';
            sendto(serverSocket, packet.data, strlen(packet.data) + 1, 0, // +1: NULL까지 포함해서 전송
                   (struct sockaddr *)&server_addr, sizeof(server_addr));
            printf("%d bytes data(seq %d) send\n", packet.data_size, packet.data_seq);
            packet.data_seq++;
        }
        close(fd);
    }
    else
    {
        printf("파일 열기에 실패했습니다.\n");
        return 0;
    }

    /* sendto(serverSocket, "0x1A", strlen("0x1A") + 1, 0, // +1: NULL까지 포함해서 전송
           (struct sockaddr *)&server_addr, sizeof(server_addr)); */
    close(serverSocket);

    return 0;
}