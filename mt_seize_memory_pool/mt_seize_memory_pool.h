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

//ͷ�ļ���_GNU_SOURCE���Ӱ��,��ͷ�ļ�Ӧ�����ȱ�����,��_GNU_SOURCE����Ԥ����
#include <stdio.h>
#include <stdint.h>

enum {
	MEMORY_POOL_FLAG_INUSED = 0x01, 
};

//Ĭ�Ϸ�����С���ڴ��(���޸ĸ�ֵ)
#define DEFAULT_ALLOC_SIZE (8192)

typedef struct memory_pool_head {
	struct memory_pool_head*	mh_tail_next;				//����β���е�ʹ��
	struct memory_pool_head**	mh_tail_prev;	
	struct memory_pool_head*	mh_prevptr;					//��һ�������ڴ�صĵ�ַ����alloc��ʼ��

	void* mh_end;
	uint32_t mh_size;										//��ǰͷ�������������ĳ���
	uint8_t mh_flag;										//��ǰͷ�ı�ʶ

	char*	mh_data;										//ʹ�õ��ڴ��
}memory_pool_head_t;

typedef struct memory_poll {
	memory_pool_head_t* inused_tailq;						//��ʹ�����ڴ���β����
	memory_pool_head_t* unused_head[24];					//��λ��ȷ��ĳ�����ȵ��ڴ��洢���Ǹ������� [0]128
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