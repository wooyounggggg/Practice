#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#define BUFF_SIZE 1024

int main(void)
{
    int server_socket = 0; /* for server socket fd allocation */
    int client_socket = 0; /* for client socket fd allocation */
    int client_addr_size = sizeof(client_addr);

    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    char buff[BUFF_SIZE + 5]; /*  */
    char buff_rcv[BUFF_SIZE];
    char buff_snd[BUFF_SIZE];

    memset(&server_addr, 0x00, sizeof(server_addr));
    memset(&client_addr, 0x00, sizeof(client_addr));

    server_socket = socket(PF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        printf("생성 실패\n");
        exit(1);
    }
    server_addr.sin_family = AF_INET;                /* IPv4 Protocol */
    server_addr.sin_port = htons(4000);              /* port 4000 */
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); /*  */
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        printf("bind() error\n");
        exit(1);
    }
    if (listen(server_socket, 5) == -1)
    {
        printf("listen error\n");
        exit(1)
    }
    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_size);
    if (client_socket == -1)
    {
        printf("client connection error\n");
        exit(1);
    }

    return 0;
}