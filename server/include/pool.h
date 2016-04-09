#ifndef _POOL_
#define _POOL_
#include"server.h"
#include"que.h"

/*线程体执行函数*/
typedef void* (*pfunc)(void*);

/*线程池属性<条件变量/>*/
typedef struct threaad_pool
{
		pthread_cond_t cond;/*条件变量*/
		pthread_t *pth;/*池<堆分配>*/
		pfunc entry;/*线程体执行函数*/
		que q;/*队列头*/
		int start;/*标志位<线程池是否启动了>*/
		int num;/*线程池的线程数*/
}pool;

int pool_init(pool* p/*out*/,int num/*in*/,int cap/*in*/,pfunc entry/*in*/);
int pool_start(pool* p/*in-out*/);
#endif
