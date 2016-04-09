#include"server.h"

/*读配置文件<文件流/文件流的格式化输出/errno全局变量>
 * <ip地址/端口号/服务端接受报文的最大限制(报文队列长度)/创建线程的个数>
 * */
int conf_load(char* server_conf,char *addr,int* port,int* capacity,int*thread_num)
{
		FILE *conf_fp = fopen(server_conf,"r");
		if(NULL == conf_fp)
		{
				/* print a system error message*/
				perror("fopen(server.conf) failed\n");
				return -1;
		}
		if(fscanf(conf_fp,"%s\n",addr) == EOF)
		{
				perror("fscanf(addr) failed\n");
				return -1;
		}
		if(fscanf(conf_fp,"%d\n",port) == EOF)
		{
				perror("fscanf(port) failed\n");
				return -1;
		}
		if(fscanf(conf_fp,"%d\n",capacity)==EOF)
		{
				perror("fscanf(capacity) failed\n");
				return -1;
		}
		if(fscanf(conf_fp,"%d\n",thread_num)==EOF)
		{
				perror("fscanf(thread_num) failed\n");
				return -1;
		}
		if(fclose(conf_fp) == EOF )
		{
				perror("fclose(conf_fp) failed\n");
				return -1;
		}
		printf("server: addr:%s,port:%d,capacity:%d,thread_num:%d.\n",addr,
*port,*capacity,*thread_num);
		return 0;		
}

/*初始化服务端<socket套接字/TCPip/errno全局变量/internet operations>
 *<socket套接字(监听）/基于TCPip协议>
 * */
int server_ready(int* socket_fd,struct sockaddr_in* server_addr,char* addr,int port,int capacity)
{
		*socket_fd=socket(AF_INET,SOCK_STREAM,0);
		if(*socket_fd == -1)
        {
				perror("socket() failed\n");
				return -1;
		}
		server_addr->sin_family=AF_INET;

		/*internet operations*/
		server_addr->sin_port=htons(port);
		server_addr->sin_addr.s_addr=inet_addr(addr);

		if(bind(*socket_fd,(struct sockaddr*)server_addr,sizeof(struct sockaddr))==-1)
		{
				perror("bind() failed\n");
				return -1;
		}
		if(listen(*socket_fd,capacity)==-1)
		{
				perror("listen() failed\n");
				return -1;
		}
		printf("server_ready() successed\n");
		return 0;
}
