#include "../ftp.h"


int socket_client(char *ip, char *port)
{
    int cfd;
    struct sockaddr_in client;
    cfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == cfd)
    {
        perror("socket");
        exit(-1);
    }
    
    bzero(&client, sizeof(client));
    client.sin_family = AF_INET;
    client.sin_port = htons(atoi(port));
    client.sin_addr.s_addr = inet_addr(ip);
    
    int connect_num = BUF_SIZE;
    int n = 1;
    
    //广播数据是否可以从套接口发送
    if(0 != setsockopt(cfd, SOL_SOCKET, SO_REUSEADDR,  (void*)&n, sizeof(int)))
    {
        close(cfd);
        exit(-1);
    }
    
    if(0 != setsockopt(cfd, SOL_SOCKET, SO_SNDBUF, (void*)&connect_num, sizeof(int)))
    {
        close(cfd);
        exit(-1);
    }
    
    if(0 != setsockopt(cfd, SOL_SOCKET, SO_RCVBUF,  (void*)&connect_num, sizeof(int)))
	{
		close(cfd);
		return -1 ;	
	}
	if(-1 == connect(cfd, (struct sockaddr*)&client, sizeof(struct sockaddr)))
	{
	    perror("connect");
		close(cfd);
		return -1 ;
	}
	return cfd;
}


void delete_space(char* src)
{
	int index, cur ;
	for(cur = -1, index = 0 ; index < strlen(src); index ++)
	{
		if(src[index] !=' ' && src[index] != '\n')
		{
			src[++cur] = src[index];
			
		}else 
		{
			if(cur != -1 && src[cur] != ' '&& src[cur] !='\n' )
			{
				src[++cur] = ' ';
			}
		}

	}
	for(; cur >= 0 ; cur --)
	{
		if(src[cur] != ' ')
		{
			break ;
		}
	}
	src[++cur] = '\0';
}


int send_n(int fd_send, char* send_buf, int len)
{
    int sum = 0;
    int nsend;
    while(sum < len)
    {
        nsend = send(fd_send, send_buf + sum, len - sum, 0);
        sum += nsend;
    }
    return sum;
}

int write_n(int fd_write, char* wirte_buf, int len)
{
    int sum = 0;
    int nwrite;
    while(sum < len)
    {
        nwrite = write(fd_write, wirte_buf+sum, len-sum);
        sum += nwrite;
    }
    return sum;
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

