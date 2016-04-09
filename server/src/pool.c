#include"pool.h"
/*线程池初始化<条件变量/线程池大小(堆空间申请内存)>*/
int pool_init(pool* p/*out*/,int num/*in*/,int cap/*in*/,pfunc entry/*in*/){
		if(pthread_cond_init(&p->cond,NULL)!=0)
		{
				printf("pthread_cond_init() failed\n");
				return -1;
		}

		/*池容量*/
		p->pth=(pthread_t*)calloc(num,sizeof(pthread_t));
		p->entry=entry;

		/*初始化线程池的时候队列也随之初始化*/
		que_init(&p->q,cap);
		p->start=0;
		p->num=num;
		return 0;
}
/*启动线程池<创建线程(回调函数）/启动状态标记>*/
int pool_start(pool* p/*in-out*/)
{
		if(p->start==0)
		{
				int i;
				for(i=0;i<p->num;i++)
				{
						if(pthread_create(&p->pth[i],NULL,p->entry,(void*)p)!=0)
						{
								printf("pthread_create() failed\n");
								return -1;
						}
				}
				p->start=1;
		}
		return 0;
}
