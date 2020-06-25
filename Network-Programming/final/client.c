#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <sys/ioctl.h>
#define BUFF_SIZE 1024
#define SERVER_IP "127.0.0.1"
int i = 0;
int main(void)
{
    int client_socket;
    struct sockaddr_in server_addr;
    char msg[BUFF_SIZE] = "";
    char buff[BUFF_SIZE + 5];
    char serverIP[20];
    client_socket = socket(PF_INET, SOCK_STREAM, 0);
    struct ifreq ifr;
    char ipstr[40];
    int s;

    s = socket(AF_INET, SOCK_STREAM, 0);
    strncpy(ifr.ifr_name, "enp0s3", IFNAMSIZ);

    if (ioctl(s, SIOCGIFADDR, &ifr) < 0)
        printf("Error");
    else
        inet_ntop(AF_INET, ifr.ifr_addr.sa_data + 2,
                  ipstr, sizeof(struct sockaddr));
    if (client_socket == -1)
    {
        printf("socket 생성 실패\n");
        exit(1);
    }
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(4000);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        printf("접속 실패\n");
        exit(1);
    }
    strcpy(serverIP, inet_ntoa(server_addr.sin_addr));
    while (strcmp(msg, "bye") != 0)
    {
        printf("[%s->send] ", ipstr);
        scanf("%s", msg);
        write(client_socket, msg, strlen(msg) + 1); // +1: NULL까지 포함해서 전송
        read(client_socket, buff, BUFF_SIZE + 1);   /*  */
        if (strcmp(msg, "bye") != 0)
            printf("[%s->recv] %s\n", SERVER_IP, buff);
        if (strcmp(buff, "bye") == 0)
        {
            close(client_socket);
            close(s);
            return 0;
        }
    }
    close(client_socket);
    close(s);
    return 0;
}
