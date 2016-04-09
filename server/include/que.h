#ifndef _QUE_
#define _QUE_
#include "server.h"

/*node 队列的元素结构*/
typedef struct fd_node
{
		int accept_fd;
		int put_fd;
		char curdir[128];
		struct fd_node* next;
}node;

/*que 队列的信息结构（头结点）*/
typedef struct fd_que
{
		node* head;
		node* tail;
		pthread_mutex_t mutex;
		int size;
		int cap;
}que;

int que_init(que* q/*in-out*/,int cap/*in*/);
int que_push(que* q/*in-out*/,node n/*in*/);
int que_pop(que* q/*in-out*/,node* n/*out*/);
int que_empty(que* q/*in*/);
#endif
