#include"packet.h"

/*接收报文
 * 获取包头：类型、长度
 * 获取数据信息
 */
int recv_pk(int fd,packet* pk)
{
		/*包头接受可做优化*/
		bzero(pk,sizeof(packet));
		recv(fd,&pk->type,4,0);
		recv(fd,&pk->length,4,0);
		int total = 0;
		int size = 0;
		while(total<pk->length)
		{
				size=recv(fd,pk->buf+total,pk->length-total,0);
				total=total+size;
		}
		return 0;
}

/*发送报文
 *填入包头：类型、长度
 *填入数据信息
 */
int send_pk(int fd,packet* pk)
{
        /*包头可做优化*/
		send(fd,&pk->type,4,0);
		send(fd,&pk->length,4,0);
		int total = 0;
		int size =0;
		while(total<pk->length)
		{
				size=send(fd,pk->buf+total,pk->length-total,0);
				total=total+size;
		}
		return 0;
}

/*制作数据包<类型/数据长度/数据信息>*/
int make_pk(packet* pk,int type,int length,char* buf)
{
		bzero(pk,sizeof(packet));
		pk->type=type;
		pk->length=length;
		strcpy(pk->buf,buf);
		return 0;
}
/*打印数据包内容*/
int view_pk(packet* pk)
{
		printf("type:%d ",pk->type);
		printf("length:%d ",pk->length);
		printf("buf:%s\n",pk->buf);
		return 0;
}


