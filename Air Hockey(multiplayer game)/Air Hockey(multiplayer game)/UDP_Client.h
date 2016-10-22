#pragma once
#ifndef UDPCLIENT_H
#define UDPCLIENT_H
#include"ConfirmNewRoundTime.h"
#include"Message_Preamble.h"
#include"NewRound.h"
#include"OutOfSincPuck.h"
#include"SyncHitWall.h"
#include"PlayerNewLocation.h"
#include"sincPuck.h"
#include"ShutDown.h"
#include"stdafx.h"


//listening
#define DEFAULT_PORT 85000
#define DEFAULT_PORT1 90000

//sending
#define DEFAULT_PORTSTR "85000"
#define DEFAULT_PORT1STR "90000"

#define DEFAULT_BUFLEN 1024
#define PREAMBLE_LENGTH 4
#define PLAYERNEWLOCATION 1
#define NEWROUND 2
#define CONFIRMNEWROUND 3
#define SYNCHITWALL 4
#define OUTOFSYNC 5
#define SINCPUCK 6
#define SHUTDOWN 7


using namespace std;
class UDP_Client
{
private:
	char ipNumber[128];
	bool host;

	HANDLE emptyhdlSend;
	HANDLE fullhdlSend;
	HANDLE emptyhdlListen;
	HANDLE fullhdlListen;

	bool terminateClient;

	struct addrinfo *addressList;
	struct addrinfo hints;

	//std::vector<Message_Preamble> *MP_Listen;
	
public:

	bool listenReady;


	vector<PlayerNewLocation*>* PNL_Listen;
	vector<NewRound*>* NR_Listen;
	vector<ConfirmNewRoundTime*>* CNRT_Listen;
	vector<SyncHitWall*>* SHW_Listen;
	vector<OutOfSincPuck*>* OSP_Listen;

	vector<PlayerNewLocation*>* PNL_Send;
	vector<NewRound*>* NR_Send;
	vector<ConfirmNewRoundTime*>* CNRT_Send;
	vector<SyncHitWall*>* SHW_Send;
	vector<OutOfSincPuck*>* OSP_Send;

	HANDLE  mutexhdlListen;
	LPCTSTR mutexnameListen;
	HANDLE  mutexhdlSend;
	LPCTSTR mutexnameSend;

	LPCTSTR semaphore;
	LPCTSTR semaphore2;
	//listening
	LPCTSTR semaphore3;
	LPCTSTR semaphore4;

	~UDP_Client();
	UDP_Client();
	int init();
	void endClient();
	void setup();
	void destroyList();
	
	static DWORD WINAPI listenSocketLoop(LPVOID); //listen till message has been heard
	static DWORD WINAPI sendSocketLoop(LPVOID);

	bool listEmpty();

	void setIp(char*);
	/*empty full (send listen)*/
	void setHandles(HANDLE*, HANDLE*,HANDLE*,HANDLE*);
	void setSemaphoresName(LPCTSTR*, LPCTSTR*, LPCTSTR*, LPCTSTR*);
	void isHost();
	void notHost();


};

#endif