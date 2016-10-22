#pragma once
#ifndef TCPCLIENT_H
#define TCPCLIENT_H
#include "stdafx.h"
#include <iostream>
#define DEFAULT_PORT "90000"
#define DEFAULT_BUFLEN 512
class TCP_Client
{
private:

	WSADATA	wsaData;
	char	recvbuf[DEFAULT_BUFLEN];

	struct addrinfo *addressList;
	struct addrinfo *ptr;
	struct addrinfo hints;

	SOCKET ConnectSocket;

	char sendbuf[128];//message can be bigger than recieve buf as long as 
	//the recieving of the message is handled correctly
	char messageArray[20];//message storage shouldn't need to be big
	char ipNumber[128];
	bool terminateClient;

	bool messageRecieved;
public:
	TCP_Client();
	/*host ip number*/
	TCP_Client(TCHAR*);
	~TCP_Client();

	int initClient();
	int sendMessageToserver(char*);
	/*recieve message is only done after sending because of block*/
	static DWORD WINAPI recieveMessageFromServer(LPVOID);
	//messages to recieve are no response from server and other clients ip is (insert ip).
	void terminate();	//only really affecting listen because send will send message anyway.
	int endClient();

	void setIp(char*);
	void getMessage(char*);
};

#endif