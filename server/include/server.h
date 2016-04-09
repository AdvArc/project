#ifndef _SERVER_
#define _SERVER_
/*standard input & output"（标准输入输出）*/
/*fopen/fscanf/perror/printf*/
#include<stdio.h>

/*standard library标准库头文件*/
/*calloc/free*/
#include<stdlib.h>

/*unistd.h 中所定义的接口通常都是大量针对系统调用的封装（英语：wrapper functions），如 fork、pipe 以及各种 I/O 原语（read、write、close 等等）*/
/*write/close/chdir/read
 *unlink
 * */
#include<unistd.h>

/*C语言里面关于字符数组的函数定义的头文件，常用函数有strlen、strcmp、strcpy等等，更详细的可以到include文件夹里面查看该文件*/
/*memcpy/strcmp/strcat/strlen/bzero/strcpy*/
#include<string.h>

/* file control 文件控制*/
/*open*/
#include<fcntl.h>

/*data types*/
/*socket/bind
 *opendir
 *open
 * */
#include<sys/types.h>

/*data returned by the stat() function*/
/*open/accept*/
#include<sys/stat.h>

/*main sockets header*/
/*socket/sockaddr_in/sockaddr/inet_addr/bind/listen/accept*/
#include<sys/socket.h>

/*I/O复用*/
/*struct epoll_event/epoll_create/epoll_ctl/epoll_wait
 *EPOLLIN
 */
#include<sys/epoll.h>

/*Internet address family*/
/*sockaddr_in/inet_addr*/
#include<netinet/in.h>

/*definitions for internet operations*/
/*htons/inet_addr*/
#include<arpa/inet.h>

/*threads*/
/*pthread_mutex_t mutex/pthread_mutex_init/pthread_mutex_lock
 *pthread_mutex_unlock
 *pthread_cond_init
 *pthread_create
 *pthread_cond_signal
 * */
#include<pthread.h>

int conf_load(char* server_conf/*in*/,char *addr/*out*/,int* port/*out*/,int* capacity/*out*/,int*thread_num/*out*/);

/*这个接口可优化*/
int server_ready(int* socket_fd/*out*/,struct sockaddr_in* server_addr/*in-out??*/,char* addr/*in*/,int  port/*in*/,int capacity/*in*/);

#endif
