#ifndef _CONTROL_
#define _CONTROL_
#include "server.h"
#include "packet.h"
#include <memory.h>

/*format of directory entries
 *opendir/readdir
 * */
#include <dirent.h>


int put(packet* pk/*out*/,int fd/*in*/);

int putend(int *fd/*in-out*/);

int mycd(packet* pk/*in-out*/,char* curdir/*in-out*/);

int myls(packet* pk/*out*/,char* curdir/*in*/);

int myputs(packet* pk/*in*/,int* fd/*out*/,char* filedir/*in*/);

int mygets(int sfd/*in*/,packet* pk/*in-out*/,char* curdir/*in*/);

int myremove(packet* pk/*in*/,char* curdir/*in*/);

int mypwd(packet* pk/*out*/,char* curdir/*in*/);
#endif

