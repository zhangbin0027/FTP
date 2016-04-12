#include "../ftp.h"

int main(int argc, char* argv[])
{
    int num = 0;//计数器，记录链接的个数
    char ip[MIN_NUM] = {0};//存ip
    char port[MIN_NUM] = {0};//存port
    pinfo_t info;
    
    if(argc != 2)
    {
        printf("error args\n");
        exit(-1);
    }//对是否输入路径做判断
    
    init_from_configfile(argv[1], ip, port);
    
    
    int sfd = socket_server(ip, port);
    int cfd;
    
    
    
    if(sfd == -1)
    {
        perror("socket_server");
        exit(-1);
    }
    
    char log_num_buf[BUF_SIZE];
    int pid;
    while(1)
    {
        cfd = accept(sfd, NULL, NULL);
        if(cfd == -1)
        {
            perror("accept");
            exit(-1);
        }
        
        bzero(log_num_buf, BUF_SIZE);
        sprintf(log_num_buf, "%d is connected\n", ++num);
        write(1, log_num_buf, strlen(log_num_buf));
        
        info = (pinfo_t)calloc(1, sizeof(*info));
        info->info_sfd = cfd;
        pid = fork();
        if(pid == 0)//子进程
        {
            client_handle(info);
            exit(1);
        }
        free(info);
    }
}

