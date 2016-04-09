#ifndef _PACKET_
#define _PACKET_
/*printf*/
#include<stdio.h>

/*bzero/strcpy*/
#include <string.h>

/*recv/send*/
#include <sys/types.h>
#include <sys/socket.h>

typedef struct packet
{
		int type;
		int length;
		char buf[4088];
}packet;

int recv_pk(int fd,packet* pk);
int send_pk(int fd,packet* pk);
int make_pk(packet* pk,int type,int length,char* buf);
int view_pk(packet* pk);
#endif

