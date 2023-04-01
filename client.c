#include "header.h"

int main()
{
    int result;
    MSG_BUFF msg_buff;
    STAFF staff_data;
    result = initSocketConnect(SOCKET_TYPE_CLIENT);

    if (result == -1)
    {
        perror("无法连接到服务端");
        exit(1);
    }

    while (1)
    {

        printf("1.添加员工\n");
        printf("2.显示员工列表\n");
        printf("0.退出本程序\n");
        printf("选择您要进行的操作：");
        scanf("%d", &msg_buff.type);
        switch (msg_buff.type)
        {
        case 0:
            close(clientFileId);
            exit(0);
            break;
        case STATUS_ADD:
            printf("输入员工姓名：");
            scanf("%s", staff_data.name);
            printf("输入员工年龄:");
            scanf("%d", &staff_data.age);
            printf("输入员工电话:");
            scanf("%s",staff_data.phone);
            msg_buff.data = staff_data;
            result = socket_IO(clientFileId, &msg_buff, 1);
            break;
        case STATUS_READS:
            result = socket_IO(clientFileId, &msg_buff, 1);

            break;

        default:
            printf("请输入正确到命令！\n");
            continue;
        }

        if (result == -1)
        {
            perror("发送失败，请重新启动客户端！");
            exit(1);
        }
        if (!((result = socket_IO(clientFileId, &msg_buff, 0)) > 0))
        {
            perror("接受消息失败，请重新启动客户端！");
            exit(1);
        }
        switch (msg_buff.type)
        {
        case STATUS_ADD:
            printf("添加成员成功！\n");
            break;
        case STATUS_START:
            printf("成员列表：\n");
            printf("%-10s\t%-10s\t%-10s\n", "姓名", "年龄", "电话");
            while (socket_IO(clientFileId, &msg_buff,0))
            {
                if (msg_buff.type != STATUS_READS)
                {
                    break;
                }
                staff_data = msg_buff.data;
                printf("%-10s\t", staff_data.name);
                printf("%-10d\t", staff_data.age);
                printf("%-11s\n", staff_data.phone);
            }
            printf("成员列表已发送完毕！\n");
            break;
        default:
            break;
        }
    }

    close(clientFileId);
}