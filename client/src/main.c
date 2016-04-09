#include "client.h"
#include "control.h"

int make(char *buf,int length,packet* pk);

int choice(int sfd,packet* pk,int* fd);
int main(int argc,char *argv[])
{
		
		if(argc!= 3)
		{
				printf("error argcs\n");
				return -1;
		}
		
		chdir("../file");
		char addr[15];
		char port[4];
		strcpy(addr,argv[1]);
		strcpy(port,argv[2]);
		printf("server: add:%s,port:%s.\n",addr,port);
		int socket_fd = socket(AF_INET,SOCK_STREAM,0);
		if(socket_fd==-1)
		{
				perror("socket() failed\n");
				return -1;
		}
		struct sockaddr_in server_addr;
		bzero(&server_addr,sizeof(server_addr));
		server_addr.sin_family=AF_INET;
		server_addr.sin_port=htons(atoi(port));
		server_addr.sin_addr.s_addr=inet_addr(addr);
		if(connect(socket_fd,(struct sockaddr*)&server_addr,sizeof(struct sockaddr))==-1)
		{
				perror("connect()\n");
				return -1;
		}
		printf("client is ready.\n");
		int epoll_fd=epoll_create(1);
		struct epoll_event ev,evs[2];
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
				perror("epoll_ctl(STDIN) failed\n");
				return -1;
		}
		int epoll_num;
		int epoll_ev;
		packet pk;
		bzero(&pk,sizeof(pk));
		int getfd=-1;
		while(1)
		{
				bzero(&evs,sizeof(evs));
				epoll_num=epoll_wait(epoll_fd,evs,2,-1);
				if(epoll_num>0)
				{
						for(epoll_ev=0;epoll_ev<epoll_num;epoll_ev++)
						{
								if(evs[epoll_ev].data.fd==socket_fd && evs[epoll_ev].events==EPOLLIN)
								{
										recv_pk(socket_fd,&pk);
										choice(socket_fd,&pk,&getfd);
								}
								if(evs[epoll_ev].data.fd==STDIN_FILENO && evs[epoll_ev].events==EPOLLIN)
								{
										char buf[128];
										bzero(buf,sizeof(buf));
										read(STDIN_FILENO,buf,sizeof(buf));
										make(buf,strlen(buf)-1,&pk);
										send_pk(socket_fd,&pk);	
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

int make(char *buf,int length,packet* pk)
{
		if(buf[0]=='c'&&buf[1]=='d'&&length>3)
		{
				make_pk(pk,1,length-3,buf+3);
				return 0;
		}
		if(buf[0]=='l'&&buf[1]=='s'&&length==2)
		{
				make_pk(pk,2,length,buf);
				return 0;
		}
		if(buf[0]=='p'&&buf[1]=='u'&&buf[2]=='t'&&buf[3]=='s'&&length>5)
		{
				make_pk(pk,3,length-5,buf+5);
				return 0;
		}
		if(buf[0]=='g'&&buf[1]=='e'&&buf[2]=='t'&&buf[3]=='s'&&length>5)
		{
				make_pk(pk,4,length-5,buf+5);
				return 0;
		}
		if(buf[0]=='r'&&buf[1]=='e'&&buf[2]=='m'&&buf[3]=='o'&&buf[4]=='v'&&buf[5]=='e'&&length>7)
		{
				make_pk(pk,5,length-7,buf+7);
				return 0;
		}
		if(buf[0]=='p'&&buf[1]=='w'&&buf[2]=='d'&&length==3)
		{
				make_pk(pk,6,length,buf);
				return 0;
		}
		if(buf[0]=='d'&&buf[1]=='o'&&buf[2]=='w'&&buf[3]=='n'&&length==4)
		{
				make_pk(pk,8,length,buf);
				return 0;
		}
		printf("error\n");
		make_pk(pk,-1,length+1,buf);
		return 0;
}
int choice(int sfd,packet* pk,int* fd)
{
		switch(pk->type)
	    {
		case 0:
			get(pk,fd);
			break;
		case 1:
			mycd(pk);
			break;
		case 2:
			myls(pk);
			break;
		case 3:
			myputs(sfd,pk);
			break;
		case 4:
			mygets(pk,fd);
			break;
		case 5:
			myremove(pk);
			break;
		case 6:
			mypwd(pk);
			break;
		case 7:
			getend(fd);
			break;
		default:
			break;
		}
		return 0;
}

