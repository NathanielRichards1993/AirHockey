#include"TCP_Server.h"
#include<iostream>

TCP_Server::TCP_Server()
{
	recvbuflen = DEFAULT_BUFLEN;
}

TCP_Server::~TCP_Server()
{
}

int TCP_Server::intiServer()
{
	int		iResult;
	int		recvbuflen = DEFAULT_BUFLEN;

	struct addrinfo *addressList = NULL;
	struct addrinfo *ptr = NULL;

	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) 
	{
		printf("WSAStartup failed: %d\n server", iResult);
		return 1;
	}

    printf("Starting server...\r\n");
	this->serverOnline = true;

    ListenSocket = INVALID_SOCKET;
    ClientSocket1 = INVALID_SOCKET;
	ClientSocket2 = INVALID_SOCKET;

	int iRcvdBytes = 0;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &addressList);
	if (iResult != 0)
	{
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return 1;
	}


	// Create a SOCKET for connecting to server
	ListenSocket = socket(addressList->ai_family, addressList->ai_socktype, addressList->ai_protocol);
	if (ListenSocket == INVALID_SOCKET)
	{
		printf("socket failed: %ld\n", WSAGetLastError());
		freeaddrinfo(addressList);
		WSACleanup();
		return 1;
	}


	BOOL bOptVal = TRUE;
    int bOptLen = sizeof(BOOL);

	if (setsockopt(ListenSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&bOptVal, bOptLen) == SOCKET_ERROR)
	{
		printf("setsockopt failed: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	iResult = bind( ListenSocket, addressList->ai_addr, (int)addressList->ai_addrlen);
	if (ListenSocket == INVALID_SOCKET)
	{
		printf("socket failed: %ld\n", WSAGetLastError());
		freeaddrinfo(addressList);
		WSACleanup();
		return 1;

	}
    

    freeaddrinfo(addressList);
}

DWORD WINAPI TCP_Server::listenSocketLoop(LPVOID parameter)//in its own thread an launches its own threads
{
	TCP_Server* ptr = (TCP_Server*)parameter;
	int iResult = 0;

	HANDLE threadhdl[2];

	std::cout << ptr->serverOnline;
	//printf("server is %i", ptr->serverOnline);
	sockaddr_in client_info = { 0 };
	int addrsize = sizeof(client_info);

	while(ptr->serverOnline == true)
	{
		iResult = 0;
		printf("\r\nListening for connection...");

		iResult = listen(ptr->ListenSocket, SOMAXCONN);
		if (iResult == SOCKET_ERROR) 
		{
			printf("listen failed ls1: %d\n", WSAGetLastError());
			closesocket(ptr->ListenSocket);
			ptr->serverOnline = false;
			WSACleanup();
			return 1;
		}
		if (ptr->ClientSocket1 == INVALID_SOCKET)
		{
			ptr->ClientSocket1 = accept(ptr->ListenSocket, (struct sockaddr*)&client_info, &addrsize);
			if (ptr->ClientSocket1 == INVALID_SOCKET)
			{
				printf("accept failed cs1: %d\n", WSAGetLastError());
				closesocket(ptr->ListenSocket);
				ptr->serverOnline = false;
				WSACleanup();
				return 1;
			}
			//getpeername(ptr->ListenSocket, (struct sockaddr*)&client_info, (int*)sizeof(client_info));
			//strcpy(ptr->ip1,(inet_ntoa(client_info.sin_addr)));
			DWORD threadid;
			threadhdl[0] = CreateThread(NULL,
				0, ptr->clientSocketLoop1,
				(LPVOID)ptr, 0, &threadid);
			getpeername(ptr->ClientSocket1, (struct sockaddr*)&client_info, (int*)sizeof(client_info));
			ptr->ip1 = (inet_ntoa(client_info.sin_addr));
			//strcpy(ptr->ip1, (char*)));

		}
		else if (ptr->ClientSocket2 == INVALID_SOCKET)
		{
			ptr->ClientSocket2 = accept(ptr->ListenSocket, (struct sockaddr*)&client_info, &addrsize);
			if (ptr->ClientSocket2 == INVALID_SOCKET)
			{
				printf("accept failed cs2: %d\n", WSAGetLastError());
				closesocket(ptr->ListenSocket);
				ptr->serverOnline = false;
				WSACleanup();
				return 1;
			}
			DWORD threadid;
			threadhdl[1] = CreateThread(NULL,
				0, clientSocketLoop1,
				(LPVOID)ptr, 0, &threadid);
			getpeername(ptr->ClientSocket1, (struct sockaddr*)&client_info, (int*)sizeof(client_info));
			ptr->ip2 = (inet_ntoa(client_info.sin_addr));
		}

		if (ptr->ClientSocket2 != INVALID_SOCKET)
			//if the 2nd socket has connected need to wait for the threads handling the messages to end
			//messages are terminating
		{

			WaitForMultipleObjects(
				2,     // Number of thread handles in array
				threadhdl,      // Array of handles
				TRUE,           // Wait for all threads
				INFINITE);      // Wait forever, 0=test and return immediately, 

			CloseHandle(threadhdl[0]);
			CloseHandle(threadhdl[1]);

			if (ptr->serverOnline == false)
			{
				ptr->sendClientSocket1("Server is going offline");
				ptr->sendClientSocket2("Server is going offline");
			}
			else
			{
				ptr->sendClientSocket1("You are player 1");
				ptr->sendClientSocket2("You are player 2");
			}
			ptr->shutdownServer();
		}
	}
}

DWORD WINAPI TCP_Server::clientSocketLoop1(LPVOID parameter)
{
	TCP_Server* ptr = (TCP_Server*)parameter;
	int iResult = NULL;
	do
	{
		// Note: This bit of code is "broken".
		// This code overwrites the contents of the buffer
		// each time around the loop.
		// Depending on your application, you should either
		// keep a pointer into the buffer which is updated
		// to the next position at which data should be
		// stored in the buffer or create a new buffer
		// each time you receive more data.
		//
		
		iResult = recv(ptr->ClientSocket1, ptr->recvbuf,
			ptr->recvbuflen, 0);
		if (iResult > 0)
		{
			printf("Bytes received: %d Msg=%s\n", iResult, ptr->recvbuf);
			//iRcvdBytes = iResult;
		}
		else if (iResult == 0)
			printf("Connection closing...\n");
		else
		{
			printf("recv failed incs1: %d\n", WSAGetLastError());
			closesocket(ptr->ClientSocket1);
			ptr->serverOnline = false;
			WSACleanup();
			return 1;
		}

	} while (iResult > 0 && ptr->serverOnline == true);
	//so if recieve is not finished && server is not being told to shutdown
}

DWORD WINAPI TCP_Server::clientSocketLoop2(LPVOID parameter)
{
	TCP_Server* TCP_Serverptr = (TCP_Server*)parameter;
	int iResult = 0;
	do
	{
		// Note: This bit of code is "broken".
		// This code overwrites the contents of the buffer
		// each time around the loop.
		// Depending on your application, you should either
		// keep a pointer into the buffer which is updated
		// to the next position at which data should be
		// stored in the buffer or create a new buffer
		// each time you receive more data.
		//
		iResult = recv(TCP_Serverptr->ClientSocket2, TCP_Serverptr->recvbuf,
			TCP_Serverptr->recvbuflen, 0);
		if (iResult > 0)
		{
			printf("Bytes received: %d Msg=%s\n", iResult, TCP_Serverptr->recvbuf);
			//iRcvdBytes = iResult;
		}
		else if (iResult == 0)
			printf("Connection closing...\n");
		else
		{
			printf("recv failed incs2: %d\n", WSAGetLastError());
			closesocket(TCP_Serverptr->ClientSocket1);
			TCP_Serverptr->serverOnline = false;
			WSACleanup();
			return 1;
		}

	} while (iResult > 0 && TCP_Serverptr->serverOnline == true);
	//so if recieve is not finished && server is not being told to shutdown
}

int TCP_Server::sendClientSocket1(char* message)
{
	int iResult;
	char sendbuf[DEFAULT_BUFLEN];
	sprintf(sendbuf, message);
	int buflen = strlen(sendbuf)+1;//+1 for null

	iResult = send(ClientSocket1, sendbuf, buflen, 0);
	if (iResult == SOCKET_ERROR)
	{
		printf("send failed: %d\n", WSAGetLastError());
		closesocket(ClientSocket1);
		WSACleanup();
		return 1;
	}

	printf("Bytes Sent: %ld\n", iResult);

	// shutdown the connection since we're done
	// shutdown() acts like a close() but waits for the receiver
	// to ACK the last data sent by the client.
	// SD_SEND = no more sends allowed
	// For TCP sockets, a FIN will be sent after all data is sent 
	// and acknowledged by the receiver.
	// Other options are SD_RECEIVE and SD_BOTH.
	//
	iResult = shutdown(ClientSocket1, SD_SEND);
	if (iResult == SOCKET_ERROR)
	{
		printf("shutdown failed: %d\n", WSAGetLastError());
		closesocket(ClientSocket1);
		WSACleanup();
		return 1;
	}

}

int TCP_Server::sendClientSocket2(char* message)
{
	int iResult;
	char sendbuf[DEFAULT_BUFLEN];
	sprintf(sendbuf, message);
	int buflen = strlen(sendbuf) + 1;//+1 for null

	iResult = send(ClientSocket2, sendbuf, buflen, 0);
	if (iResult == SOCKET_ERROR)
	{
		printf("send failed: %d\n", WSAGetLastError());
		closesocket(ClientSocket2);
		WSACleanup();
		return 1;
	}

	printf("Bytes Sent: %ld\n", iResult);

	// shutdown the connection since we're done
	// shutdown() acts like a close() but waits for the receiver
	// to ACK the last data sent by the client.
	// SD_SEND = no more sends allowed
	// For TCP sockets, a FIN will be sent after all data is sent 
	// and acknowledged by the receiver.
	// Other options are SD_RECEIVE and SD_BOTH.
	//
	iResult = shutdown(ClientSocket2, SD_SEND);
	if (iResult == SOCKET_ERROR)
	{
		printf("shutdown failed: %d\n", WSAGetLastError());
		closesocket(ClientSocket2);
		WSACleanup();
		return 1;
	}

}

int TCP_Server::shutdownServer()
{
	// cleanup
	closesocket(ClientSocket1);
	closesocket(ClientSocket2);
	closesocket(ListenSocket);

	shutdown(ListenSocket, SD_SEND);
	shutdown(ClientSocket1, SD_SEND);
	shutdown(ClientSocket2, SD_SEND);

	serverOnline = false;

	return 0;
}


//int _tmain(int argc, _TCHAR* argv[])
//{
//
//
//	// Initialize Winsock
//	// The WSAStartup function is called to initiate use of WS2_32.lib.
//	// The WSADATA structure contains information about the Windows Sockets 
//	// implementation. The MAKEWORD(2,2) parameter of WSAStartup makes a 
//	// request for the version of Winsock on the system, and sets the passed 
//	// version as the highest version of Windows Sockets support that the 
//	// caller can use.
//	//
//	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
//	if (iResult != 0) 
//	{
//		printf("WSAStartup failed: %d\n", iResult);
//		return 1;
//	}
//
//    printf("Starting server...\r\n");
//
//    // For a server to accept client connections, it must be bound to a 
//    // network address within the system. The following code demonstrates 
//    // how to bind a socket that has already been created to an 
//    // IP address and port. Client applications use the IP address and 
//    // port to connect to the host network.
//    // The sockaddr structure holds information regarding the address family,
//    // IP address, and port number.
//    //
//    SOCKET ListenSocket = INVALID_SOCKET;
//    SOCKET ClientSocket = INVALID_SOCKET;
//
//    int iRcvdBytes = 0;
//
//    // ****************************************
//    // After initialization, a SOCKET object must be instantiated
//    // ****************************************
//
//    // Call the getaddrinfo function requesting the IP address for this
//    // server. The application requests the socket type to be a 
//    // stream socket for the TCP protocol.
//    //
//    // The getaddrinfo function is used to determine the values in the 
//    // sockaddr structure:
//    // - AF_INET is used to specify the IPv4 address family. 
//    // - SOCK_STREAM is used to specify a stream socket. 
//    // - IPPROTO_TCP is used to specify the TCP protocol . 
//    // - AI_PASSIVE flag indicates the caller intends to use the returned 
//    // socket address structure in a call to the bind function. 
//    // When the AI_PASSIVE flag is set and nodename parameter to the 
//    // getaddrinfo function is a NULL pointer, the IP address portion 
//    // of the socket address structure is set to INADDR_ANY for 
//    // IPv4 addresses or IN6ADDR_ANY_INIT for IPv6 addresses. 
//    // - 27015 is the port number associated with the server that the client will connect to. 
//    //
//    // Returns a linked list of addresses, the memory for which must be
//    // freed by calling freeaddrinfo() when finished with.
//    //
//    ZeroMemory(&hints, sizeof(hints));
//    hints.ai_family = AF_INET;
//    hints.ai_socktype = SOCK_STREAM;
//    hints.ai_protocol = IPPROTO_TCP;
//    hints.ai_flags = AI_PASSIVE;
//
//    // Resolve the server address and port
//    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &addressList);
//    if ( iResult != 0 ) 
//    {
//        printf("getaddrinfo failed: %d\n", iResult);
//        WSACleanup();
//        return 1;
//    }
//
//    // Create a SOCKET for connecting to server
//    ListenSocket = socket(addressList->ai_family, addressList->ai_socktype, addressList->ai_protocol);
//    if (ListenSocket == INVALID_SOCKET) 
//    {
//        printf("socket failed: %ld\n", WSAGetLastError());
//        freeaddrinfo(addressList);
//        WSACleanup();
//        return 1;
//    }
//
//    BOOL bOptVal = TRUE;
//    int bOptLen = sizeof(BOOL);
//
//    if (setsockopt(ListenSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&bOptVal, bOptLen) == SOCKET_ERROR)
//    {
//        printf("setsockopt failed: %d\n", WSAGetLastError());
//        closesocket(ListenSocket);
//        WSACleanup();
//        return 1;
//    }
//
//    // Call the bind function (associate socket with local address), 
//    // passing the created socket and sockaddr structure 
//    // returned from the getaddrinfo function as parameters. 
//    // Check for general errors.
//    //
//    // Setup the TCP listening socket
//    //
//    iResult = bind( ListenSocket, addressList->ai_addr, (int)addressList->ai_addrlen);
//    if (iResult == SOCKET_ERROR) 
//    {
//        printf("bind failed: %d\n", WSAGetLastError());
//        freeaddrinfo(addressList);
//        closesocket(ListenSocket);
//        WSACleanup();
//        return 1;
//    }
//
//    freeaddrinfo(addressList);
//
//	while(!_kbhit())        // Loop until key pressed
//	{        
//        printf("\r\nListening for connection...");
//
//		// After the socket is bound to an IP address and port on the system, 
//		// the server must then listen on that IP address and port for 
//		// incoming connection requests.
//		// To listen on a socket call the listen function, passing the created 
//		// socket and the maximum number of allowed connections to accept as 
//		// parameters. Check for general errors.
//		//
//		iResult = listen(ListenSocket, SOMAXCONN);
//		if (iResult == SOCKET_ERROR) 
//		{
//			printf("listen failed: %d\n", WSAGetLastError());
//			closesocket(ListenSocket);
//			WSACleanup();
//			return 1;
//		}
//
//		// Once the socket is listening for a connection, the program must 
//		// handle connection requests on that socket.
//		// To accept a connection on a socket, create a temporary SOCKET object 
//		// called AcceptSocket for accepting connections.
//		//
//
//		// Create a continuous loop that checks for connections requests. 
//		// If a connection request occurs, call the accept function to 
//		// handle the request.
//		//
//		// Accept a client socket
//		SOCKET temp;
//		ClientSocket = accept(ListenSocket, NULL, NULL);
//		if (ClientSocket == INVALID_SOCKET) 
//		{
//			printf("accept failed: %d\n", WSAGetLastError());
//			closesocket(ListenSocket);
//			WSACleanup();
//			return 1;
//		}
//
//		// The send and recv functions both return an integer value of the 
//		// number of bytes sent or received, respectively, or an error. 
//		// Each function also takes the same parameters: the active socket, 
//		// a char buffer, the number of bytes to send or receive, and any 
//		// flags to use.
//
//		// Receive until the peer shuts down the connection
//		do 
//		{
//            // Note: This bit of code is "broken".
//            // This code overwrites the contents of the buffer
//            // each time around the loop.
//            // Depending on your application, you should either
//            // keep a pointer into the buffer which is updated
//            // to the next position at which data should be
//            // stored in the buffer or create a new buffer
//            // each time you receive more data.
//            //
//			iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
//			if (iResult > 0)
//			{
//				printf("Bytes received: %d Msg=%s\n", iResult, recvbuf);
//				iRcvdBytes = iResult;
//			}
//			else if (iResult == 0)
//				printf("Connection closing...\n");
//			else  
//			{
//				printf("recv failed: %d\n", WSAGetLastError());
//				closesocket(ClientSocket);
//				WSACleanup();
//				return 1;
//			}
//
//		} while (iResult > 0);
//
//		// Echo the buffer back to the sender
//		iResult = send( ClientSocket, recvbuf, iRcvdBytes, 0 );
//		if (iResult == SOCKET_ERROR) 
//		{
//			printf("send failed: %d\n", WSAGetLastError());
//			closesocket(ClientSocket);
//			WSACleanup();
//			return 1;
//		}
//
//		printf("Bytes Sent: %ld\n", iResult);
//
//		// shutdown the connection since we're done
//		// shutdown() acts like a close() but waits for the receiver
//		// to ACK the last data sent by the client.
//		// SD_SEND = no more sends allowed
//		// For TCP sockets, a FIN will be sent after all data is sent 
//		// and acknowledged by the receiver.
//		// Other options are SD_RECEIVE and SD_BOTH.
//		//
//		iResult = shutdown(ClientSocket, SD_SEND);
//		if (iResult == SOCKET_ERROR) 
//		{
//			printf("shutdown failed: %d\n", WSAGetLastError());
//			closesocket(ClientSocket);
//			WSACleanup();
//			return 1;
//		}
//
//		// cleanup
//		closesocket(ClientSocket);
//	}
//
//    // When the client connection has been accepted, close the 
//    // original socket, transfer control from the temporary socket 
//    // to the original socket, and stop checking for new connections.
//    // No longer need server socket.
//    //
//    closesocket(ListenSocket);
//
//    printf("\r\nHit key to exit");
//	while(!_kbhit());
//
//	WSACleanup();
//
//	return 0;
//	
//}
