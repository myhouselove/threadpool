#include <iostream>
using namespace std;

#include "../include/client.h"
#include "../include/socket.h"
#include "../include/thread.h"

#include <stdio.h>
#include <string.h>


//Mutex *mutex = new Mutex();
//CondThread *cond = new CondThread();
int flag = 1;
pthread_mutex_t mm;
pthread_cond_t cc;

int j = 0;

void thread11_func()
{
	//float rate = GetHostCPURate();
	//system("cat /proc/stat | grep -v ^[intr]");
	//cout<<"rate = "<<rate<<endl;
	printf("this is %d task  thread.\n",j++);
	
	#if 0//sleep(2);
		pthread_mutex_lock(&mm);

		printf("signal cond,flag = %d.\n",flag);

		if(flag == 1)
			flag = 0;
		else
			flag = 1;
		pthread_mutex_unlock(&mm);

		pthread_cond_signal(&cc);
#endif

}
void thread3_func(void *arg)
{
	float rate = GetHostCPURate();
	//system("cat /proc/stat | grep -v ^[intr]");
	cout<<"rate = "<<rate<<endl;
	printf("this is %d task  thread.\n",j++);
	
	/*	pthread_mutex_lock(&mm);
		if(flag == 1)
		{
			pthread_cond_signal(&cc);
			printf("signal cond,flag = %d.\n",flag);
		}
		flag = 0;
		pthread_mutex_unlock(&mm);

*/
}


void *thread22_func(void *arg)
{
	//printf("this is two  thread.\n");
	 
	while(1)
	{
	printf ("this thread is 0x%x .\n", pthread_self ());
		pthread_mutex_lock(&mm);
		if(flag == 1)
		{
		printf("wait cond,flag = %d.\n",flag);
		pthread_cond_wait(&cc,&mm);
			//printf("wait cond,flag = %d.\n",flag);
		}
		pthread_mutex_unlock(&mm);
		
		printf (" thread is 0x%x doing sth .\n", pthread_self ());
		sleep(10);
		//flag ++;
		
	}
}



int main(int argc,char *argv[])
{
#if 0
	pthread_t tid1[4];
	pthread_t tid2,tid3;
	void *arg;
	
	pthread_mutex_init(&mm, NULL); 
	pthread_cond_init(&cc, NULL);
	int i =0;
	for(;i < 4;i++)
		pthread_create(&tid1[i],NULL,&thread22_func,NULL);
	
	sleep(3);
	pthread_create(&tid2,NULL,&thread11_func,NULL);
	sleep(3);
	thread11_func(arg);
	//pthread_create(&tid3,NULL,&thread11_func,NULL);
	sleep(25);
	//pthread_exit(0);

return 0;
#endif
	if(argc == 1)
	{
		argv[1] = new char[30];
		strcpy(argv[1],"127.0.0.1");//use local ip.
	}
	if(argc == 2)
	{
		char ping_IP[30];
		printf("Your input server ip is %s \n",argv[1]);
		sprintf(ping_IP,"ping %s -c 3",argv[1]);//check 3 times then end.
		printf("Now Test Your IP by cmd 'ping' \n");
		system(ping_IP);
	}


	BankCustomerClientBase *bs = new BankCustomerClientICBC();
	
	Thread *thread1 = new ICBCClientThread(argv[1],bs,thread11_func);
	thread1->join();
	return 0;
#if 0

	float rate = GetHostCPURate();
	cout<<"rate = "<<rate<<endl;
	//return 0;
	//BankCustomerClientBase *bs = new BankCustomerClientICBC();
	
	//Thread *thread1 = new ICBCClientThread(argv[1],bs,task_func);
	//Thread *thread2 = new ICBCClientThread(argv[1],bs,thread2_func);
	ThreadPool *thread3 = ThreadPool::createThreadPool(8);

	//CSingleton *cs = CSingleton::GetInstance();
	//thread3->_thread[]
	//sleep(5);
	printf("task coming.\n");
	//test threadpool
	for(int i = 0;i < 15;i++)
	{
	thread3->wakeupThread(thread11_func);
	//sleep(1);
	thread3->wakeupThread(thread3_func);
	
	}
	sleep(60);
	for(int j = 0;j < 15;j++)
	{
	thread3->wakeupThread(thread11_func);
	//sleep(1);
	thread3->wakeupThread(thread3_func);
	
	}
	cout<<"first client!"<<endl;
	//pthread();
	//sleep(100);
	//thread3->ThreadJoin();
#endif
	return 0;
}

