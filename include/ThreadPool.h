#ifndef _THREADPOOL_H
#define _THREADPOOL_H
#pragma once

#include <atomic>
//#include <boost/asio.hpp>
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
using namespace std;

#define _DEF_LIMIT 10 //保证每次扩容的线程不超过10个
#define TIME_OUT 3

class ThreadPool
{
public://线程池参数
	bool shutdown;		
	uint32_t thread_min;    
	uint32_t thread_max;
	uint32_t thread_alive;
	uint32_t thread_busy;
	uint32_t thread_kill;
public://任务队列
	typedef struct
	{
		void* (*task)(void*);
		void* arg;
	}task_t;
	uint32_t queue_max;
	uint32_t queue_head;
	uint32_t queue_tail;
	uint32_t queue_cur;
	task_t* task;
	pthread_t* arr_pthread_tid;    //线程id数组
	pthread_t manage_tid;          //管理线程id
	static ThreadPool* m_instance; //线程池实例对象

	pthread_cond_t not_empty;
	pthread_cond_t not_full;
	pthread_mutex_t lock_task;

private:
	ThreadPool();
public:
	~ThreadPool();
public:
	static ThreadPool* GetInstance()//单例模式公共接口
	{
		for(;;)
		{
			if(m_instance)
				return m_instance;
			ThreadPool* threadpool = new ThreadPool();
			if(!m_instance)
			{
				if(__sync_bool_compare_and_swap(&m_instance,NULL,threadpool))
				{
					threadpool = NULL;
					return m_instance;
				}
				else
				{
					delete threadpool;
					threadpool = NULL;
				}
			}
		}
	
	}
public:
	bool InitThreadPool(int max,int min,int queue_max);
	bool DestroyThreadPool();
	bool AddTask(void* (*task)(void*),void* arg);
	static void * ConsumeTask(void*);
	bool if_thread_alive(pthread_t pid);
	static void* ManagerThreadPool(void*);
};

#endif// _THREADPOOL_H

