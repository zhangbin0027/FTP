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
#define BUF_SIZE 1024
#define MAX_CONNECT 10

//服务器初始化
extern int tcp_init(const char *ip, int port);


//服务端接收客户端的链接
extern int tcp_accept(int sfd);


//用于客户端的链接
extern int tcp_connect(const char* ip, int port);
#endif
