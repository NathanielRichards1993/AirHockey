#pragma once
#ifndef TCPSERVER_H
#define TCPSERVER_H
#include "stdafx.h"
#define DEFAULT_PORT "90000"
#define DEFAULT_BUFLEN 512
class TCP_Server
{
private:
	WSADATA	wsaData;
	char	recvbuf[DEFAULT_BUFLEN];
	int		recvbuflen;
	int		iResult;
	int iRcvdBytes;
	struct addrinfo hints;
	SOCKET ListenSocket;
	SOCKET ClientSocket1;
	SOCKET ClientSocket2;
	char* ip1;
	char* ip2;
	bool serverOnline;


public:
	TCP_Server();
	~TCP_Server();

	int intiServer();
	static DWORD WINAPI listenSocketLoop(LPVOID); //listen till message has been heard
	static DWORD WINAPI clientSocketLoop1(LPVOID);	//listen to the actual message
	static DWORD WINAPI clientSocketLoop2(LPVOID);	//listen to the actual message
	int sendClientSocket1(char*);
	int sendClientSocket2(char*);
	int shutdownServer();
};

#endif