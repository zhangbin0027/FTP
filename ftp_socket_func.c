#include "ftp.h"

int tcp_init(const char *ip, int port)//用于初始化
{
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == sfd)
    {
        perror("socket");
        return -1;
    }
    struct sockaddr_in ser;
    bzero(&ser, sizeof(ser));
    ser.sin_family = AF_INET;
    ser.sin_port = htons(port);
    ser.sin_addr.s_addr = inet_addr(ip);

    int ret;
    ret = bind(sfd, (struct sockaddr*)&ser, sizeof(struct sockaddr));
    if(-1 == ret)
    {
         perror("bind");
         close(sfd);
         exit(-1);
    }

    ret = listen(sfd, MAX_CONNECT);
    if(-1 == ret)
    {
        perror("listen");
        close(sfd);
        exit(-1);
    }
    return sfd;
}



int tcp_accept(int sfd)//用于服务端的接收
{
    struct sockaddr_in client;
    bzero(&client, sizeof(struct sockaddr));
    int addrlen =sizeof(struct sockaddr);
    int new_fd = accept(sfd, (struct sockaddr*)&client, &addrlen);
    if(-1 == new_fd)
    {
        perror("accept");
        close(sfd);
        exit(-1);
    }
    printf("ip = %s, port = %d success connect!\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
    return new_fd;
}

//用于客户端的链接
int tcp_connect(const char* ip, int port)
{
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == sfd)
    {
        perror("socket");
        exit(-1);
    }
    
    struct sockaddr_in server;
    bzero(&server, sizeof(struct sockaddr));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(ip);
    
    if((connect(sfd, (struct sockaddr*)&server, sizeof(struct sockaddr))) == -1)
    {
        perror("connect");
        close(sfd);
        exit(-1);
    }
    return sfd;
}



