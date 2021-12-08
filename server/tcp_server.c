#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

/* 服务器的端口号 */
#define SERVER_PORT 12356
/* 服务器的IP地址 */
#define SERVER_IP "192.168.3.17"
/* 服务器允许的最大客户端同时连接数量 */
#define CONNCUM 10
/* 服务器存放日志的位置 */
#define LOG_FILE "/home/tiger/tmpfile/server_log.txt"

/**
 * @function: 获取本地时间，将本地时间用通用格式呈现，并传给入参
 * @logcaltime: 传入的一个字符串，用来存储本地时间
 * @count: logcaltime字符串的最大长度
 */
void get_localtime(char *localtime_string, int count)
{
    time_t tmp_time;
    struct tm *tmp_ptr = NULL;

    time(&tmp_time);
    tmp_ptr = localtime(&tmp_time);
    snprintf(localtime_string, count, "[%d.%d.%d %d:%d:%02d 星期%d]", 
            tmp_ptr->tm_year + 1900, tmp_ptr->tm_mon + 1, tmp_ptr->tm_mday, tmp_ptr->tm_hour, 
            tmp_ptr->tm_min, tmp_ptr->tm_sec, tmp_ptr->tm_wday);
}

int main(int argc, char *argv[])
{
    int sockfd, new_fd;
    struct sockaddr_in server_addr, client_addr;
    int ret;
    int sin_size;
    char hello[] = "I'm One Server\n";
    char log_string[2048];
    int filefd;
    char localtime_string[1024];

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    /* 绑定服务器上的所有IP地址 */
//    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    /* 绑定服务器上某个特定的IP地址 */
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(SERVER_PORT);

    /* AF_INET表示IPv4, AF_INET6表示IPv6, SOCK_STREAM表示TCP, SOCK_DGRAM表示UDP */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Socket create error\n");
        return -1;
    }

    ret = bind(sockfd, (struct sockaddr *)(&server_addr), sizeof(server_addr));
    if (ret == -1) {
        printf("Socket bind error\n");
        close(sockfd);
        return -1;
    }

    ret = listen(sockfd, CONNCUM);
    if (ret == -1) {
        printf("Socket listen error\n");
        close(sockfd);
        return -1;
    }

    filefd = open(LOG_FILE, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (filefd == -1) {
        printf("open %s file error\n", LOG_FILE);
        close(sockfd);
        return -1;
    }

    while (1) {
        sin_size = sizeof(struct sockaddr_in);
        new_fd = accept(sockfd, (struct sockaddr *)(&client_addr), &sin_size);
        if (new_fd == -1) {
            printf("Socket accept error\n");
            close(sockfd);
            close(filefd);
            return -1;
        }

        get_localtime(localtime_string, 1024);
        /* 使用snprintf保证安全性，记录日志到字符串，后续可将字符串打印或写到文件 */
        snprintf(log_string, sizeof(log_string), "%stiger server get connection from %s\n", 
                localtime_string, inet_ntoa(client_addr.sin_addr));

        printf("%s", log_string);

        ret = write(filefd, log_string, strlen(log_string));

        ret = write(new_fd, hello, strlen(hello));
        if (ret == -1) {
            printf("Socket write error\n");
            close(sockfd);
            close(new_fd);
            return -1;
        }

        close(new_fd);
    }

    close(sockfd);
    close(filefd);

    return 0;
}

