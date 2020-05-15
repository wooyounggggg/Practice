#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define BUFF_SIZE 1024
typedef struct Packet
{
    char data[4096];
    int data_size;
    int data_seq;
} Packet;
int main(void)
{
    int client_socket;
    return 0;
}