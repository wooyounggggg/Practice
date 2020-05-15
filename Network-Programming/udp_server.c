#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define BUFF_SIZE 1024
int main(void)
{
    int serverSocket;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int bindState;
    char buf[BUFF_SIZE];
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(4000);
    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    bindState = bind(serverSocket, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (bindState == -1)
    {
        perror("execution error\n");
        exit(1);
    }

    return 0;
}