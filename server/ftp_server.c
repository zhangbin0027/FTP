#include "../ftp.h"

int main(int argc, char* argv[])//the path of server.conf
{
    if(argc != 3)
    {
        printf("Usage:path, ip, port\n");
        exit(-1);
    }
    
    int sfd = tcp_init(argv[1], atoi(argv[2]));
    char buf[BUF_SIZE] = {0};
    while(1)
    {
        int cfd = tcp_accept(sfd);
        if(fork() == 0)
        {
            recv(cfd, buf, sizeof(buf), 0);
            puts(buf);
            send(cfd, "hello", 6, 0);
            close(cfd);
        }else{
            close(cfd);
        }
    }
    close(sfd);
}
