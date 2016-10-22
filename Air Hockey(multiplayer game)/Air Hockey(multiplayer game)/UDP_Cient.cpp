#include"UDP_Client.h"

UDP_Client::UDP_Client()
{
	host = false;
	listenReady = false;
}
UDP_Client::~UDP_Client()
{
	delete(addressList);
}
void UDP_Client::setup()
{
	HANDLE* emptyhdlSend = NULL;
	HANDLE* fullhdlSend = NULL;
	HANDLE* emptyhdlListen = NULL;
	HANDLE* fullhdlListen = NULL;
	
	semaphore = (LPCTSTR)"ProducerSemaphoreSend";
	semaphore2 = (LPCTSTR)"ConsumerSemaphoreSend";
	semaphore3 = (LPCTSTR)"ProducerSemaphoreListen";
	semaphore4 = (LPCTSTR)"ConsumerSemaphoreListen";

	PNL_Listen = new vector<PlayerNewLocation*>();
	NR_Listen = new vector<NewRound*>();;
	CNRT_Listen = new vector<ConfirmNewRoundTime*>();
	SHW_Listen = new vector<SyncHitWall*>();
	OSP_Listen = new vector<OutOfSincPuck*>();

	//vector<Message_Preamble*> MP_Send;
	PNL_Send = new vector<PlayerNewLocation*>();
	NR_Send = new vector<NewRound*>();
	CNRT_Send = new vector<ConfirmNewRoundTime*>();
	SHW_Send = new vector<SyncHitWall*>();
	OSP_Send = new vector<OutOfSincPuck*>();


	mutexnameListen = (LPCTSTR)"listenMutex";
	mutexnameSend = (LPCTSTR)"sendMutex";	

	mutexhdlListen = CreateMutex(NULL,  // Access to the mutex object
                    FALSE,              // Inherit the handle?
                    mutexnameListen);         // Name the mutex to be opened

	mutexhdlSend = CreateMutex(NULL,  // Access to the mutex object
				FALSE,              // Inherit the handle?
				mutexnameSend);         // Name the mutex to be opened

	HANDLE  mutexhdlListen;
	HANDLE  mutexhdlSend;
	//MHP_Listen->push_back()
}
/*must call if you call setup*/
void UDP_Client::destroyList()
{
	//make sure to added if to check size and delete the stuff inside unless
	//some other code does it
	delete(PNL_Listen);
	delete(NR_Listen);
	delete(CNRT_Listen);
	delete(SHW_Listen);
	delete(OSP_Listen);
	delete(PNL_Send);
	delete(NR_Send);
	delete(CNRT_Send);
	delete(SHW_Send);
	delete(OSP_Send);
}

int UDP_Client::init()
{
	//int		recvbuflen = DEFAULT_BUFLEN;
	//SOCKET	UdpSocket = INVALID_SOCKET;

	//struct addrinfo *addressList = NULL;

	//ZeroMemory(&hints, sizeof(hints));
	//hints.ai_family = AF_UNSPEC;
	//hints.ai_socktype = SOCK_DGRAM;
	//hints.ai_protocol = IPPROTO_UDP;
	//WSADATA	wsaData;
	//int iResult = 0;

	//iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	//if (iResult != 0)
	//{
	//	printf("WSAStartup failed: %d\n", iResult);
	//	return 1;
	//}

	////server ip
	//iResult = getaddrinfo(ipNumber, DEFAULT_PORT, &hints, &addressList);

	//if (addressList != NULL)
	//{
	//	// Create a SOCKET for connecting to server
	//	UdpSocket = socket(addressList->ai_family,
	//		addressList->ai_socktype,
	//		addressList->ai_protocol);

	//	if (UdpSocket == INVALID_SOCKET)
	//	{
	//		printf("Error at socket(): %ld\n", WSAGetLastError());
	//		freeaddrinfo(addressList);
	//		WSACleanup();
	//		return 1;
	//	}
	//}
	return 0;
}

void UDP_Client::endClient()
{
	//freeaddrinfo(addressList);
}

DWORD WINAPI UDP_Client::listenSocketLoop(LPVOID parameter)
{
	UDP_Client* client = (UDP_Client*)parameter;
	int			iRcvdBytes = 0;
	sockaddr_in	rx_addr;
	sockaddr_in sender_addr;
	int			iSenderAddrSize = sizeof(sender_addr);
	SOCKET	UdpSocket1 = INVALID_SOCKET;

	HANDLE emptyhdlListen = OpenSemaphore(SYNCHRONIZE | SEMAPHORE_MODIFY_STATE, // Access to the semaphore object
		FALSE,                                  // Inherit the handle ? 
		client->semaphore3);                         // Names the semaphore to be opened

	HANDLE fullhdlListen = OpenSemaphore(SYNCHRONIZE | SEMAPHORE_MODIFY_STATE, // Access to the semaphore object
		FALSE,                                  // Inherit the handle ? 
		client->semaphore4);
	//int r = WaitForSingleObject(emptyhdlListen,INFINITE);

	HANDLE listenMutex = OpenMutex(MUTEX_ALL_ACCESS,NULL,client->mutexnameListen);
	
	//iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	//if (iResult != 0)
	//{
	//	printf("WSAStartup failed: %d\n", iResult);
	//	return 1;
	//}

	UdpSocket1 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (UdpSocket1 == INVALID_SOCKET)
	{
		printf("socket failed: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}
	u_short port = 0;
	if (client->host == true)
	{
		port = (u_short)DEFAULT_PORT;
	}
	else
	{
		port = (u_short)DEFAULT_PORT1;
	}
	int recvbuflen = 1024;
	char recvbuf[1024];
	//mutex
		rx_addr.sin_family = AF_INET;

		rx_addr.sin_port = htons(port);

		rx_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		
		int iResult = bind(UdpSocket1, (SOCKADDR *)&rx_addr, sizeof(rx_addr));
		if (iResult == SOCKET_ERROR)
		{
			printf("bind listen failed: %d\n", WSAGetLastError());
			closesocket(UdpSocket1);
			WSACleanup();
			return 1;
		}
		else
		{
			printf("works");
		}
	
		client->listenReady = true;


	while (1)
	{
		
		
		int iResult = recvfrom(UdpSocket1,
			recvbuf,
			recvbuflen,
			0,			// flags we don't need, so set to zero
			NULL,
			NULL);

		iResult = WaitForSingleObject(emptyhdlListen, 20);
			
		if (iResult == WAIT_OBJECT_0)
			//Do not know why but list is full
		{
			Message_Preamble preamble = Message_Preamble(0, 0);
			preamble.UnPackPreamble((unsigned char*)recvbuf);
			switch (preamble.getPreambleCode())
			{
				case PLAYERNEWLOCATION:
				{					
					//Take Mutex
					WaitForSingleObject(listenMutex,INFINITE);
					PlayerNewLocation* mes = new PlayerNewLocation();
					mes->UnPackPlayerNewLocation((unsigned char*)recvbuf);
					client->PNL_Listen->push_back(mes);
					ReleaseMutex(listenMutex);
					//mutex close
					printf("\n player new location read\n");
					break;
				}
				case NEWROUND:
				{
					//Take Mutex
					WaitForSingleObject(listenMutex,INFINITE);
					NewRound* mes = new NewRound();
					mes->UnPackNewRound((unsigned char*)recvbuf);
					client->NR_Listen->push_back(mes);
					ReleaseMutex(listenMutex);
					//mutex close
					printf("\n new round read\n");
					break;
				}
				case CONFIRMNEWROUND:
				{
					//Take Mutex
					WaitForSingleObject(listenMutex,INFINITE);
					ConfirmNewRoundTime* mes = new ConfirmNewRoundTime();
					mes->UnPackConfirmNewRoundTime((unsigned char*)recvbuf);
					client->CNRT_Listen->push_back(mes);
					ReleaseMutex(listenMutex);
					//mutex close
					printf("\n confirm new round read\n");
					break;
				}
				case SYNCHITWALL:
				{
					//Take Mutex
					WaitForSingleObject(listenMutex,INFINITE);
					SyncHitWall* mes = new SyncHitWall();
					int check = strlen(recvbuf);
					mes->UnPackSyncHitWall((unsigned char*)recvbuf);
					client->SHW_Listen->push_back(mes);
					ReleaseMutex(listenMutex);
					//Release Mutex			
					printf("\n sync wall hit read\n");
					break;
				}
				case OUTOFSYNC:
				{
					//Take Mutex
					WaitForSingleObject(listenMutex,INFINITE);
					OutOfSincPuck* mes = new OutOfSincPuck();
					mes->UnPackPuckLocation((unsigned char*)recvbuf);
					client->OSP_Listen->push_back(mes);
					ReleaseMutex(listenMutex);
					//Release Mutex
					printf("\n out of sync read\n");
					break;
				}
				case SINCPUCK:
				{
					//WaitForSingleObject(listenMutex,INFINITE);
					//SINCPUCK* mes = new OutOfSincPuck
					//ReleaseMutex(listenMutex);
					break;
				}
				case SHUTDOWN:
				{
				}
			}

			if (iResult == SOCKET_ERROR)
			{
				printf("recv failed: %d\n", WSAGetLastError());
				closesocket(UdpSocket1);
				WSACleanup();
				return 1;
			}
			printf("Bytes received: %d Msg=%s\n", iResult, recvbuf);

			ReleaseSemaphore(fullhdlListen, 1, NULL);
		}
		else
		{
			printf("this system is not balanced");
		}
		
		Sleep(23);
		if (client->terminateClient == true)
		{
			break;
		}
		//very dangerous becarefull
		//test to make sure no condition were list exist after termination
		//basically make sure the game deletes the list correctly
	}
		
}

DWORD WINAPI UDP_Client::sendSocketLoop(LPVOID parameter)
{
	UDP_Client* client = (UDP_Client*)parameter;
	
	WSADATA	wsaData;
	char	recvbuf[DEFAULT_BUFLEN];
	int		iResult;
	bool recieveMessage = false;
	int		recvbuflen = DEFAULT_BUFLEN;
	SOCKET	UdpSocket = INVALID_SOCKET;

	struct addrinfo hints;

	struct addrinfo *addressList = NULL;

	HANDLE emptyhdlSend = OpenSemaphore(SYNCHRONIZE | SEMAPHORE_MODIFY_STATE, // Access to the semaphore object
										FALSE,                                  // Inherit the handle ? 
										client->semaphore);                         // Names the semaphore to be opened

	HANDLE fullhdlSend = OpenSemaphore(SYNCHRONIZE | SEMAPHORE_MODIFY_STATE, // Access to the semaphore object
										FALSE,                                  // Inherit the handle ? 
										client->semaphore2);

	unsigned char sendbuf[1024];
	int buflen = 0;

	HANDLE sendMutex = OpenMutex(MUTEX_ALL_ACCESS,NULL,client->mutexnameSend);

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;

	char port[256];
	//server ip
	if (client->host == true)
	{
		iResult = getaddrinfo(client->ipNumber, (PCSTR)DEFAULT_PORT1STR, &hints, &addressList);
	}
	else
	{
		iResult = getaddrinfo(client->ipNumber, (PCSTR)DEFAULT_PORTSTR, &hints, &addressList);
	}
	if ( iResult != 0 ) 
	{
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return 1;
	}
	if (addressList != NULL)
	{
		// Create a SOCKET for connecting to server
		UdpSocket = socket(addressList->ai_family,
			addressList->ai_socktype,
			addressList->ai_protocol);

		if (UdpSocket == INVALID_SOCKET)
		{
			printf("Error at socket(): %ld\n", WSAGetLastError());
			freeaddrinfo(addressList);
			WSACleanup();
			return 1;
		}
	}

	//samething as listen the mutex shouldn't be a problem since it is only copying stuff


	while (1)//loop
	{
		Sleep(20);
		
			iResult = 0;
			iResult = WaitForSingleObject(fullhdlSend, 20);//dec

			if (iResult == WAIT_OBJECT_0)
			{
				Sleep(100);
				buflen = 0;		
				//change from infinite wait to a with failure
				//Take Mutex
				int iResult = WaitForSingleObject(sendMutex,INFINITE); 
				if(iResult == WAIT_OBJECT_0)
				{
					// sending doen't delete pointers because it would delete
					//the original
					if (client->PNL_Send->size() > 0)
					{
						PlayerNewLocation* mes = client->PNL_Send->at(0);	
						buflen = mes->PackPlayerNewLocation(sendbuf);
						Message_Preamble preamb = Message_Preamble(PLAYERNEWLOCATION,buflen);
						preamb.PackPreamble(sendbuf);
				
						client->PNL_Send->erase(client->PNL_Send->begin());
						ReleaseMutex(sendMutex);

						PlayerNewLocation* ma = new PlayerNewLocation();
						ma->UnPackPlayerNewLocation(sendbuf);

						delete(mes);
						//Release Mutex 
						printf("\n new player location send\n");
					}
					else if (client->NR_Send->size() > 0)
					{
						NewRound* mes = client->NR_Send->at(0);
				
						buflen = mes->PackNewRound(sendbuf);
						Message_Preamble preamb = Message_Preamble(NEWROUND, buflen);
						preamb.PackPreamble(sendbuf);

						client->NR_Send->erase(client->NR_Send->begin());
						ReleaseMutex(sendMutex);
					
						delete(mes);
						//Release Mutex
						printf("\n new round send\n");
					}
					else if (client->CNRT_Send->size() > 0)
					{
						ConfirmNewRoundTime* mes = client->CNRT_Send->at(0);

						buflen = mes->PackConfirmNewRoundTime(sendbuf);
						Message_Preamble preamb = Message_Preamble(CONFIRMNEWROUND,buflen);
				
						preamb.PackPreamble(sendbuf);
				
						client->CNRT_Send->erase(client->CNRT_Send->begin());
						ReleaseMutex(sendMutex);

						delete(mes);
						//Release Mutex
						printf("\nconfirm new round send\n");
					}
					else if (client->SHW_Send->size() > 0)
					{
						SyncHitWall* mes = client->SHW_Send->at(0);
				
						buflen = mes->PackSyncHitWall(sendbuf);
						Message_Preamble preamb = Message_Preamble(SYNCHITWALL,buflen);
						preamb.PackPreamble(sendbuf);

						client->SHW_Send->erase(client->SHW_Send->begin());
						ReleaseMutex(sendMutex);

						SyncHitWall* ma = new SyncHitWall();
						ma->UnPackSyncHitWall(sendbuf);

						delete(mes);

						//Release Mutex
						printf("\nhit wall sync send\ns");
					}
					else if (client->OSP_Send->size() > 0)
					{
						OutOfSincPuck* mes = client->OSP_Send->at(0);
						buflen = mes->PackPuckLocation(sendbuf);
						Message_Preamble preamb = Message_Preamble(OUTOFSYNC, buflen);				
						preamb.PackPreamble(sendbuf);

						client->OSP_Send->erase(client->OSP_Send->begin());
						ReleaseMutex(sendMutex);
						delete(mes);
						//Release Mutex
						printf("\n out of sync send \n");
					}
					else
					{
						printf("blank message who touched my memory");
					}
					//Hope for no boom
					//addrinfo addr = *client->addressList;
					if(buflen != 0)
					{
						printf("Sending %i bytes\n", buflen);
						iResult = sendto(UdpSocket,
							(char *)sendbuf,
							(int)(buflen+1),
							0,							// flags - we never need them so zero them
							addressList->ai_addr,
							(int)addressList->ai_addrlen);

						if (iResult == SOCKET_ERROR)
						{
							printf("send failed: %d\n", WSAGetLastError());
							freeaddrinfo(addressList);
							closesocket(UdpSocket);
							WSACleanup();
							return 1;
						}
						ReleaseSemaphore(emptyhdlSend, 1, NULL);//inc
					}
					else
					{
						//memory update problem
						printf("\nmemory update error(send 0 size)\n 1 message is not a problem >1 is tho\n");
						//Release Mutex if update didn't go through
						ReleaseMutex(sendMutex);
						//this will keep the check of the memory going
						ReleaseSemaphore(fullhdlSend,1,NULL);
					}

					printf("Bytes Sent: %ld\n", iResult);

				}
			}
			else{}//could put waiting message in


	}
}


void UDP_Client::setIp(char* ip)
{
	sprintf(ipNumber, "");
	strcpy(ipNumber, ip);
}
/**/
void UDP_Client::setHandles(HANDLE* sEmpty, HANDLE* sFull,HANDLE* lEmpty,HANDLE* lFull)
{
	//emptyhdlSend = sEmpty;
	//fullhdlSend = sFull;
	//emptyhdlListen = lEmpty;
	//fullhdlListen = lFull;
}
/*does not work*/
void UDP_Client::setSemaphoresName(LPCTSTR* sEmpty, LPCTSTR* sFull, LPCTSTR* lEmpty, LPCTSTR* lFull)
{
//	clientUDP->semaphore = sEmpty;
//	clientUDP->semaphore2 = sFull;
//	//listening
//	clientUDP->semaphore3 = lEmpty;
//	clientUDP->semaphore4 = lFull;
}

void UDP_Client::isHost()
{
	host = true;
}

void UDP_Client::notHost()
{
	host = false; //should already be false
}

bool UDP_Client::listEmpty()
{
	//listen part
	if(PNL_Listen->size() > 0)
		return false;
	if(NR_Listen->size() > 0)
		return false;
	if(CNRT_Listen->size() > 0)
		return false;
	if(SHW_Listen->size() > 0)
		return false;
	if(OSP_Listen > 0)
		return false;

	//send part
	if(PNL_Listen->size() > 0)
		return false;
	if(NR_Send->size() > 0)
		return false;
	if(CNRT_Listen->size() > 0)
		return false;
	if(SHW_Listen->size() > 0)
		return false;
	if(OSP_Send->size() > 0)
		return false;

	return true;
}
