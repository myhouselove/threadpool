#include "../include/server.h"
#include "../include/socket.h"

char recvline[200],sendline[200];

int n = 0;

void ICBCServerCenter::ServerDo()
{
	SocketBuilder *sBuilder = new SocketBuilder(PORT);
	SocketDirector *sDirector = new SocketDirector(sBuilder);

	sDirector->construct(TCP_SERVER);

	while(1)
	{
		sBuilder->build_accept();

		while(1)
		{
			printf("waiting for client...\n");
			n=read(sBuilder->_client_sockfd,recvline,1024);
			if(n==-1)
			{
				printf("recv error\n");
			}
			recvline[n]='\0';
			
			printf("recv data is:%s\n",recvline);
			
			printf("Input your words:");
			scanf("%s",sendline);
			write(sBuilder->_client_sockfd,sendline,strlen(sendline));	
		}
	}
}





