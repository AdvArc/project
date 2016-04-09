#include"control.h"

/*往链接写报文<I/O操作><系统调用/陷入内核，写内核的发送缓冲区>*/
int put(packet* pk/*out*/,int fd/*in*/)
{
		write(fd,pk->buf,pk->length);
		return 0;
}

/*关闭链接<系统调用，关闭文件描述符>*/
int putend(int *fd/*in-out*/)
{
		close(*fd);
		*fd = -1;
		return 0;
}

/*cd*/
int mycd(packet* pk/*in-out*/,char* curdir/*in-out*/)
{
		/*切换路径*/
		chdir(curdir);
		chdir(pk->buf);
		/*发送当前的绝对路径*/
		char buf[128];
		bzero(buf,128);
		getcwd(buf,128);
		pk->length = strlen(buf);
		strcpy(pk->buf,buf);
		strcpy(curdir,buf);
		return 0;
}

/*ls<改变工作目录/打开一个目录/读目录获取文件名>
 *char *strcat(char *dest, const char *src);
 * */
int myls(packet* pk/*out*/,char* curdir/*in*/)
{
		bzero(pk->buf,sizeof(pk->buf));
		chdir(curdir);
		DIR* dir = opendir(getcwd(NULL,0));
		if(NULL == dir)
				return -1;
		struct dirent* pt;
		while((pt=readdir(dir)))
		{
				if((!strcmp(pt->d_name,"."))||(!strcmp(pt->d_name,"..")))
				{}
				else
				{
						/*这里涉及格式*/
						strcat(pk->buf,pt->d_name);
						strcat(pk->buf," ");
				}
		}
		pk->length=strlen(pk->buf);
		return 0;
}

/*打开文件<切换工作目录/打开文件/全局错误errno>*/
int myputs(packet* pk/*in*/,int* fd/*out*/,char* filedir/*in*/)
{
		chdir(filedir);
		*fd=open(pk->buf,O_RDWR|O_CREAT|O_APPEND,0666);
		if(*fd==-1)
		{
				perror("open() failed");
				return -1;
		}
		printf("fd=%d\n",*fd);
		return 0;
}

/*读取服务端文件内容并发给客户端
 *<切换工作目录/循环读文件--发数据/制作并发送0包/关闭链接>
 * */
int mygets(int sfd/*in*/,packet* pk/*in-out*/,char* curdir/*in*/)
{
		chdir(curdir);
		int getfd=open(pk->buf,O_RDONLY);
		if(getfd==-1)
		{
				perror("open() failed\n");
				pk->type=-1;
				return -1;
		}
		send_pk(sfd,pk);
		while(1)
		{
				bzero(pk,sizeof(struct packet));
				pk->type=0;
				pk->length=read(getfd,pk->buf,sizeof(pk->buf));
				if(pk->length>0)
						send_pk(sfd,pk);
				else
						break;
		}
		make_pk(pk,7,0,"");/*制作0包，表示结束*/
		send_pk(sfd,pk);
		close(getfd);
		return 0;
}

/*移除文件<切换工作目录/获取当前绝对路径/字符串拼接/形成要删除文件的绝对路径>*/
int myremove(packet* pk/*in*/,char* curdir/*in*/)
{
		chdir(curdir);
		char removebuf[128]={0};
		getcwd(removebuf,128);
		strcat(removebuf,"/");
		strcat(removebuf,pk->buf);
		if(unlink(removebuf)<0)
				return -1;
		return 0;
}

/*获取当前路径<切换工作目录/获取当前路径>*/
int mypwd(packet* pk/*out*/,char* curdir/*in*/)
{
		chdir(curdir);
		getcwd(pk->buf,128);
		pk->length=strlen(pk->buf);
		return 0;
}







