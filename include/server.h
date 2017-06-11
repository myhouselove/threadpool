#ifndef __SERVER_H_INCLUDED__
#define __SERVER_H_INCLUDED__

#define MAX_LENTH (10)
#define PORT (6666)

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "../include/thread.h"
#include <string>

class BankServerCenterBase{

	public:
		BankServerCenterBase(){printf("this is a bank!\n");}
		virtual ~BankServerCenterBase(){}
	virtual void ServerDo(){}
	private:
		//int a;
	
	

	
};

class ICBCServerCenter : public BankServerCenterBase
{
	public:
		ICBCServerCenter(){ 
		//printf("a is %d.\n",a);
		}
	void ServerDo();

	private:
	char _name[MAX_LENTH];
	long _pubPhone;
};


class ICBCServerThread : public Thread
{
	private:
		BankServerCenterBase *_bscb;
		int _number;
	public:
		ICBCServerThread(int number,BankServerCenterBase *bscb):_number(number),_bscb(bscb)
			{
			start();
		}

	void run()
		{
	//cout<<"this ICBC thread's number is "<<_number<<endl;
	printf("number is %d.\n",_number);
	if(_bscb)
		_bscb->ServerDo();
	}


};

#endif
