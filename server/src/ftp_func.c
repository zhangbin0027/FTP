#include "../include/ftp.h"
socket_t socket_server(char* ip, char* port)
{
	int fd_server ;
	struct sockaddr_in server_addr ;
	fd_server = socket(AF_INET, SOCK_STREAM, 0);
	if(fd_server == -1)
	{
		return -1;
	}
	memset(&server_addr, 0, sizeof(server_addr) );
	server_addr.sin_family = AF_INET ;
	server_addr.sin_port = htons(atoi(port));
	server_addr.sin_addr.s_addr = inet_addr(ip);
	int reuse = 1 ;
	int buf_num = BUF_SIZE;
	if(0 != setsockopt(fd_server, SOL_SOCKET, SO_REUSEADDR,  (void*)&reuse, sizeof(int)) )
	{
		close(fd_server);
		return -1;
	}
	if(0 != setsockopt(fd_server, SOL_SOCKET, SO_SNDBUF,  (void*)&buf_num, sizeof(int)) )
	{
		close(fd_server);
		return -1 ;	
	}
	if(0 != setsockopt(fd_server, SOL_SOCKET, SO_RCVBUF,  (void*)&buf_num, sizeof(int)) )
	{
		close(fd_server);
		return -1 ;	
	}

	if(-1 == bind(fd_server, (struct sockaddr*)&server_addr, sizeof(server_addr) ) )
	{
		close(fd_server);
		return -1 ;	
	}
	if(-1 == listen(fd_server, 5))
	{
		close(fd_server);
		return -1 ;
	}
	return fd_server ;

}


socket_t socket_client(char* ip, char* port)
{
	int fd_client ;
	struct sockaddr_in server_addr ;
	fd_client = socket(AF_INET, SOCK_STREAM, 0);
	if(fd_client == -1)
	{
		return -1;
	}
	memset(&server_addr, 0, sizeof(server_addr) );
	server_addr.sin_family = AF_INET ;
	server_addr.sin_port = htons(atoi(port));
	server_addr.sin_addr.s_addr = inet_addr(ip);
	int reuse = 1 ;
	int buf_num = BUF_SIZE;
	if(0 != setsockopt(fd_client, SOL_SOCKET, SO_REUSEADDR,  (void*)&reuse, sizeof(int)) )
	{
		close(fd_client);
		return -1;
	}
	if(0 != setsockopt(fd_client, SOL_SOCKET, SO_SNDBUF,  (void*)&buf_num, sizeof(int)) )
	{
		close(fd_client);
		return -1 ;	
	}
	if(0 != setsockopt(fd_client, SOL_SOCKET, SO_RCVBUF,  (void*)&buf_num, sizeof(int)) )
	{
		close(fd_client);
		return -1 ;	
	}
	if(-1 == connect(fd_client, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)))
	{
		close(fd_client);
		return -1 ;
	}
	return fd_client ;

}

int recvn(socket_t fd_recv,char* recv_buf, int len)
{
	int sum = 0 ;
	int nrecv ;
	while(sum < len)
	{
		nrecv = recv(fd_recv, &recv_buf[ sum], len - sum, 0);
		sum += nrecv ;
	}
	recv_buf[sum] = '\0';
	return sum ;
}

int readn(int fd_read, char* read_buf, int len)
{
	int sum = 0 ;
	int nread ;
	while(sum < len)
	{
		nread = read(fd_read, &read_buf[ sum], len - sum);
		if(nread == 0)
		{
			break ;
		}
		sum += nread ;
	}
	read_buf[sum] = '\0';
	return sum ;
		
}
int sendn(socket_t fd_send, char* send_buf, int len)
{
	int sum = 0 ;
	int nsend ;
	while(sum < len)
	{
		nsend = send(fd_send, send_buf + sum, len - sum, 0);
		sum += nsend ;
	}
	return sum ;
}


int writen(int fd_write, char* write_buf, int len)
{
	int sum = 0 ;
	int nwrite ;
	while(sum < len)
	{
		nwrite = write(fd_write, write_buf + sum, len - sum);
		sum += nwrite ;
	}
	return sum ;

}


void trim_space(char* src)
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
int upload(socket_t fd_up, char* file_name)
{
	int fd_file = open(file_name, O_RDONLY);
	if(fd_file == -1)
	{
		return -1 ;
	}
	char *read_buf = (char*)malloc(8 * 1024);
	bzero(read_buf, 8 * 1024);
	int nread ;
	while(1)
	{
		nread = readn(fd_file, read_buf, 8192);
		if(nread < 8192)
		{
			send(fd_up, &nread, sizeof(int), 0);
			sendn(fd_up, read_buf, nread);
			break ;
		}else
		{
			
			send(fd_up, &nread, sizeof(int), 0);
			sendn(fd_up, read_buf, nread);
		}
	}
	int flag = 0 ;
	send(fd_up, &flag, sizeof(flag), 0);
	close(fd_file);
	return 0 ;
}
int download(socket_t fd_down, char* file_name)
{
	int fd_file = open(file_name, O_WRONLY|O_CREAT,0666 );
	if(fd_file == -1)
	{
		return -1 ;
	}
	char* write_buf = (char*)malloc(8192);
	bzero(write_buf, 8192);
	int nwrite ;
	while(1)
	{
		recv(fd_down, &nwrite, sizeof(int), 0);
		if(nwrite == 0)
		{
			break ;
		}
		recvn(fd_down, write_buf, nwrite);
		writen(fd_file, write_buf, nwrite);
	}
	close(fd_file);
	return 0 ;
}
















static char* file_type(mode_t md)
{
	if(S_ISREG(md))
	{
		return "-";	
	}else if(S_ISDIR(md))
	{
		return "d";
	}else if(S_ISFIFO(md))
	{
		return "p";
	}else 
	{
		return "o" ;
	}
}


static void do_ls(psession_t ps) 
{
	DIR* pdir = opendir("./");
	if(pdir == NULL)
	{
		int flag = -1 ;
		send(ps ->sess_sfd, &flag, sizeof(int), 0);
	}else
	{
		struct dirent* mydir ;
		int len ;
		while( (mydir = readdir(pdir)) != NULL)
		{
			if(strncmp(mydir->d_name, ".", 1) == 0 || strncmp(mydir->d_name,"..", 2) == 0)
			{
				continue ;
			}
			struct stat mystat;
			bzero(&mystat, sizeof(stat));
			stat(mydir->d_name, &mystat);
			bzero(ps ->sess_buf, 1024);
			sprintf(ps ->sess_buf, "%-2s %-20s %10dB", file_type(mystat.st_mode),mydir->d_name, mystat.st_size );
			len =  strlen(ps ->sess_buf);
			send(ps ->sess_sfd, &len, sizeof(int), 0);
			sendn(ps ->sess_sfd, ps ->sess_buf, len);
		}
		len = 0 ;
		send(ps ->sess_sfd, &len, sizeof(int), 0);
	}

}

static void do_cd(psession_t ps) 
{
	char dir[128]= "";
	sscanf(ps ->sess_buf +3, "%s", dir);
	chdir(dir);
	getcwd(dir, 128);
	int len = strlen(dir);
	send(ps ->sess_sfd, &len, sizeof(int), 0);
	sendn(ps ->sess_sfd, dir, strlen(dir));
}
static void do_pwd(psession_t ps) 
{
	bzero(ps ->sess_buf, BUF_SIZE);
	getcwd(ps ->sess_buf, BUF_SIZE);
	int len = strlen(ps ->sess_buf);
	send(ps ->sess_sfd, &len, sizeof(int), 0);
	sendn(ps ->sess_sfd, ps ->sess_buf, len);

}
static void do_gets(psession_t ps) 
{
	char file_name[256];
	int file_pos = 5 ;
	while(bzero(file_name, 256),sscanf(ps ->sess_buf + file_pos,"%s", file_name ) == 1)
	{
		file_pos += strlen(file_name) + 1; 
		if(upload(ps ->sess_sfd, file_name) == 0)
		{
			printf(" file-> %s upload success\n", file_name);
		}else 
		{
			printf(" file-> %s upload failed\n", file_name);
		}

	}

}
static void do_puts(psession_t ps) //puts a.txt b.txt
{
	char file_name[256];
	int file_pos = 5 ;
	while(bzero(file_name, 256), sscanf(ps ->sess_buf + file_pos,"%s", file_name) == 1)
	{
		file_pos += strlen(file_name) + 1 ;
		if(download(ps -> sess_sfd, file_name) == 0)
		{
			printf("file -> %s download success \n", file_name);
		}else
		{

			printf("file -> %s download failed \n", file_name);
		}
	}
}
static void do_remove(psession_t ps)// remove file 
{
	char cmd[256] ="" ;
	sprintf(cmd, "rm -f %s", ps ->sess_buf + 7);
	system(cmd);
	bzero(ps ->sess_buf, BUF_SIZE);
	sprintf(ps ->sess_buf, "%s removed", ps ->sess_buf + 7);
	int len = strlen(ps ->sess_buf);
	send(ps ->sess_sfd, &len, sizeof(int),0);
	sendn(ps ->sess_sfd, ps ->sess_buf, len);
}
void client_handle(psession_t ps)
{

	socket_t fd_client = ps -> sess_sfd ;
	int cmd_len = 0 ;
	int recv_ret ;
	while(1)
	{
		bzero(ps -> sess_buf, BUF_SIZE);
		recv_ret = recv(fd_client, &cmd_len, sizeof(int),0);
		if(cmd_len == 0 || recv_ret == 0)
		{
			printf("client exit !\n");
			close(ps ->sess_sfd);
			free(ps);
			exit(1);
		}
		recvn(fd_client, ps->sess_buf, cmd_len);
		if(strncmp("cd", ps ->sess_buf, 2) == 0)
		{
			do_cd(ps);
		}else if(strncmp("ls", ps ->sess_buf, 2) == 0)
		{
			do_ls(ps);
		}else if( strncmp("puts", ps ->sess_buf, 4)== 0)
		{
			do_puts(ps);
		}else if( strncmp("gets", ps ->sess_buf, 4)== 0)
		{
			do_gets(ps);

		}else if( strncmp("remove", ps ->sess_buf, 6)== 0)
		{
			do_remove(ps);

		}else if(strncmp("pwd", ps ->sess_buf, 3) == 0) 
		{
			do_pwd(ps);

		}else 
		{
			continue ;
		}


	}
}
















static void ftp_arg_set(char* line, char* arg) 
{	
	char* ptr = strchr(line, '=');
	if(ptr == NULL)
	{
		ERR_EXIT("no =");
	}
	strcpy(arg, ptr + 1);
}


void ftp_init_from_file(char* path, char* ip, char* port)
{
	FILE* fp_conf = fopen(path, "r");
	if(fp_conf == NULL)
	{
		ERR_EXIT("open conf file fail!");	
	}
	char line[128];

	memset(line, 0, 128);
	fgets(line, 128, fp_conf) ;
	line[strlen(line) - 1] = '\0' ;
	ftp_arg_set(line, ip);

	memset(line, 0, 128);
	fgets(line, 128, fp_conf) ;
	line[strlen(line) - 1] = '\0' ;
	ftp_arg_set(line, port);
	fclose(fp_conf);
}



