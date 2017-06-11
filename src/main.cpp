#include <iostream>
using namespace std;

#include "../include/server.h"

int main(int argc,char *argv[])
{
	
	BankServerCenterBase *bs = new ICBCServerCenter();
	
	Thread *thread = new ICBCServerThread(1003,bs);
	thread->join();
	cout<<"first test!"<<endl;
	return 0;
}
