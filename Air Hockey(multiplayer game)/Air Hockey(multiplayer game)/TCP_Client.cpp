#include"TCP_Client.h"

TCP_Client::TCP_Client()
{
	sprintf(ipNumber, "");
	strcpy(ipNumber,"127.0.0.1");
	terminateClient = false;
}
TCP_Client::TCP_Client(TCHAR* hostIp)
{
	//ipNumber = hostIp;
	terminateClient = false;
}
TCP_Client::~TCP_Client()
{
}

int TCP_Client::initClient()
{

	int	iResult;
	struct addrinfo *addressList = NULL;
	struct addrinfo *ptr = NULL;

	
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}


	ConnectSocket = INVALID_SOCKET;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;	//Data stram
	hints.ai_protocol = IPPROTO_TCP;
	TCHAR* name = ipNumber;
	//resolve the ip address of the server we want to connect to
	iResult = getaddrinfo(ipNumber, DEFAULT_PORT, &hints, &addressList);

	if (iResult != 0)
	{
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	for (ptr = addressList; ptr != NULL; ptr = ptr->ai_next)
	{
		//Create socket to connect to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

		if (ConnectSocket == INVALID_SOCKET)
		{
			printf("Error at socket(): %ld\n", WSAGetLastError());
			freeaddrinfo(addressList);
			WSACleanup();
			return 1;
		}
		//active open:connect to server
		//block until server acks
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR)
		{
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}
	freeaddrinfo(addressList);

	if (ConnectSocket == INVALID_SOCKET)
	{
		printf("Unable to connect to server!: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	// send message was here

	//recieve here

	return 0;
	//remember that this could might work but there is no 
	//mangment of the data bounderies for recieving and also 
	//tcp does not always recieve in order so fix.
}

int TCP_Client::sendMessageToserver(char* message)
{
	int iResult = 0;


	sprintf(sendbuf, "%s %i", message, clock());
	//send data to server
	iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf) + 1, 0);

	printf("Bytes sent: %ld\n", iResult);

	//shutdown the client side of tyhe connection because 
	//we have finihsed sending all the data we want to
	iResult = shutdown(ConnectSocket, SD_SEND);

	return 0;
}

DWORD WINAPI TCP_Client::recieveMessageFromServer(LPVOID parameter)
{
	TCP_Client* client = (TCP_Client*)parameter;
	int iResult = 0;
	int recvbuflen = DEFAULT_BUFLEN;
	//receive until server closes the connection
	do
	{
		iResult = recv(client->ConnectSocket, client->recvbuf, recvbuflen, 0);
		if (iResult > 0)
			printf("Bytes received: %d Msg=%s\n", iResult, client->recvbuf);
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed: %d\n", WSAGetLastError());
		return 1;

	} while (iResult > 0);
	return 0;
}

int TCP_Client::endClient()
{
	closesocket(ConnectSocket);
	//}
	shutdown(ConnectSocket, SD_SEND);
	//printf("\r\nHit key to exit");
	//while (!_kbhit());

	//WSACleanup();

	return 0;
}

void TCP_Client::setIp(char* ip)
{
	sprintf(ipNumber, "");
	strcpy(ipNumber, ip);
}

void TCP_Client::getMessage(char* message)
{
	strcpy(message, recvbuf);
}