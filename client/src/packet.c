#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include"packet.h"

int recv_pk(int fd,packet* pk)
{
		bzero(pk,sizeof(packet));
		recv(fd,&pk->type,4,0);
		recv(fd,&pk->length,4,0);
		int total=0;
		int size=0;
		while(total<pk->length)
		{
				size=recv(fd,pk->buf+total,pk->length-total,0);
				total=total+size;
		}
		return 0;
}
int send_pk(int fd,packet* pk)
{
		send(fd,&pk->type,4,0);
		send(fd,&pk->length,4,0);
		int total=0;
		int size=0;
		while(total<pk->length)
		{
				size=send(fd,pk->buf+total,pk->length-total,0);
				total=total+size;
		}
		return 0;
}

int make_pk(packet* pk,int type,int length,char* buf)
{
		bzero(pk,sizeof(packet));	
		pk->type=type;
		pk->length=length;
		strcpy(pk->buf,buf);
		return 0;
}

int view_pk(packet* pk)
{
		printf("type:%d ",pk->type);
		printf("length:%d ",pk->length);
		printf("buf:%s\n",pk->buf);
		return 0;
}
