#include"control.h"

int mycd(packet* pk)
{
		printf("%s\n",pk->buf);
		return 0;
}

int myls(packet* pk)
{
		printf("%s\n",pk->buf);
		return 0;
}

int myputs(int sfd,packet* pk)
{
		int putfd=open(pk->buf,O_RDONLY);
		if(putfd==-1)
		{
				perror("open() failed\n");
				pk->type=-1;
				return -1;
		}
		printf("fd=%d\n",putfd);
		while(1)
		{
				bzero(pk,sizeof(*pk));
				pk->type=0;
				pk->length=read(putfd,pk->buf,sizeof(pk->buf));
				if(pk->length>0)
						send_pk(sfd,pk);
				else
						break;
		}
		make_pk(pk,7,0,"");
		send_pk(sfd,pk);
		close(putfd);
		return 0;
}


int mygets(packet* pk,int* fd)
{
		*fd=open(pk->buf,O_RDWR|O_CREAT|O_APPEND,0666);
		if(*fd==-1)
		{
				perror("open() failed");
				return -1;
		}
		return 0;
}

int get(packet* pk,int* fd)
{
		write(*fd,pk->buf,pk->length);
		return 0;
}

int getend(int* fd)
{
		close(*fd);
		*fd=-1;
		printf("gets successed\n");
		return 0;
}
int myremove(packet* pk)
{
		printf("remove successed\n");
		return 0;
}
int mypwd(packet* pk)
{
		printf("%s\n",pk->buf);
		return 0;
}


