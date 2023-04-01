#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <pthread.h>

#define SOCKET_PORT 15246
#define SOCKET_ADDR "127.0.0.1"
#define STATUS_ADD 1
#define STATUS_READS 2
#define STATUS_FINISHI 3
#define STATUS_START 4

typedef struct
{
    char name[10];
    int age;
    char phone[11];
} STAFF;

typedef struct
{
    int type;
    STAFF data;
} MSG_BUFF;
typedef unsigned short int socket_type;
// 服务端
const static socket_type SOCKET_TYPE_SERVER = 1;
// 客户端
const static socket_type SOCKET_TYPE_CLIENT = 2;
int clientFileId;
int serverFileId;

int initSocketConnect(socket_type type)
{
    struct in_addr ia;
    struct sockaddr_in si;
    if (!(type == SOCKET_TYPE_CLIENT | SOCKET_TYPE_SERVER))
    {
        return -1;
    }
    si.sin_family = AF_INET;
    si.sin_port = htons(SOCKET_PORT);
    inet_aton(SOCKET_ADDR, &si.sin_addr);
    
    if (type == SOCKET_TYPE_CLIENT)
    {
        // 初始化客户端
        printf("初始化客户端");
        clientFileId = socket(AF_INET, SOCK_STREAM, 0);
        return connect(clientFileId, (struct sockaddr *)&si, sizeof(struct sockaddr_in));
    }
    else if (type == SOCKET_TYPE_SERVER)
    {
        // 初始化服务端
        printf("初始化服务端\n");
        serverFileId = socket(AF_INET, SOCK_STREAM, 0);
        bind(serverFileId, (struct sockaddr *)&si, sizeof(struct sockaddr_in));
        return listen(serverFileId, 10);
    }
    return -1;
}

/**
 * socket发送消息
 * _buff = 消息内容
 * _dataLen = data的长度
 * _send = 0读取，非零发送
 */
int socket_IO(int fd,MSG_BUFF *_buff, int _send)
{
    int r;
    size_t len = sizeof(MSG_BUFF);
    if (_send)
    {
        r = send(fd, (void *)_buff, len, 0);
        if (r == -1)
        {
            perror("发送消息失败");
        }
    }
    else
    {
        r = recv(fd, (void *)_buff, len, 0);
    }
    return r;
}