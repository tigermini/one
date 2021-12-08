#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

/* 服务器的端口号 */
#define SERVER_PORT 12356
/* 服务器的IP地址 */
#define SERVER_IP "192.168.3.17"

int main(int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_in server_addr;
    int ret;
    char server_buffer[1024];
    int serverbytes;

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Socket create error\n");
        return -1;
    }

    ret = connect(sockfd, (struct sockaddr *)(&server_addr), sizeof(server_addr));
    if (ret == -1) {
        printf("Socket connect error\n");
        close(sockfd);
        return -1;
    }

    serverbytes = read(sockfd, server_buffer, 1024);
    if (serverbytes == -1) {
        printf("Socket read error\n");
        close(sockfd);
        return -1;
    }
    server_buffer[serverbytes] = '\0';
    printf("server buffer is %s\n", server_buffer);

    close(sockfd);

    return 0;
}

