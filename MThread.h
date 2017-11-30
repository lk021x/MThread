#ifndef _THREAD_H
#define _THREAD_H

//编译的时候要加上参数: -lpthread
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

#define STACK_SIZE  (1024*1024)		//线程堆栈设置为1M

enum{
	THREAD_STATUS_NEW = 0,  //线程状态--新建
	THREAD_STATUS_RUNNING, //线程状态--正在运行
	THREAD_STATUS_EXIT //线程状态--运行结束
};

/*
* 线程基类
*
*/
class MThread
{
public:
	MThread();
	~MThread();
	virtual void run() = 0;  //用户实现
	void start();  						//开始执行线程
	void wait();  						//等待线程直至退出
	void suspend();
	void resume();

	int getFlag();
	pthread_mutex_t *getMutex();
	pthread_cond_t *getCond();

	int getState();  					//获取线程状态״̬
	pthread_t getThreadID(); 			//获取This线程ID
	void setThreadScope(bool isSystem);  //设置线程类型：绑定/非绑定
	bool getThreadScope();  			//获取线程类型
	void setThreadPriority(int type);
	void setThreadPriorityParam(int priority);//设置线程属性,1-99,其中99为实时，以外的为普通
	int getThreadPriority();  				//获取线程优先级
	void setThreadDetachState(bool isDetach); //设置线程分离属性

private:
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	int flag;
	int threadStatus; 					//线程的状态
	pthread_t pid;  					//当前线程的线程ID
	pthread_attr_t attr;  				//线程的属性
	int prio;  							//线程优先级
	struct sched_param param;
	static void* thread_proxy_func(void* pVoid);  	//run代理函数
	static int getNextThreadNum(); 		//获取一个线程序号
};

/*
* 互斥锁类
* java风格
*/
class CCriticalSection
{
public:
	CCriticalSection(){
		m_plock = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
		pthread_mutexattr_t attr;
		pthread_mutexattr_init(&attr);
		pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);  //互斥锁
		pthread_mutex_init(m_plock, &attr);
		pthread_mutexattr_destroy(&attr);
	}

	virtual ~CCriticalSection(){
		pthread_mutex_destroy(m_plock);
		free(m_plock);
		m_plock = NULL;
	}

	void lock(){
		pthread_mutex_lock(m_plock);
	}

	void unlock(){
		pthread_mutex_unlock(m_plock);
	}

protected:
	pthread_mutex_t *m_plock;
};

#endif /* _THREAD_H */


