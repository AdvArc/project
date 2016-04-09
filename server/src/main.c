#include"server.h"
#include"packet.h"
#include"que.h"
#include"pool.h"
#include"control.h"
void* thread_handle(void* arg);
void server_do(node* n);
int choice(node* n,packet* pk,int* running,char *filedir);
int main(int argc,char* argv[])
{
		if(2 != argc)
		{
				printf("no server.conf\n");
				return -1;
		}
		char oridir[128] = {0};
		chdir("../file");

		/*Get current working directory*/
		/*保存原始路径*/
		getcwd(oridir,128);

		/*--------------服务端初始化阶段--------------------
		 *<读配置文件/启动线程池/启动报文接受队列/服务端就绪>
		 * */
		char server_conf[32];
		char addr[15];
		int port;
		int cap;
		int num;
		strcpy(server_conf/*out*/,argv[1]);
		/*读配置文件
		 *加载服务器启动的配置项
		 *即<ip地址/端口号/服务端接受报文的最大限制(报文队列长度)/创建线程的个数>   
		 */
		if(conf_load(server_conf,addr,&port,&cap,&num)==-1)
		{
				printf("conf_load() failed\n");
				return -1;
		}

		/*初始化化线程池（初始化报文接受队列)
		 *线程池初始化<条件变量/线程池大小(堆空间申请内存)/线程体的执行函数/线程个数/线程池启动状态/调用报文队列的初始化API>
		 *报文接受队列初始化<头指针、尾指针、线程锁、当前元素个数、最大元素个数>
		 * */
		pool p;
		pool_init(&p,num,cap,thread_handle);
		pool_start(&p);

		/*服务器就绪
		 * <socket套接字(监听）/基于TCPip协议>    
		 * */
		int socket_fd;
		struct sockaddr_in server_addr;
		bzero(&server_addr,sizeof(server_addr));
		if(server_ready(&socket_fd,&server_addr,addr,port,cap)==-1)
		{
				printf("server_ready() failed\n");
				return -1;
		}
		/*----------------服务器完成初始化了---------------------*/
		
		/*-----------------服务器运行阶段---------------------------
		 *<epoll(I/O复用）/while(1)/branch 1：socket_fd可读（accept）/branch 2: STDIN_FILENO可读(read/branch 1: down(close(socket_fd)))
		 */ 

		/*epoll
		 *可读(EPOLLIN):socket_fd,STDIN_FILENO；
		 */
	    struct epoll_event ev,evs[2];
		int epoll_fd=epoll_create(1);
		ev.events=EPOLLIN;
		ev.data.fd=socket_fd;
		if(epoll_ctl(epoll_fd,EPOLL_CTL_ADD,socket_fd,&ev)==-1)
		{
				perror("epoll_ctl(socket_fd) failed\n");
				return -1;
		}
		ev.events=EPOLLIN;
		ev.data.fd=STDIN_FILENO;
		if(epoll_ctl(epoll_fd,EPOLL_CTL_ADD,STDIN_FILENO,&ev)==-1)
		{
				perror("epoll_ctl(STDIN_FILENO) failed\n");
				return -1;
		}
		int epoll_num;
		int epoll_ev;
		int accept_fd;
		struct sockaddr_in client_addr;
		socklen_t addrlen=sizeof(client_addr);
		bzero(&client_addr,addrlen);
		node n;
		bzero(&n,sizeof(node));
		while(1)
		{
				/*wait indefinitely,amount to block*/
				epoll_num=epoll_wait(epoll_fd,evs,2,-1);
				if(epoll_num>0)
				{
						for(epoll_ev=0;epoll_ev<epoll_num;epoll_ev++)
						{
								/*监听套接字可读（接受链接）
								 *accept：从内核的TCP/IP协议组中已完成三次握手的队列中拿到一个链接，并正式建立链接，返回链接套接字
								 *node结点制作<链接套接字/put_fd/根路径/指向下一个结点的指针>
								 *node进队<队列头（被线程池p包含）/结点>
								 *唤醒在cond上等待的线程
								 * */
								if(evs[epoll_ev].data.fd==socket_fd && evs[epoll_ev].events==EPOLLIN)
							    {
										accept_fd=accept(socket_fd,(struct sockaddr*)&client_addr,&addrlen);
										printf("client: addr:%s, port:%d,accept_fd:%d\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port),accept_fd);
										bzero(&n,sizeof(node));
										n.accept_fd=accept_fd;
										n.put_fd=-1;
										strcpy(n.curdir,oridir);
										n.next=NULL;
										que_push(&p.q,n);
										pthread_cond_signal(&p.cond);
								 }
								 /*标准输入可读
								  *read:读标准输入；
								  *branch1:若为down，则关闭监听套接字（服务器断开链接)
								  *问：这个时候内核的TCP/IP组处于什么情况？
								  *问：监听套接字和内核的TCP/IP组是什么关系？
								  */
								 if(evs[epoll_ev].data.fd==STDIN_FILENO && evs[epoll_ev].events==EPOLLIN)
								 {
										 char buf[128];
										 read(STDIN_FILENO,buf,sizeof(buf));
										 if(strcmp("down\n",buf)==0)
										 {
												 close(socket_fd);
												 return 0;
										 }
								 }

								}
						}
				}
}
/*---------------------------------服务器主流程介绍完毕----------------------------------*/

/*---------------------------------线程体执行函数---------------------------------------*/
/*while(1):<获取线程池/获取报文队列/判断队列是否为空（若空则条件等待）/若非空，则让队列头元素出队/执行服务端的服务>
 */ 
void* thread_handle(void* arg)
{
		while(1)
		{
				pool* p=(pool*)arg;
				printf("the start is %d\n",p->start);
				que* q=&p->q;
				node n;
				if(que_empty(q))
				{
						pthread_cond_wait(&p->cond,&q->mutex);
						pthread_mutex_unlock(&q->mutex);
				}
				que_pop(q,&n);
				server_do(&n);
		}
}

/*------------------------------服务器功能封装--------------------------------------*/
/*获取根路径
 * while(runing)<获取包的内容/根据类型作出不同操作>
 *recv_pk：获取类型/获取长度/根据长度来读取报文
 * */
void server_do(node* n)
{
		packet pk;
		bzero(&pk,sizeof(packet));
		char filedir[128]={0};
		strcpy(filedir,n->curdir);
		int running=1;
		while(running)
		{
				recv_pk(n->accept_fd,&pk);
				choice(n,&pk,&running,filedir);
		}
		printf("close:fd=%d\n",n->accept_fd);
		close(n->accept_fd);

}

/*
 *选择：0<往链接写报文>/1<cd>/2<myls>/3<打开文件>/4<读取服务端文件内容并发给客户端>
 *******5<移除文件>/6<获取当前路径>/7<关闭链接>/8<退出循环>
 * */
int choice(node* n,packet* pk,int* running,char *filedir)
{
		switch(pk->type)
		{
				case 0:
						put(pk,n->put_fd);
						break;
				case 1:
						mycd(pk,n->curdir);
						send_pk(n->accept_fd,pk);
						break;
				case 2:
						myls(pk,n->curdir);
						send_pk(n->accept_fd,pk);
						break;
				case 3:
						myputs(pk,&(n->put_fd),filedir);
						send_pk(n->accept_fd,pk);
						break;
				case 4:
						mygets(n->accept_fd,pk,n->curdir);
						break;
				case 5:
						myremove(pk,n->curdir);
						send_pk(n->accept_fd,pk);
						break;
				case 6: 
						mypwd(pk,n->curdir);
						send_pk(n->accept_fd,pk);
						break;
				case 7:
						putend(&(n->put_fd));
						break;
				case 8:
						*running=0;
						break;
				default:break;
		}
		return 0;
}





