#ifndef __FTP_H__
#define __FTP_H__
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/select.h>
#include<fcntl.h>
#include<sys/time.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<dirent.h>
#include<sys/stat.h>
#include<signal.h>
#include<string.h>
#include<strings.h>
#include<sys/wait.h>
#include<pthread.h>
#define BUF_SIZE 1024
#define MIN_NUM 30
#define FILE_MAX_LEN 64

typedef struct information
{
    int info_sfd;
    char info_buf[BUF_SIZE];
}info_t, *pinfo_t;


//server

//从文件获取ip和port
void init_from_configfile(char *path, char *ip, char *port);

//服务端初始化到监听函数为止
int socket_server(char *ip, char *port);

//服务端接收客户端信息
int recv_n(int fd_recv,char *recv_buf, int len);

//服务端接收来自客户端的命令处理函数
void client_handle(pinfo_t info);





//client
//客户端初始化函数
int socket_client(char *ip, char *port);

//处理函数
void delete_space(char* src);

int write_n(int fd_write, char* wirte_buf, int len);//写

int send_n(int fd_send, char* send_buf, int len);//发

#endif
