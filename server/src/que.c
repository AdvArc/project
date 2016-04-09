#include"que.h"

/*初始化队列<头指针、尾指针、线程锁、当前元素个数、最大元素个数>*/
int que_init(que* q/*in-out*/,int cap/*in*/)
{
		q->head=NULL;
		q->tail=NULL;
		if(pthread_mutex_init(&q->mutex,NULL)!=0)
		{
				printf("pthread_mutex_init failed\n");
				return -1;
		}
		q->size=0;
		q->cap=cap;
		return 0;
}

/*结点进队<临时区（线程锁）/堆区申请内存/尾插法>*/
int que_push(que* q/*in-out*/,node n/*in*/)
{
		pthread_mutex_lock(&q->mutex);
		node* new_n=(node*)calloc(1,sizeof(node));
		memcpy(new_n,&n,sizeof(node));
		if(q->tail==NULL)
		{
				q->head=new_n;
				q->tail=new_n;
		}
		else
		{
				q->tail->next=new_n;
				q->tail=q->tail->next;
		}
		q->size++;
		pthread_mutex_unlock(&q->mutex);
		return 0;
}

/*结点出队<临时区（线程锁）/堆区释放内存/头取法>
 *堆区释放内存需要判断指针所指的内存地址是否有效
 */ 
int que_pop(que* q/*in-out*/,node* n/*out*/)
{
		pthread_mutex_lock(&q->mutex);
		if(q->head!=NULL)
		{
				node* cur=q->head;
				memcpy(n,cur,sizeof(node));
				if(q->head!=q->tail)
				{
						q->head=q->head->next;
				}
				else
				{
						q->head=NULL;
						q->tail=NULL;
				}
				free(cur);
				q->size--;
		}
		pthread_mutex_unlock(&q->mutex);
		return 0;
}
/*判断队列是否为空<临时区（线程锁）>*/
int que_empty(que* q/*in*/)
{
		pthread_mutex_lock(&q->mutex);
		if(q->size==0)
				return 1;
		pthread_mutex_unlock(&q->mutex);
		return 0;
}
