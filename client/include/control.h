#ifndef _CONTROL_
#define _CONTROL_
#include "packet.h"
#include "memory.h"
#include "client.h"
int get(packet* pk,int* fd);
int getend(int* fd);
int mycd(packet* pk);
int myls(packet* pk);
int myputs(int sfd,packet* pk);
int mygets(packet* pk,int* fd);
int myremove(packet* pk);
int mypwd(packet* pk);
#endif
