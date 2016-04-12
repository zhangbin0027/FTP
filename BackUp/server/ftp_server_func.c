#include "../ftp.h"

void init_from_configfile(char *path, char *ip, char *port)
{
    FILE* fp = fopen(path, "r");
    if(fp == NULL)
    {
        printf("open configfile failed!\n");
        exit(-1);
    }
    
    char buf[BUF_SIZE] = {0};
    fgets(buf, BUF_SIZE, fp);
    buf[strlen(buf) - 1] = '\0';
    
    //ip
    char *ip_ptr = strchr(buf, '=');
    if(ip_ptr == NULL)
    {
        printf("error config file without '='\n");
        exit(-1);
    }
    
    strcpy(ip, ip_ptr + 1);
    //ip complete
    
    
    //port
    memset(buf, BUF_SIZE, 0);
    fgets(buf, BUF_SIZE, fp);
    buf[strlen(buf) - 1] = '\0';
    char *port_ptr = strchr(buf, '=');
    if(port_ptr == NULL)
    {
        printf("error config file without '='\n");
        exit(-1);
    }
    strcpy(port, port_ptr + 1);
    //port complete
    
    fclose(fp);
}


int socket_server(char *ip, char *port)
{
    int sfd;
    struct sockaddr_in server;
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == sfd)
    {
        perror("socket");
        exit(-1);
    }
    
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(port));
    server.sin_addr.s_addr = inet_addr(ip);
    
    int connect_num = BUF_SIZE;
    int n = 1;
    //广播数据是否可以从套接口发送
    if(0 != setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (void*)&n, sizeof(int)))
    {
        close(sfd);
        exit(-1);
    }
    
    if(0 != setsockopt(sfd, SOL_SOCKET, SO_SNDBUF, (void*)&connect_num, sizeof(int)))
    {
        close(sfd);
        exit(-1);
    }
    
    int ret;
    ret = bind(sfd, (struct sockaddr*)&server, sizeof(server));
    if(-1 == ret)
    {
        perror("bind");
        close(sfd);
        exit(-1);
    }
    
    ret = listen(sfd, 5);
    if(-1 == ret)
    {
        perror("listen");
        close(sfd);
        exit(-1);
    }
    
    return sfd;
}

int recv_n(int fd_recv,char *recv_buf, int len)
{
    int sum = 0;
    int nrecv;
    while(sum < len)
    {
        nrecv = recv(fd_recv, &recv_buf[sum], len - sum, 0);
        sum += nrecv;
    }
    recv_buf[sum] = '\0';
    return sum;
}


void client_handle(pinfo_t info)
{
    int cfd = info->info_sfd;
    int len = 0;
    int recv_ret;
    while(1)
    {
        bzero(info->info_buf, BUF_SIZE);
        recv_ret = recv(cfd, &len, sizeof(int), 0);
        if(len == 0 || recv_ret == 0)
        {
            printf("client exit\n");
            close(info->info_sfd);
            free(info);
            exit(1);
        }
        
        recv_n(cfd, info->info_buf, len);
        if(strncmp("cd", info->info_buf, 2) == 0)
        {
            printf("I am cd\n");
        }else{
            continue;
        }
    }
}




















