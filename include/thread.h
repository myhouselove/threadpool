#ifndef __THREAD_H_INCLUDED__
#define __THREAD_H_INCLUDED__


#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include "queue.h"
#include "system.h"


class Thread {
private:
    pthread_t _thread;
    bool _valid;
    bool _stopped;

    static void * start_routine(void * param)
    {
        Thread * thread = (Thread*)param;
        thread->run();
        thread->_stopped = true;
		
        return NULL;
    }
protected:
    virtual void run()
    {
    }
public:
    virtual void start()
    {	
        _valid = (pthread_create(&_thread, NULL, &start_routine, this) == 0);
		
    }
    bool stopped()
    {
        return _stopped;
    }
	void creat()
	{
		_valid = (pthread_create(&_thread, NULL, &start_routine, this) == 0);
	}
    void join()
    {
        if ( _valid ) {
            void * res;
            pthread_join( _thread, NULL );
            _valid = false;
        }
    }
	void exit(void *ptr)
    {
        if ( _valid ) {
            void * res;
            pthread_exit(ptr);
            _valid = false;
        }
    }
	void cancel()
    {
        if ( _valid ) {
            void * res;
            pthread_cancel(_thread);
            _valid = false;
        }
    }
    Thread()
    : _valid(false), _stopped(false)
    {
    }
    virtual ~Thread()
    {
        if ( _valid ) {
            pthread_detach( _thread );
        }
    }
};

class Mutex {
private:
    pthread_mutex_t _mutex;
    bool _valid;
public:
	friend class CondThread;
	friend class ThreadPool;
    Mutex()
    {
    	pthread_mutexattr_t attr;
    	pthread_mutexattr_init(&attr); 
    	// 设置 recursive 属性
    	pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE_NP); 
        _valid = ( pthread_mutex_init(&_mutex, &attr) ==0 );
    }
    ~Mutex()
    {
        if ( _valid )
            pthread_mutex_destroy( &_mutex );
    }
    bool lock()
    {
        if ( _valid )
            return (pthread_mutex_lock( &_mutex )==0);
        return false;
    }
    bool trylock()
    {
        if ( _valid )
            return (pthread_mutex_trylock( &_mutex )==0);
        return false;
    }
    void unlock()
    {
        if ( _valid )
            pthread_mutex_unlock( &_mutex );
    }
};

class Lock
{
  private:
  	pthread_mutex_t _mutex_t;
  public:
    Lock(pthread_mutex_t mt):_mutex_t(mt){
		pthread_mutex_lock( &_mutex_t );
	}
	~Lock(){
	pthread_mutex_unlock( &_mutex_t );
	}

};


class CondThread
{
	private:
    pthread_cond_t _cond;
    bool _valid;
public:
    CondThread()
    {
        _valid = ( pthread_cond_init(&_cond, NULL) ==0 );
    }
    ~CondThread()
    {
        if ( _valid )
            pthread_cond_destroy( &_cond );
    }
    bool signal()
    {
        if ( _valid )
            return (pthread_cond_signal( &_cond )==0);
        return false;
    }
    bool broadcast()
    {
        if ( _valid )
            return (pthread_cond_broadcast( &_cond )==0);
        return false;
    }
    void wait(Mutex *mutex)
    {
    	//this->_mutex = mutex->_mutex;
        if ( _valid )
            pthread_cond_wait( &_cond ,&(mutex->_mutex));
    }
	void timedwait( Mutex *mutex, const struct timespec * abstime)
    {
        if ( _valid )
            pthread_cond_timedwait( &_cond ,&(mutex->_mutex),abstime);
    }
		


};



class ThreadPool 
{
	private:
		pthread_t *_thread;
		pthread_t *_thread_bak;
		pthread_t taskqueue_thread;

		int u4sequence;
		int wait_time;
		int CANCEL_SIGNAL;
		bool F_improve_ThrdPoll;
		Mutex *mutex;
		CondThread *task_cond;

		TaskFuncCallback callback;
	
		int _num_threads;
		
		ThreadPool(int num_threads):_num_threads(num_threads),
			F_improve_ThrdPoll(0),
			wait_time(0),
			u4sequence(0),
			CANCEL_SIGNAL(0){
		init();
		ManagerThreadInit();
		ThreadPoolInit(num_threads);
		}
		
	public:
		LVQueue<TASK_QUEUE_T> task_wait_queue;
		LVQueue<TASK_QUEUE_T> task_doing_queue;

		~ThreadPool(){
			delete(mutex);
			delete(task_cond);
			delete(_thread);
			delete(_thread_bak);

		}
		//single class
		static ThreadPool* createThreadPool(int num)
		{	
			static ThreadPool *_pool = new ThreadPool(num);
			return _pool;
		}
		void init(){
			
			_thread = new pthread_t[_num_threads];
			mutex = new Mutex();
			task_cond = new CondThread();

		}
		
	    //friend void *thread_func(void *arg);
		//API_RETURN_TYPE_T ThreadTaskQueueInit();
		API_RETURN_TYPE_T ThreadPoolInit(int num_thr);
		API_RETURN_TYPE_T run();
		friend void clean_up(void *arg);
		API_RETURN_TYPE_T ManagerThreadInit();
		API_RETURN_TYPE_T managerThread();
		API_RETURN_TYPE_T wakeupThread(TaskFuncCallback p_func);
		API_RETURN_TYPE_T AutoComputeOptimumThreadNum(int wait_que_num,int &_u4sequence);
		API_RETURN_TYPE_T ThreadJoin();
		API_RETURN_TYPE_T ReleaseSubThreadPool();
		API_RETURN_TYPE_T DestroyThreadPool();
		
		
		
		
};

class CSingleton
{
private:
	CSingleton()   //构造函数是私有的
	{
	}
	
public:
	static CSingleton *m_pInstance;
	static CSingleton * GetInstance()
	{
		if(m_pInstance == NULL)  //判断是否第一次调用
			m_pInstance = new CSingleton();
		return m_pInstance;
	}
};




#endif

