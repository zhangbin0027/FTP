#include "../ftp.h"

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("Usage:need ip, port\n");
        exit(-1);
    }
    
    char *ip_server = argv[1];
    char *port_server = argv[2];
    char recv_buf[BUF_SIZE], send_buf[BUF_SIZE];
    int recv_len, send_len, read_len, write_len;
    
    int cfd = socket_client(ip_server, port_server);
    if(-1 == cfd)
    {
        perror("socket_client");
        exit(-1);
    }
    
    char buf[BUF_SIZE];
    while(bzero(buf, BUF_SIZE), (read_len = read(0, buf, BUF_SIZE)) > 0)
    {
        delete_space(buf);
        send_len = strlen(buf);
        send(cfd, &send_len, sizeof(int), 0);
        send_n(cfd, buf, send_len);
        if(strncpy("cd", buf, 2) == 0)
        {
            recv(cfd, &recv_len, sizeof(int), 0);
            recv_n(cfd, recv_buf, recv_len);
            printf("%s\n", recv_buf);
        }else{
            continue;
        }
    }
    return 0;
}


















































