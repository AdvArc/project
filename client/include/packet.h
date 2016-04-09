#ifndef _PACHET_
#define _PACKET_
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
