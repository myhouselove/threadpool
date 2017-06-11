#ifndef __CLIENT_H_INCLUDED__
#define __CLIENT_H_INCLUDED__

#define MAX_LENTH (10)
#define PORT (6666)

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "../include/thread.h"

class BankCustomerClientBase
{
	public:
		BankCustomerClientBase(){printf("this is a bank client!\n");}
		virtual ~BankCustomerClientBase(){}
	virtual void ClientDo(char ip[20]){}


};


class BankCustomerClientICBC : public BankCustomerClientBase
{
	public:
		BankCustomerClientICBC(){}

	void ClientDo(char ip[20]);


};

class ICBCClientThread : public Thread
{
	private:
		BankCustomerClientBase *_bccb;
		char _ip[20];
		void (*_p_func_t)();
	public:
		ICBCClientThread(char ip[20],BankCustomerClientBase *bccb,void (*p_func_t)() ):_bccb(bccb),_p_func_t(p_func_t)
			{
			strcpy(_ip,ip);
			start();
		}

		void run()
		{
			printf("run here.\n");
			_bccb->ClientDo(_ip);
		}
	


};





#endif
