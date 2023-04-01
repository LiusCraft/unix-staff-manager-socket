#include "header.h"
int loadDB()
{
    int dbFd;
    if ((dbFd = open("emp.db", O_RDWR | O_CREAT | O_APPEND, 0744)) == -1)
    {
        printf("打开db失败\n");
        exit(1);
    }
    return dbFd;
}

void *client_handler(void *args)
{
    int clientId = *(int *)args;
    MSG_BUFF msg_buff;
    STAFF staff_data;
    int dbFile;
    printf("创建成功%d\n", clientId);
    while (socket_IO(clientId, &msg_buff, 0) > 0)
    {
        switch (msg_buff.type)
        {
        case STATUS_ADD:
            dbFile = loadDB();
            printf("接受到了\n");
            staff_data = msg_buff.data;
            printf("添加成员：\n");
            printf("姓名:%s\t", staff_data.name);
            printf("年龄:%d\t", staff_data.age);
            printf("手机号:%s\n", staff_data.phone);
            write(dbFile, &staff_data, sizeof(STAFF));
            close(dbFile);
            socket_IO(clientId, &msg_buff, 1);
            break;
        case STATUS_READS:
            dbFile = loadDB();
            msg_buff.type = STATUS_START;
            socket_IO(clientId, &msg_buff, 1);
            while (read(dbFile, &staff_data, sizeof(STAFF)) > 0)
            {
                msg_buff.type = STATUS_READS;
                msg_buff.data = staff_data;
                socket_IO(clientId, &msg_buff, 1);
            }
            close(dbFile);
            msg_buff.type = STATUS_FINISHI;
            socket_IO(clientId, &msg_buff, 1);
            break;
        default:
            break;
        }
    }
    printf("断开了连接\n");
}

void main()
{
    pthread_t pt_client_handler[10];
    int clientCount = 0;

    if (initSocketConnect(SOCKET_TYPE_SERVER) == -1)
    {
        perror("创建服务端失败");
        exit(1);
    }
    printf("1shuchu\n");
    while (1)
    {
        struct sockaddr_in client_si;
        socklen_t cliaddr_len;
        cliaddr_len = sizeof(struct sockaddr_in);
        int clientId = accept(serverFileId, (struct sockaddr *)&client_si, &cliaddr_len);
        if (clientId < 1)
        {
            close(serverFileId);
            exit(1);
        }
        if (pthread_create(&pt_client_handler[clientCount], NULL, client_handler, (void *)&clientId) != 0)
        {
            perror("客户端处理线程创建错误...\n");
            printf("客户端：%s:%d\n", inet_ntoa(client_si.sin_addr), client_si.sin_port);
            close(clientId);
        }
        printf("tt\n");
        clientCount++;
    }
}