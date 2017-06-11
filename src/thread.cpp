#include "../include/thread.h"
#include "../include/queue.h"

#include <unistd.h>

void clean_up(void *arg)
{
	
}


API_RETURN_TYPE_T ThreadPool::run()
{
	//printf("this is run  thread.\n");
	void *arg;
	while(1)
	{
		//usleep(400000);
		
		printf ("thread 0x%x begin\n", pthread_self ()); 
		this->mutex->lock();
		if((CANCEL_SIGNAL == 0) && (task_doing_queue.length() < _num_threads || F_improve_ThrdPoll == 1) )
		{
			printf ("thread 0x%x is waiting\n", pthread_self ());  
			this->task_cond->wait(mutex);
		}
		printf("123.\n");
		usleep(200000);
		this->mutex->unlock();
		pthread_testcancel();
		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);
		if(callback != NULL)
		{
			callback(arg);
			callback = NULL;
		}
		task_doing_queue.popFront();
		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
		printf("wait len =%d.\n",task_wait_queue.length());
		printf("thread 0x%x done.length() = %d.\n",pthread_self (),task_doing_queue.length());

	}

	return API_SUCCESS;

}
API_RETURN_TYPE_T ThreadPool::managerThread()
{
	while(1)
	{
		usleep(400000);
		printf("managerThread!.\n");
		this->mutex->lock();
		TASK_QUEUE_T task1;
		TASK_QUEUE_T task2;
		task1.sTask = TASK_DOING;
		if(task_wait_queue.length() != 0)
		{
			//printf("len =%d.\n",task_doing_queue.length());
			if(task_doing_queue.length() < _num_threads)
			{
				task2 = task_wait_queue.popFront();
				callback = task2.cTaskFunc;
				task_doing_queue.pushBack(task1);
				task_cond->signal();
				printf("signal cond.\n");
			}

		}
		if(task_wait_queue.length() >= _num_threads && F_improve_ThrdPoll == 0)
		{
			AutoComputeOptimumThreadNum(task_wait_queue.length(),u4sequence);
			F_improve_ThrdPoll = 1;
			ThreadPoolInit(u4sequence);	
			sleep(2);
		}
		if(F_improve_ThrdPoll == 1 )
		{
			while(task_wait_queue.length() == 0 && 
			task_doing_queue.length() == 0)
			{
				printf("no task!.\n");
				usleep(500000);
				wait_time++;
				if(wait_time == NO_TASK_TIMEOUT)
				{
					this->mutex->unlock();
					ReleaseSubThreadPool();
					printf("release!.\n");
					F_improve_ThrdPoll = 2;
					wait_time = 0;
					break;
				}
			}
			wait_time = 0;
		}
		if(F_improve_ThrdPoll != 2)
			this->mutex->unlock();
	}
	return API_SUCCESS;

}
void  *thread_func(void *arg)
{
	ThreadPool *thread = (ThreadPool*)arg;
	thread->run();
}
void *thread_task_queue(void *arg)
{	
	ThreadPool *thread = (ThreadPool*)arg;
	thread->managerThread();
	
}

API_RETURN_TYPE_T ThreadPool::AutoComputeOptimumThreadNum(int wait_que_num,int &_u4sequence)
{
	if(wait_que_num >= 4*_num_threads)
	{
		_u4sequence = _num_threads;
	}
	else if(wait_que_num >= 2*_num_threads)
	{
		_u4sequence = _num_threads/2;
	}
	else
	{
		_u4sequence = 0;
	}
	return API_SUCCESS;
}

API_RETURN_TYPE_T ThreadPool::ManagerThreadInit()
{
	//create manager threadpool thread.
	if(RET_OK != pthread_create(&taskqueue_thread, NULL, &thread_task_queue, this))
	{	
		return API_FAIL;
	}
	return API_SUCCESS;
}

API_RETURN_TYPE_T ThreadPool::ThreadPoolInit(int num_thr)
{
	printf("num = %d.\n",num_thr);	
	if(num_thr == 0)
	{
		return API_SUCCESS;
	}
	pthread_attr_t attr;
	pthread_attr_init (&attr);
	pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_DETACHED);
	
	int i = 0;
	if(F_improve_ThrdPoll == 1)
	{
		_thread_bak = new pthread_t[num_thr];
		for(;i < num_thr;i++)
		{
			if(RET_OK != pthread_create(&(_thread_bak[i]), &attr, &thread_func, this))
			{
				return API_FAIL;
			}
		}
		return API_SUCCESS;
	}
	//create thread pool.
	for(;i < num_thr;i++)
	{
		if(RET_OK != pthread_create(&(_thread[i]), &attr, &thread_func, this))
		{
			return API_FAIL;
		}
	}

	pthread_attr_destroy (&attr);
	return API_SUCCESS;
}

API_RETURN_TYPE_T ThreadPool::ReleaseSubThreadPool()
{
	this->mutex->lock();
	CANCEL_SIGNAL = 1;
	this->mutex->unlock();
	task_cond->broadcast();
	for(int i = 0;i < _num_threads;i++)
	{
		if(RET_OK != pthread_cancel(_thread_bak[i]))
		{
			return API_FAIL;
		}
	}
	this->mutex->lock();
	printf("4444.\n");
	CANCEL_SIGNAL = 0;
	this->mutex->unlock();

	return API_SUCCESS;
}

API_RETURN_TYPE_T ThreadPool::DestroyThreadPool()
{
	//first ,destroy manager thread.
	if(RET_OK != pthread_cancel(taskqueue_thread))
	{
		return API_FAIL;
	}
	if(task_doing_queue.length() == 0)
	{
		
	}

	return API_SUCCESS;
}


API_RETURN_TYPE_T ThreadPool::ThreadJoin()
{
	for(int i = 0;i < _num_threads;i++)
	{
		pthread_join(_thread[i],NULL);
		
	}
	pthread_join(taskqueue_thread,NULL);

	return API_SUCCESS;

}
API_RETURN_TYPE_T ThreadPool::wakeupThread(TaskFuncCallback p_func)
{
	printf("wakeupThread in .\n");
	this->mutex->lock();
	TASK_QUEUE_T task;
	task.cTaskFunc = p_func;
	task.sTask = TASK_WAIT;
	if(task_wait_queue.length() < MAX_TASK_NUM )
	{	
		this->task_wait_queue.pushBack(task); 
	}
	else
	{
		printf("Current Thread Buffer is full!Please wait a moment!\n");
		this->mutex->unlock();
		return API_FAIL;
	}
	this->mutex->unlock();
	
	
	return API_SUCCESS;

}





