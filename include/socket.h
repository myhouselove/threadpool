#ifndef __SOCKET_H_INCLUDED__
#define __SOCKET_H_INCLUDED__


#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <string.h>

#define MYPORT  8887
#define QUEUE   20
#define BUFFER_SIZE 1024

/*
udp or tcp,server or client.

*/
typedef enum
{
TCP_SERVER,
TCP_CLIENT,
UDP_SERVER,
UDP_CLIENT
}SERVER_OR_CLIENT_T;

class SocketBuilder
{
    
public:
   
    ///定义sockfd
    int _server_sockfd;
	///定义sockaddr_in
    struct sockaddr_in _server_sockaddr;
	int _port;
	int _client_sockfd;
	char _ip_addr[20];

public:
	SocketBuilder(int port):_port(port){
	
	}
	SocketBuilder(int port,char ip_addr[20]):_port(port){
		strcpy(_ip_addr,ip_addr);
	}
	~SocketBuilder(){
		_close();
	}
    
	//virtual start(){}

	 void _initCommon()
	{
		_server_sockaddr.sin_family = AF_INET;
	    _server_sockaddr.sin_port = htons(_port);
	    _server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
			
	}
	void _initClient()
	{
		_server_sockaddr.sin_family = AF_INET;
    	_server_sockaddr.sin_port = htons(_port);
		if( inet_pton(AF_INET, _ip_addr, &_server_sockaddr.sin_addr) <= 0){	
			printf("inet_pton error for %s\n",_ip_addr);  
			exit(0);  
		} 

	}
 
	void build_socket()
	{
		_server_sockfd = socket(AF_INET,SOCK_STREAM, 0);
		
	}

	void build_bind()
	{
		///bind，成功返回0，出错返回-1
	   if(bind(_server_sockfd,(struct sockaddr *)&_server_sockaddr,sizeof(_server_sockaddr))==-1)
		{
			perror("bind");
			exit(1);
		}
	}
	void build_listen()
	{
	    ///listen，成功返回0，出错返回-1
	    if(listen(_server_sockfd,QUEUE) == -1)
	    {
	        perror("listen");
	        exit(1);
	    }
	}

	void build_accept()
	{
	    struct sockaddr_in client_addr;
	    socklen_t length = sizeof(client_addr);

	    ///成功返回非负描述字，出错返回-1
	    _client_sockfd = accept(_server_sockfd, (struct sockaddr*)&client_addr, &length);
	    if(_client_sockfd < 0)
	    {
	        perror("connect");
	        exit(1);
	    }
	}
	void build_connect()
	{
		int err=connect(_server_sockfd,(struct sockaddr *)&_server_sockaddr,sizeof(_server_sockaddr));
		if(err==-1)
		{
			printf("connect error\n");
			//return -1;
		}
	}
	
   void _close()
	{
		close(_client_sockfd);
	    close(_server_sockfd);
	}

};

class SocketDirector
{
	SocketBuilder *_mBuilder = NULL;

	public:
		SocketDirector(SocketBuilder *mBuilder){
			_mBuilder = mBuilder;
		}
	void construct(SERVER_OR_CLIENT_T soc){
		switch(soc)
			{
		case TCP_SERVER:
			_mBuilder->_initCommon();
			_mBuilder->build_socket();
			_mBuilder->build_bind();
			_mBuilder->build_listen();
			//_mBuilder->build_accept();
			
			break;
		case UDP_SERVER:
			_mBuilder->_initCommon();
			_mBuilder->build_socket();
			_mBuilder->build_bind();
			
			break;
		case TCP_CLIENT:
			_mBuilder->_initClient();
			_mBuilder->build_socket();
			_mBuilder->build_connect();
			
			break;
		case UDP_CLIENT:
			_mBuilder->_initClient();
			_mBuilder->build_socket();
			break;
		default:
			printf("please input correct protocols.\n");
			break;
		}
			

	}


};
#endif
