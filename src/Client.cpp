#include "../include/client.h"
#include "../include/socket.h"

char recvline[200],sendline[200];

int n = 0;



void BankCustomerClientICBC::ClientDo(char ip[20])
{
	printf("client start.\n");
	
	SocketBuilder *sBuilder = new SocketBuilder(PORT,ip);
	SocketDirector *sDirector = new SocketDirector(sBuilder);

	sDirector->construct(TCP_CLIENT);


	while(1)
	{
		printf("Input your words:");
		scanf("%s",sendline);
		
		write(sBuilder->_server_sockfd,sendline,strlen(sendline));

		printf("waiting for server...\n");

		n=read(sBuilder->_server_sockfd,recvline,100);
		recvline[n]='\0';
		
		printf("recv data is:%s\n",recvline);
	}
	
	sBuilder->_close();


}

