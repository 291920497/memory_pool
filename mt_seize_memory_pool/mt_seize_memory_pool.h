#ifndef _MT_SEIZE_MEMORY_POOL_H_
#define _MT_SEIZE_MEMORY_POOL_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
#include <Windows.h>
#else
#define _GNU_SOURCE
#include <pthread.h>
#endif//_WIN32

//头文件受_GNU_SOURCE宏的影响,此头文件应当首先被引用,或将_GNU_SOURCE加入预处理
#include <stdio.h>
#include <stdint.h>

enum {
	MEMORY_POOL_FLAG_INUSED = 0x01, 
};

//默认分配最小的内存块(可修改该值)
#define DEFAULT_ALLOC_SIZE (8192)

typedef struct memory_pool_head {
	struct memory_pool_head*	mh_tail_next;				//用于尾队列的使用
	struct memory_pool_head**	mh_tail_prev;	
	struct memory_pool_head*	mh_prevptr;					//上一个连续内存池的地址。由alloc初始化

	void* mh_end;
	uint32_t mh_size;										//当前头所包含缓冲区的长度
	uint8_t mh_flag;										//当前头的标识

	char*	mh_data;										//使用的内存块
}memory_pool_head_t;

typedef struct memory_poll {
	memory_pool_head_t* inused_tailq;						//在使用中内存块的尾队列
	memory_pool_head_t* unused_head[24];					//以位来确定某个长度的内存块存储在那个队列中 [0]128
#ifdef _WIN32
	CRITICAL_SECTION unused_lock;
	CRITICAL_SECTION inused_lock;
	uint32_t thread_id;
#else
	pthread_spinlock_t unused_lock;
	pthread_spinlock_t inused_lock;
#endif//_WIN32
}memory_poll_t;


int mt_seize_memory_create();

void mt_seize_memory_destroy();

void* mt_seize_malloc(size_t size);

void mt_seize_free(void* free_ptr);

#ifdef __cplusplus
}
#endif


#endif//_MT_SEIZE_MEMORY_POOL_H_