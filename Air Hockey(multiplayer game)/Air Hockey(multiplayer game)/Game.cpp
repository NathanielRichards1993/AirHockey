#include<iostream>
#include"stdafx.h"
#include"glut.h"
#include<time.h>

int mainMenuSelection();
void displayMenu();
void displayGame();
void display();
//void game();
void gameLoop();
void MouseControl(int button,int state,int x,int y);
void reshape(int,int);
void DrawCircle(float cx,float cy, float r, int num_segments);
void keyboardread(unsigned char key, int x, int y);
void timeFill(unsigned short*,unsigned short*,unsigned short*);
unsigned short plusTimeSeconds(unsigned short plusValue,unsigned short* minute,unsigned short* seconds);
unsigned short plusTimeMinutes(unsigned short plusValue,unsigned short* hour,unsigned short* minute);
unsigned short minusTime(short minusValue,unsigned short* minute,unsigned* seconds);
short differnceInSeconds(unsigned short minuteA,unsigned short secondsA,
								  unsigned short minuteB,unsigned short secondsB);
short differnceInMinutes(unsigned short hourA,unsigned short minuteA,
								  unsigned short hourB,unsigned short minuteB);

void newRoundHost();
void newRoundClient();
int endProgram();

bool endGame = false;
bool gameStarted = false;
bool roundStart = false;
bool host = false;
bool connecting = false;


//setting up new round
ConfirmNewRoundTime* cNRT_SU = NULL;

unsigned short numberOfFailures = 0;
bool gameStartAgreed = false;
bool timeAgreed = false;
unsigned short aMinute = 0;;
unsigned short aSeconds = 0;
unsigned short state = 0;//

unsigned short sMinute = 0;//end time is 3+
unsigned short sSeconds = 0;

unsigned short eHour = 0;
unsigned short eMinute = 0;
unsigned short eSeconds = 0;

char instruction1[1024];
int length = 0;
bool startTimer = false;
char storeTime[8] = "";
bool resetGame = false;

Vector2f* vec = new Vector2f(10,150);
player* homePlayer = new player(vec,15);
Vector2f* vec2 = new Vector2f(790, 150);
player* opponentPlayer = new player(vec2, 15);
Vector2f* vec3 = new Vector2f(400,150);
Vector2f* vel1 = new Vector2f(0, 10);
puck* currentPuck = new puck(vec3,vel1,15);

double width = 800;
double height = 300;
float topGoalPost = 225;
float botGoalPost = 75;

TCP_Server* serverTCP = NULL;
TCP_Client* clientTCP = NULL;
UDP_Client* clientUDP = NULL;
//UDP_Server* serverUDP;

HANDLE handleClientTcp;
HANDLE handleServerTCP;
DWORD threadidTcpServer;
DWORD threadidTCPClient;

HANDLE handleUdpListen;
HANDLE handleUdpSend;
DWORD threadidUdpListen;
DWORD threadidUDPSend;

time_t locTime;
int dot = 1;
Vector2f* mouseLocation = new Vector2f(0.0, 0.0);
//unsigned long currentTime;
//unsigned long previousTime;
Vector2f* mouse = new Vector2f(150,400);
Vector2f* homeGoalLocation = new Vector2f(0, 150);
Vector2f* opponentGoalLocation = new Vector2f(800, 150);
bool ipEnter = false;
char serverIp[512];
//float tick = 0;
//sending
HANDLE  emptyhdlSend;
HANDLE  fullhdlSend;
//listening
HANDLE  emptyhdlListen;
HANDLE  fullhdlListen;

//time
unsigned short hour;
unsigned short minute;
unsigned short seconds;

void main(int argc, char **argv)
{
	//setupGame screen
	glutInit(&argc,argv);							// Initialize GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);				// Set the window's initial width & height
	glutInitWindowPosition(150, 150);					// Position the window's initial top-left corner
	glutCreateWindow("Air Hockey On Pc");			// Create a window with the given title
	glutDisplayFunc(gameLoop);						// Register display callback handler for window re-paint
	glutIdleFunc(gameLoop);
	glutReshapeFunc(reshape);
	glutMouseFunc(MouseControl);
	glutKeyboardFunc(keyboardread);
	
	state = 1;
	homePlayer->setGoalLocation(homeGoalLocation);
	opponentPlayer->setGoalLocation(opponentGoalLocation);
	sprintf(serverIp, "");
	//client->setIp((TCHAR*)"127.0.0.1");

	//setup width height and 2d game
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);
	Vector2f* dir = new Vector2f(2, 3);
	currentPuck->setPuckVelocity(dir);
	gameStarted = false;
	
	glutMainLoop();           // Enter the event-processing loop
}

void gameLoop()
{
	if(glutGetWindow() == -1)
	{
		endProgram();
	}

	if (gameStarted == false)
	{
		displayMenu();
		if (endGame == true)//close game
		{
			//glutDestroyWindow(1);
		}
	}
	else
	{
		if(roundStart == true)
		{
			displayGame();
		}
		else
		{
			if(homePlayer->getHost() == true)
				newRoundHost();
			else
				newRoundClient();
		}
	}
}

void displayMenu()
{
	Sleep(1);
	if (gameStarted == false)
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		bool host = false;
		bool leaveGame = false;
		bool chosen = false;
		char* recieveMessage = "\n";
		float strength = 1.0;

		char instruction1[1024];
		char* blank = "";

		if (connecting == true)
		{
			strength = 0.5;
		}
		else
		{
			strength = 1.0;
		}
		//float size = GL_LINE_WIDTH;

		glLineWidth(5);

		glColor3f(strength, 0.0, 0.0);
		glBegin(GL_QUADS);
		glVertex2f(50, 110);
		glVertex2f(250, 110);
		glVertex2f(250, 190);
		glVertex2f(50, 190);
		glEnd();

		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);
		glVertex2f(50, 110);
		glVertex2f(250, 110);

		glVertex2f(250, 107);
		glVertex2f(250, 192);

		glVertex2f(250, 190);
		glVertex2f(50, 190);

		glVertex2f(50, 107);
		glVertex2f(50, 192);
		glEnd();

		glColor3f(0.0, strength, 0.0);
		glBegin(GL_QUADS);
		glVertex2f(300, 110);
		glVertex2f(500, 110);
		glVertex2f(500, 190);
		glVertex2f(300, 190);
		glEnd();

		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);
		glVertex2f(300, 110);
		glVertex2f(500, 110);

		glVertex2f(500, 107);
		glVertex2f(500, 192);

		glVertex2f(500, 190);
		glVertex2f(300, 190);

		glVertex2f(300, 107);
		glVertex2f(300, 192);
		glEnd();

		glColor3f(0.0, 0.0, 1.0);
		glBegin(GL_QUADS);
		glVertex2f(550, 110);
		glVertex2f(750, 110);
		glVertex2f(750, 190);
		glVertex2f(550, 190);
		glEnd();

		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);
		glVertex2f(550, 110);
		glVertex2f(750, 110);

		glVertex2f(750, 107);
		glVertex2f(750, 192);

		glVertex2f(750, 190);
		glVertex2f(550, 190);

		glVertex2f(550, 192);
		glVertex2f(550, 107);
		glEnd();

		glColor3f(1.0, 1.0, 1.0);
		if (leaveGame == true)
		{
			////how did you get here
			//glRasterPos2i(10, 290);
			//char instruction1[128];
			//int length = strlen(instruction1);
			//strcpy(instruction1, "Exiting");

			//if (dot == 1)
			//{
			//	sprintf(instruction1, ".  ");
			//}
			//else if (dot == 2)
			//{
			//	sprintf(instruction1, ".. ");
			//}
			//else
			//{
			//	sprintf(instruction1, "...");
			//	dot = 0;
			//}
			//for (int i = 0; i < length; i++)
			//{
			//	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, instruction1[i]);
			//}
			//dot++;
		}

		if (connecting == false)
		{
			glRasterPos2i(95, 142);
			strcpy(instruction1, blank);
			strcpy(instruction1, "Host Game");
			int length = strlen(instruction1);

			for (int i = 0; i < length; i++)
			{
				glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, instruction1[i]);
			}

			glRasterPos2i(310, 142);
			strcpy(instruction1, blank);
			strcpy(instruction1, "Connect To Game");
			length = strlen(instruction1);

			for (int i = 0; i < length; i++)
			{
				glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, instruction1[i]);
			}

			glRasterPos2i(600, 142);
			strcpy(instruction1, blank);
			strcpy(instruction1, "Exit Game");
			length = strlen(instruction1);

			for (int i = 0; i < length; i++)
			{
				glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, instruction1[i]);
			}
		}


		//if tcp are talking
		if(connecting == true)
		{
			DWORD message = WaitForSingleObject(handleClientTcp, 1);
			if (message == WAIT_TIMEOUT)
			{
				glRasterPos2i(95, 142);
				strcpy(instruction1, blank);
				strcpy(instruction1, "Waiting");
				int length = strlen(instruction1);

				for (int i = 0; i < length; i++)
				{
					glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, instruction1[i]);
				}

				glRasterPos2i(310, 142);
				strcpy(instruction1, blank);
				strcpy(instruction1, "No Option");
				length = strlen(instruction1);

				for (int i = 0; i < length; i++)
				{
					glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, instruction1[i]);
				}

				glRasterPos2i(600, 142);
				strcpy(instruction1, blank);
				strcpy(instruction1, "Exit Game");
				length = strlen(instruction1);

				for (int i = 0; i < length; i++)
				{
					glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, instruction1[i]);
				}


				glRasterPos2i(10, 12);
				strcpy(instruction1, "");
				strcpy(instruction1, "Connecting");
				if (dot <= 3)
				{
					strcat(instruction1, ".  ");
				}
				else if (dot <= 6)
				{
					strcat(instruction1, ".. ");
				}
				else if(dot <= 9)
				{
					strcat(instruction1, "...");
				}
				else
				{
					dot = 0;
				}
 
				length = strlen(instruction1);

				for (int i = 0; i < length; i++)
				{
					glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, instruction1[i]);
				}
				dot++;
			}
			else if (message == WAIT_OBJECT_0)//Connected
			{
				char message[512] = "";
				clientTCP->getMessage(message);
				printf("\nm:%s\n",message);
				if (message[15] == '1')
				{
					homePlayer->setHost(true);
					opponentPlayer->setHost(false);
				}
				else
				{
					homePlayer->setHost(false);
					opponentPlayer->setHost(true);
				}

				//closing tcp client and server
				if(homePlayer->getHost() == true)
				{
					
					WaitForSingleObject(handleClientTcp,INFINITE);
					TerminateThread(handleClientTcp,0);
					CloseHandle(handleClientTcp);
				}
				else
				{
				}
				WaitForSingleObject(handleServerTCP,INFINITE);
				TerminateThread(handleServerTCP,0);
				CloseHandle(handleServerTCP);

				clientUDP = new UDP_Client();
				clientUDP->setup();

				//always create any thread specific memory,
				//mutexs and semiphores before the thread is created
				emptyhdlSend = CreateSemaphore(NULL,                   // Security attributes -- NULL for none.
					0,                  // Initial count for the semaphore object. 
					5,                  // Maximum count for the semaphore object 
					clientUDP->semaphore);     // name of the semaphore object. 
				// The name is limited to MAX_PATH characters.

				fullhdlSend = CreateSemaphore(NULL,                    // Security attributes -- NULL for none.
					0,                  // Initial count for the semaphore object. 
					5,                  // Maximum count for the semaphore object 
					clientUDP->semaphore2);    // name of the semaphore object. 

				emptyhdlListen = CreateSemaphore(NULL,                   // Security attributes -- NULL for none.
					0,                  // Initial count for the semaphore object. 
					5,                  // Maximum count for the semaphore object 
					clientUDP->semaphore3);     // name of the semaphore object. 
				// The name is limited to MAX_PATH characters.

				fullhdlListen = CreateSemaphore(NULL,                    // Security attributes -- NULL for none.
					0,                  // Initial count for the semaphore object. 
					5,                  // Maximum count for the semaphore object 
					clientUDP->semaphore4);    // name of the semaphore object. 
					
				ReleaseSemaphore(emptyhdlSend, 5, NULL);
				ReleaseSemaphore(emptyhdlListen, 5, NULL);
					

				if(homePlayer->getHost() == true)
					clientUDP->isHost();
				else
					clientUDP->notHost();
				//to udp client list
				if (homePlayer->getHost() == true)
					clientUDP->setIp("127.0.0.1");
				else
					clientUDP->setIp(serverIp);
				//listen
				/*clientUDP->init();*/

				clientUDP->init();

				handleUdpListen = CreateThread(NULL, 0,
					clientUDP->listenSocketLoop,
					(LPVOID)clientUDP, 0, NULL);
				Sleep(100);
				while(clientUDP->listenReady == false)
				{
					//waiting for listen to be setup
				}
				//send
				handleUdpSend = CreateThread(NULL, 0,
					clientUDP->sendSocketLoop,
					(LPVOID)clientUDP, 0, NULL);

				if(homePlayer->getHost() == true)
					clientUDP->isHost();
				else
					clientUDP->notHost();

				gameStarted = true;

				}
			

		}
		else if (connecting == false)
		{
			if (ipEnter == true)
			{
				glRasterPos2i(310, 130);
				strcpy(instruction1, blank);
				strcpy(instruction1, "ip:\0");
				strcat(instruction1, (char*)serverIp);
				int length = strlen(instruction1);

				for (int i = 0; i < length; i++)
				{
					glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, instruction1[i]);
				}
			}
			else
			{
				glRasterPos2i(95, 142);
				strcpy(instruction1, blank);
				strcpy(instruction1, "Host Game");
				int length = strlen(instruction1);

				for (int i = 0; i < length; i++)
				{
					glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, instruction1[i]);
				}

				glRasterPos2i(310, 142);
				strcpy(instruction1, blank);
				strcpy(instruction1, "Connect To Game");
				length = strlen(instruction1);

				for (int i = 0; i < length; i++)
				{
					glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, instruction1[i]);
				}

				glRasterPos2i(600, 142);
				strcpy(instruction1, blank);
				strcpy(instruction1, "Exit Game");
				length = strlen(instruction1);

				for (int i = 0; i < length; i++)
				{
					glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, instruction1[i]);
				}
			}
		}


	}
	else
	{

	}
	
	glFlush();
	glutSwapBuffers();
}		

void displayGame()
{
	//tick = clock();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	if(startTimer == false)
	{
		homePlayer->updateLocation();
		opponentPlayer->updateLocation();

		if (currentPuck->updatePuck(width, height) == true)
		{
			//push to send
			//dec
			int iResult = WaitForSingleObject(emptyhdlSend, 5);//this does not really need to send
			if (iResult == WAIT_OBJECT_0)
			{

				//Take Mutex
				WaitForSingleObject(clientUDP->mutexhdlSend,INFINITE);
				SyncHitWall* sHW = new SyncHitWall(hour,minute,seconds, 
					currentPuck->getPucksLocation(), currentPuck->getPuckVelocty());
				clientUDP->SHW_Send->push_back(sHW);			
				ReleaseMutex(clientUDP->mutexhdlSend);
				//Release Mutex
				ReleaseSemaphore(fullhdlSend, 1, NULL);//inc

			}
		}
	}

	//this is done when the player moves in mouseControl

	//consume the update into homeplayer or opponent
	int iResult = WaitForSingleObject(fullhdlListen, 50); // should get this quick
	if (iResult == WAIT_OBJECT_0)
	{				
		//Take Mutex
		WaitForSingleObject(clientUDP->mutexhdlListen,INFINITE);
		if (clientUDP->PNL_Listen->size() > 0)
		{
			PlayerNewLocation* pNL = clientUDP->PNL_Listen->at(0);//get the first
			clientUDP->PNL_Listen->erase(clientUDP->PNL_Listen->begin());				
			ReleaseMutex(clientUDP->mutexhdlListen);
			//Release Mutex

			//Release Semaphore
			ReleaseSemaphore(emptyhdlListen, 1, NULL);
			//player new location
			if(homePlayer->getHost() == true)
			{
				//opponentPlayer->setPlayerLocation(&pNL->getPlayerLocation());
				opponentPlayer->setPlayerVelocity(&pNL->getPlayerVelocity());
			}
			else
			{
				//homePlayer->setPlayerLocation(&pNL->getPlayerLocation());
				homePlayer->setPlayerVelocity(&pNL->getPlayerVelocity());
			}
			
			delete(pNL);
		}
		else if(clientUDP->SHW_Listen->size() > 0)
		{
			SyncHitWall* sHW = clientUDP->SHW_Listen->at(0);//get the first
			clientUDP->SHW_Listen->erase(clientUDP->SHW_Listen->begin());				
			ReleaseMutex(clientUDP->mutexhdlListen);
			//Release Mutex

			//Release Semaphore
			ReleaseSemaphore(emptyhdlListen, 1, NULL);
			
			if(currentPuck->getPuckVelocty()->getX() == sHW->getNewVelocity().getX()
				&& currentPuck->getPuckVelocty()->getY() == sHW->getNewVelocity().getY())
			{
				printf("\nin sync\n");
			}
			else
			{
				printf("\nout of sync\n");
			}

			delete(sHW);
				
		}
		else if (clientUDP->NR_Listen->size() > 0)
		{
			NewRound* nR = clientUDP->NR_Listen->at(0);
			clientUDP->NR_Listen->erase(clientUDP->NR_Listen->begin());
			ReleaseMutex(clientUDP->mutexhdlListen);
			//Release Mutex

			//Release Semaphore
			ReleaseSemaphore(emptyhdlListen,1,NULL);

			//reset round
			if(homePlayer->getHost() == true)
			{
				if(aMinute == nR->getTimeMinute() &&
					aSeconds == nR->getTimeSeconds())
				{
					startTimer = true;
					plusTimeSeconds(10,&eMinute,&eSeconds);
				}
				else
				{
					timeFill(&hour,&minute,&seconds);
					aMinute = minute;
					aSeconds = seconds;
					plusTimeSeconds(10,&aMinute,&aSeconds);
					int r = WaitForSingleObject(clientUDP->mutexhdlSend, INFINITE);
					if(r == WAIT_OBJECT_0)
					{
						NewRound* newRound = new NewRound(hour,aMinute,aSeconds,
							homePlayer->getScore(),opponentPlayer->getScore());
						clientUDP->NR_Send->push_back(newRound);
						ReleaseMutex(clientUDP->mutexhdlSend);

						ReleaseSemaphore(emptyhdlSend,1,NULL);

						printf("\nnew time plus 10\n");

					}
					else
					{
						printf("\ncouldn't accure mutex send on a infinite wait\n");
					}
				}
			}
			else
			{
				timeFill(&hour,&minute,&seconds);
				aMinute = minute;
				aSeconds = seconds;
				plusTimeSeconds(5,&aMinute,&aSeconds);
				unsigned short secDif = differnceInSeconds(nR->getTimeMinute(),nR->getTimeSeconds(),
					aMinute,aSeconds);
				if(secDif >= 2)
				{
					startTimer = true;
					int iResult = WaitForSingleObject(emptyhdlSend, 5);//this does not really need to send
					if (iResult == WAIT_OBJECT_0)
					{
						int r = WaitForSingleObject(clientUDP->mutexhdlSend, INFINITE);
						if(r == WAIT_OBJECT_0)
						{
							NewRound* newRound = new NewRound(hour,nR->getTimeMinute()
								,nR->getTimeSeconds(),
								homePlayer->getScore(),opponentPlayer->getScore());
							clientUDP->NR_Send->push_back(newRound);
							ReleaseMutex(clientUDP->mutexhdlSend);

							ReleaseSemaphore(fullhdlSend,1,NULL);

							printf("\ngood time sending same time back\n");

						}
						else
						{
							printf("\ncouldn't accure mutex send on a infinite wait\n");
						}
					}
					else
					{
						printf("\ncannot grab a semaphore emptyhdlsend\n");
					}
				}
				else
				{
					int iResult = WaitForSingleObject(emptyhdlSend, 5);//this does not really need to send
					if (iResult == WAIT_OBJECT_0)
					{
						NewRound* newRound = new NewRound(hour,nR->getTimeMinute()-1
							,nR->getTimeSeconds()-1,
							homePlayer->getScore(),opponentPlayer->getScore());
						clientUDP->NR_Send->push_back(newRound);
						ReleaseMutex(clientUDP->mutexhdlSend);
						
						ReleaseSemaphore(fullhdlSend,1,NULL);

						printf("\nbad time sending time back -1\n");
					}
					else
					{
						printf("\ncannot grab a semaphore emptyhdlsend\n");
					}
				}
			}
			delete(nR);
		}
		else
		{
			//Release Mutex
			ReleaseMutex(clientUDP->mutexhdlListen);
			//Release Semaphore
			ReleaseSemaphore(fullhdlListen,1,NULL);
		}
	}

	homePlayer->collisonWithPuck(currentPuck);
	opponentPlayer->collisonWithPuck(currentPuck);

	timeFill(&hour,&minute,&seconds);

	//before puck update check to see if there has been a goal
	if(currentPuck->goalScored(width,topGoalPost,botGoalPost) == true)
	{
		if(homePlayer->getHost() == true)
		{
			int iResult = WaitForSingleObject(emptyhdlSend, 5);//this does not really need to send
			if (iResult == WAIT_OBJECT_0)
			{
				timeFill(&hour,&seconds,&minute);
				aMinute = minute;
				aSeconds = seconds;
				plusTimeSeconds(10,&aMinute,&aSeconds);
				//Take Mutex
				WaitForSingleObject(clientUDP->mutexhdlSend,INFINITE);
				NewRound* nRound = new NewRound(hour,aMinute,aSeconds, 
					homePlayer->getScore(),opponentPlayer->getScore());
				clientUDP->NR_Send->push_back(nRound);			
				ReleaseMutex(clientUDP->mutexhdlSend);
				//Release Mutex
				ReleaseSemaphore(fullhdlSend, 1, NULL);//inc
			}
		}
		else
		{
			timeFill(&hour,&seconds,&minute);
			aMinute = minute;
			aSeconds = seconds;
			plusTimeSeconds(10,&aMinute,&aSeconds);
		}
	}
	else
	{
		//not near the goal
	}

	//Home Player Score board background
	glColor3f(0.0, 0.6, 0.2);
	glBegin(GL_QUADS);
		glVertex2f(width/2 - 30,//top left
			height);
		glVertex2f(width/2,//top right
			height);
		glVertex2f(width/2,//bottom right
			height - 30);
		glVertex2f(width/2 - 30,//bottom left
			height -30);
	glEnd();

	glColor3f(1.0,1.0,1.0);
	glRasterPos2i(width/2 - 15, height-15);
	sprintf(instruction1,"%d",homePlayer->getScore());
	instruction1[3] = '\0';
	length = strlen(instruction1);
	glColor3f(1.0,1.0,1.0);
	for (int i = 0; i < length; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, instruction1[i]);
	}

		//Home Player Score board background
	glColor3f(0.6, 0.0, 0.2);
	glBegin(GL_QUADS);
		glVertex2f(width/2,//top left
			height);
		glVertex2f(width/2 +30,//top right
			height);
		glVertex2f(width/2 + 30,//bottom right
			height - 30);
		glVertex2f(width/2,//bottom left
			height -30);
	glEnd();

	glColor3f(1.0,1.0,1.0);
	glRasterPos2i(width/2 + 15, height-15);
	sprintf(instruction1,"%d",opponentPlayer->getScore());
	length = strlen(instruction1);
	instruction1[3] = '\0';
	for (int i = 0; i < length; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, instruction1[i]);
	}

	if(startTimer == true)
	{
		glColor3f(1.0,1.0,1.0);
		glRasterPos2i(width/2 - 20,height/2 + 20);
		timeFill(&hour,&minute,&seconds);
		
		sprintf(instruction1, "NEW ROUND : %d",
			differnceInSeconds(aMinute,aSeconds,minute,seconds));
		length = strlen(instruction1);

		glColor3f(1.0,1.0,1.0);
		for (int i = 0; i < length; i++)
		{
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, instruction1[i]);
		}
	}
	else
	{
		glColor3f(1.0,1.0,1.0);
		glRasterPos2i(width/2,10);
		timeFill(&hour,&minute,&seconds);
		int min = differnceInMinutes(eHour,eMinute,hour,minute);
		int sec = differnceInSeconds(eMinute,eSeconds,minute,seconds);
		int sMin = (float)sec/(float)60;
		sec = sec - (sMin)*60;
		sprintf(instruction1,"%d:%d",sMin,sec);
		length = strlen(instruction1);
		
		glColor3f(1.0,1.0,1.0);
		for (int i = 0; i < length; i++)
		{
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, instruction1[i]);
		}
	}

	//Draw Home player
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_QUADS);
		glVertex2f(homePlayer->getPlayerLocation()->getX() - 15,//top left
			homePlayer->getPlayerLocation()->getY()+ 15);
		glVertex2f(homePlayer->getPlayerLocation()->getX() + 15,//top right
			homePlayer->getPlayerLocation()->getY() + 15);
		glVertex2f(homePlayer->getPlayerLocation()->getX() + 15,//bottom right
			homePlayer->getPlayerLocation()->getY() - 15);
		glVertex2f(homePlayer->getPlayerLocation()->getX() - 15,//bottom left
			homePlayer->getPlayerLocation()->getY() - 15);
	glEnd();
	DrawCircle(0, 150, 100, 20);
	//Draw Opponent Player
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_QUADS);
		glVertex2f(opponentPlayer->getPlayerLocation()->getX() - 15,//top left
			opponentPlayer->getPlayerLocation()->getY() + 15);
		glVertex2f(opponentPlayer->getPlayerLocation()->getX() + 15,//top right
			opponentPlayer->getPlayerLocation()->getY() + 15);
		glVertex2f(opponentPlayer->getPlayerLocation()->getX() + 15,//bottom right
			opponentPlayer->getPlayerLocation()->getY() - 15);
		glVertex2f(opponentPlayer->getPlayerLocation()->getX() - 15,//bottom left
			opponentPlayer->getPlayerLocation()->getY() - 15);
	glEnd();
	DrawCircle(800, 150, 100, 20);

	//Draw Puck
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_QUADS);
	glVertex2f(currentPuck->getPucksLocation()->getX() - 15,//top left
		currentPuck->getPucksLocation()->getY() + 15);
	glVertex2f(currentPuck->getPucksLocation()->getX() + 15,//top right
		currentPuck->getPucksLocation()->getY() + 15);
	glVertex2f(currentPuck->getPucksLocation()->getX() + 15,//bottom right
		currentPuck->getPucksLocation()->getY() - 15);
	glVertex2f(currentPuck->getPucksLocation()->getX() - 15,//bottom left
		currentPuck->getPucksLocation()->getY() - 15);
	glEnd();

	DrawCircle(400, 150, 100, 20);

	glFlush();
	glutSwapBuffers();
}

void MouseControl(int button, int state, int xAxis, int yAxis)
{
	if (roundStart == true)
	{ 
		mouse->setX(xAxis);
		mouse->setY(yAxis);
		if (homePlayer->getHost() == true)
		{
			//push to send			
			int iResult = WaitForSingleObject(emptyhdlSend, 65);
			if (iResult == WAIT_OBJECT_0)
			{
				homePlayer->moveTowardsMouse(mouse, 50);
				timeFill(&hour,&minute,&seconds);
				

				//Take Mutex
				WaitForSingleObject(clientUDP->mutexhdlSend,INFINITE);	
				PlayerNewLocation* newLocation = new PlayerNewLocation(
					hour,minute,seconds, homePlayer->getPlayerLocation(),
					homePlayer->getPlayerVelocity());
				clientUDP->PNL_Send->push_back(newLocation);
				ReleaseMutex(clientUDP->mutexhdlSend);
				//Release Mutex

				ReleaseSemaphore(fullhdlSend, 1, NULL);
			}
		}
		else/*if not host basically you are the opponent*/
		{
			//push to send
			
			int iResult = WaitForSingleObject(emptyhdlSend, 50);
			if (iResult == WAIT_OBJECT_0)
			{
				opponentPlayer->moveTowardsMouse(mouse, 50);
				timeFill(&hour,&minute,&seconds);

				//Take Mutex
				WaitForSingleObject(clientUDP->mutexhdlSend,INFINITE);	
				PlayerNewLocation* newLocation = new PlayerNewLocation(
					hour,minute,seconds, opponentPlayer->getPlayerLocation()
					,opponentPlayer->getPlayerVelocity());
				clientUDP->PNL_Send->push_back(newLocation);
				ReleaseMutex(clientUDP->mutexhdlSend);
				//Release Mutex

				ReleaseSemaphore(fullhdlSend, 1, NULL);

			}
		}

	}
	else
	{
		if (button == 0 && xAxis > 50 && xAxis < 250
			&& yAxis > 50 && yAxis < 250 && connecting == false)
		{
			//mouse hit start server and client
			serverTCP = new TCP_Server();
			serverTCP->intiServer();
			handleServerTCP = CreateThread(NULL, 0,
				serverTCP->listenSocketLoop,
				(LPVOID)serverTCP, 0, NULL);

			Sleep(150);
			clientTCP = new TCP_Client();
			clientTCP->initClient();
			clientTCP->sendMessageToserver("hello server");
			handleClientTcp = CreateThread(NULL, 0,
				clientTCP->recieveMessageFromServer,
				(LPVOID)clientTCP, 0, NULL);
			//client->recieveMessageFromServer(recieveMessage); //launch this as thread and check

			host = true;
			connecting = true;
		}
		else if (button == 0 && xAxis > 300 && xAxis < 500
			&& yAxis > 110 && yAxis < 190 && connecting == false)
		{
			ipEnter = true;
		}
	}
}

void reshape(int w, int h) 
{
	
	/*glViewport(250, 250, w, h);
	gluOrtho2D(0, float(width), 0, float(height));*/
}


void DrawCircle(float cx, float cy, float r, int num_segments)
{
	glBegin(GL_LINE_LOOP);
	for (int ii = 0; ii < num_segments; ii++)
	{
		float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle 

		float x = r * cosf(theta);//calculate the x component 
		float y = r * sinf(theta);//calculate the y component 

		glVertex2f(x + cx, y + cy);//output vertex 

	}
	glEnd();
}

void keyboardread(unsigned char key, int x, int y)
{
	char* ip = "";
	int result;
	int len = 0;
	if (ipEnter == true)
	{
		switch (key)
		{
		case '\b':
			
			if(serverIp[0] == '\0')
			{
				//ignore no length
			}
			else
			{
				len = strlen(serverIp)-1;
				serverIp[len] = '\0';
			}
			break;
		case'r':
			sprintf(serverIp, "");
			break;
		case'e':
			clientTCP = new TCP_Client();
			clientTCP->setIp(serverIp);
			printf("ip entered: %s", serverIp);
			result = clientTCP->initClient();
			if (result != 1)
			{
				clientTCP->sendMessageToserver("hello server");
				handleClientTcp = CreateThread(NULL, 0,
					clientTCP->recieveMessageFromServer,
					(LPVOID)clientTCP, 0, NULL);

				host = false;
				connecting = true;
			}
			else
			{
				printf("server does not exist");
			}

			break;
		default:
			printf("num:%d", key);
			if ((int)key > 47 && (int)key < 58 || key == '.')
			{
				char k[3];
				k[0] = key;
				k[1] = '\0';

				strcat(serverIp, k);
			}
			break;
		}
	}
	
}
/*First 6 signifcant figures are hours.mins.sec no space
betweem numbers*/
/*Maybe change to short instead of long*/
void timeFill(unsigned short* hour, unsigned short* minute, unsigned short* seconds)
{
	time_t tock;
	struct tm * timeinfo;
	char tempTime[26] = "\0";

	time(&tock);
	timeinfo = localtime(&tock);
	sprintf(tempTime,"%s",asctime(timeinfo));


	*hour = ((short)tempTime[11]- 48)*10 +
		((short)tempTime[12]- 48);
	*minute = ((short)tempTime[14]- 48)*10 +
		((short)tempTime[15]- 48);
	*seconds = ((short)tempTime[17]- 48)*10 +
		((short)tempTime[18]- 48);
}

/*plues value must be 23 or less*/
unsigned short plusTimeMinutes(unsigned short plusValue,unsigned short* fHour, unsigned short* fMinute)
{
	
	if(plusValue < 23)
	{
		if((*fMinute + plusValue) > 59)
		{
			if(*fMinute < 59)
				*fHour+=1;
			else
				*fHour = 0;

			*fMinute+=plusValue-60;
		}
		else
		{
			*fMinute+=plusValue;
		}
		return 1;
	}
	return 22;
}

/*plues value must be 59 or less*/
unsigned short plusTimeSeconds(unsigned short plusValue,unsigned short* min, unsigned short* sec)
{
	
	if(plusValue < 59)
	{
		if((*sec + plusValue) > 59)
		{
			if(*min < 59)
				*min+=1;
			else
				*min = 0;

			*sec+=plusValue-60;
		}
		else
		{
			*sec+=plusValue;
		}
		return 1;
	}
	return 22;
}
/*keep the negative value*/
/*not the amount to minus*/
unsigned short minusTime(short minusValue,unsigned short* minute, unsigned short* seconds)
{
	
	if(minusValue > 0)
	{
		if(*seconds+minusValue < 0)
		{
			if(minute > 0)
				*minute--;
			else
				*minute=59;

			*seconds+=minusValue;
		}
		else
		{
			*seconds+=minusValue;
		}
		return 1;
	}
	return 22;
}
/*for positive number larger values first*/
short differnceInSeconds(unsigned short minuteA, unsigned short secondsA,
								  unsigned short minuteB,unsigned short secondsB)
{
	//should stop incorrect value
	if(minuteA == 59 && minuteB == 0)
	{
		minuteB += 60;
	}
	else if(minuteB == 59 && minuteB == 0)
	{
		minuteA += 60;
	}
	//if one of them is 59 and the other 0 
	//make the 0 one 60
		
	short secondsFromMinutes = minuteA*60 - minuteB*60;
	short sec = secondsA-secondsB;
	return (secondsFromMinutes+sec);
}
/*for positive number larger values first*/
short differnceInMinutes(unsigned short hourA, unsigned short minuteA,
								  unsigned short hourB,unsigned short minuteB)
{
	//should stop incorrect value
	if(hourA == 23 && hourB == 0)
	{
		minuteB += 24;
	}
	else if(hourB == 23 &&hourB == 0)
	{
		minuteA += 24;
	}
	//if one of them is 23 and the other 0 
	//make the 0 one 24
		
	short minutesFromHours = hourA*24 - hourB*24;
	short min = minuteA-minuteB;
	return (minutesFromHours+min);
}

//send new round start time
//wait for go ahead in recption of confirm new round
//send rount start time if not agreed else sent the same message back
//then count down to confirmed time
void newRoundHost()
{

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	switch(state)
	{
	case 1:

		{
			timeFill(&hour,&minute,&seconds);
			unsigned short pTime = 20;//seconds
			aMinute = minute;
			aSeconds = seconds;
			plusTimeSeconds(pTime,&aMinute,&aSeconds);
			int iResult = WaitForSingleObject(emptyhdlSend, 50);
			if (iResult == WAIT_OBJECT_0)
			{

				//Take send mutex
				WaitForSingleObject(clientUDP->mutexhdlSend,INFINITE);	
				NewRound* nRound = new NewRound(hour,aMinute,aSeconds,homePlayer->getScore(),
					opponentPlayer->getScore());
				clientUDP->NR_Send->push_back(nRound);

				
				ReleaseMutex(clientUDP->mutexhdlSend);
				//Release Mutex
				
				ReleaseSemaphore(fullhdlSend, 1, NULL);	
				state++;
				printf("\nSending New Round Host\n");
			}
			break;
		}
	case 2:
		{
			int iResult = WaitForSingleObject(fullhdlListen, 50); // should get this quick
			if (iResult == WAIT_OBJECT_0)
			{					
				//Take Mutex
				WaitForSingleObject(clientUDP->mutexhdlListen,INFINITE);
				if(clientUDP->CNRT_Listen->size() > 0)
				{
					cNRT_SU = clientUDP->CNRT_Listen->at(0);
					clientUDP->CNRT_Listen->erase(clientUDP->CNRT_Listen->begin());				
					ReleaseMutex(clientUDP->mutexhdlListen);
					//Release Mutex
					ReleaseSemaphore(emptyhdlListen, 1, NULL);//inc

					printf("Heard Confirm New Round Host");
					state++;
				}
				else
				{
					//resetting the semaphore and mutex to check again
					ReleaseMutex(clientUDP->mutexhdlListen);
					ReleaseSemaphore(fullhdlListen,1,NULL);
				}
				
			}
			if(cNRT_SU != NULL)
			{
				if(cNRT_SU->getGo() == true)
				{

					int iResult = WaitForSingleObject(emptyhdlSend, 50);
					if (iResult == WAIT_OBJECT_0)
					{
						//Take Mutex
						WaitForSingleObject(clientUDP->mutexhdlSend,INFINITE);
						ConfirmNewRoundTime* cNRT = new ConfirmNewRoundTime(
							hour,aMinute,aSeconds,true);
						clientUDP->CNRT_Send->push_back(cNRT);													
						ReleaseMutex(clientUDP->mutexhdlSend);
						//Release Mutex
						ReleaseSemaphore(fullhdlSend, 1, NULL);
						startTimer = true;
												
						eHour = hour;
						eMinute = aMinute;
						eSeconds = aSeconds;
						plusTimeMinutes(3,&eHour,&eMinute);

						printf("Start Time Agreed Send Host");

					}
				}
				else
				{
				}
			}
			else
			{
			}
			break;
		}
	}

	//there is no case 3
		
	timeFill(&hour,&minute,&seconds);
	//strcpy(storeTime,blank);
	if(startTimer == true)
	{
		short secs = differnceInSeconds(aMinute,aSeconds,minute,seconds);
		sprintf(storeTime," %d ",secs);

		if(secs == 0)
		{
			startTimer = false;
			roundStart = true;
		}
	}
	else
	{
		strcpy(storeTime,"WAITING");
	}

	glRasterPos2i(310, 142);
	strcpy(instruction1, "Starting In: ");
	strcat(instruction1,storeTime);
	length = strlen(instruction1);

	for (int i = 0; i < length; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, instruction1[i]);
	}


	glRasterPos2i(width-(length+80), 0);
	strcpy(instruction1, "Confirming Round");
	if (dot <= 3)
	{
		strcat(instruction1, ".  ");
	}
	else if (dot <= 6)
	{
		strcat(instruction1, ".. ");
	}
	else if(dot <= 9)
	{
		strcat(instruction1, "...");
	}
	else
	{
		dot = 0;
	}
	//int 
	length = strlen(instruction1);

	for (int i = 0; i < length; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, instruction1[i]);
	}
	dot++;

	glFlush();
	glutSwapBuffers();
		
}

//so listen for new round message 
//then confirm it if there is 15 seconds before the agreed time 
//else responed bool false
//if good wait for confirmation of host recieving it then count down
void newRoundClient()
{

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	switch(state)
	{
		case 1:
		{
			
			int iResult = WaitForSingleObject(fullhdlListen, 50);
			// should get this quick
			if (iResult == WAIT_OBJECT_0)
			{
				
				//Take Mutex
				WaitForSingleObject(clientUDP->mutexhdlListen,INFINITE);

				if(clientUDP->NR_Listen->size() > 0)
				{
					NewRound* nR = clientUDP->NR_Listen->at(0);
					clientUDP->NR_Listen->erase(clientUDP->NR_Listen->begin());
					ReleaseMutex(clientUDP->mutexhdlListen);
					//Release Mutex

					ReleaseSemaphore(emptyhdlListen, 1, NULL);//inc

					aMinute = nR->getTimeMinute();
					aSeconds = nR->getTimeSeconds();
					timeFill(&hour,&minute,&seconds);
					unsigned short nTime = -10;//seconds 10 before time
					minusTime(nTime,&minute,&seconds);

					state++;

					printf("\nHeard New Round Client\n");
				}
				else
				{
					ReleaseMutex(clientUDP->mutexhdlListen);
					ReleaseSemaphore(fullhdlListen,1,NULL);
				}
			}
			else
			{

			}	
			break;
		}

		case 2:
		{

			//enough time
			if(minute == aMinute)// is the minute the same
			{
				if(seconds < aSeconds) // is there more seconds before agreed time
				{
					timeAgreed = true;
					printf("good time1");

				}
			}
			else if(minute > aMinute)
			{
				//message recived to late
				timeAgreed = false;
				printf("bad time");
			}
			else if(minute < aMinute)
			{
				if(seconds < 40+aSeconds)//must be close to aSeconds(0-10)
				{
					timeAgreed = true;
					printf("good time2");
				}
			}




			int iResult = WaitForSingleObject(emptyhdlSend, 50);
			if (iResult == WAIT_OBJECT_0)
			{
				//there is no need for if because homePlayer is set by host
				ConfirmNewRoundTime* cNRT;
				if(timeAgreed == true)
				{
					state++;
				}
				else
				{
					state--;
				}

				//Take Mutex
				WaitForSingleObject(clientUDP->mutexhdlSend,INFINITE);
				cNRT = new ConfirmNewRoundTime(hour,aMinute,aSeconds,timeAgreed);
				clientUDP->CNRT_Send->push_back(cNRT);				
				ReleaseMutex(clientUDP->mutexhdlSend);
				//Release Mutex
				ReleaseSemaphore(fullhdlSend, 1, NULL);
				
				printf("\nSending Confirm New Round Client\n");
			}
			break;
		}

		case 3:
		{
			int iResult = WaitForSingleObject(fullhdlListen, 50); // should get this quick
			if (iResult == WAIT_OBJECT_0)
			{
				//Take Mutex
				WaitForSingleObject(clientUDP->mutexhdlListen,INFINITE);
				if(clientUDP->CNRT_Listen->size() > 0)
				{
					ConfirmNewRoundTime* cNRT = clientUDP->CNRT_Listen->at(0);
					clientUDP->CNRT_Listen->erase(clientUDP->CNRT_Listen->begin());				
					ReleaseMutex(clientUDP->mutexhdlListen);
					//Release Mutex

					ReleaseSemaphore(emptyhdlListen, 1, NULL);//inc

					if(cNRT->getGo() == true)
					{
						startTimer = true;
						printf("\nHeard Confirm New Round Client GS\n");
						
						eHour = hour;
						eMinute = aMinute;
						eSeconds = aSeconds;
						plusTimeMinutes(3,&eHour,&eMinute);
					}
					else
						printf("round didn't start");
					state++;
				}
				else
				{
					ReleaseMutex(clientUDP->mutexhdlListen);
					ReleaseSemaphore(fullhdlListen,1,NULL);
				}
			}
			break;
	
		}
	}

	timeFill(&hour,&minute,&seconds);

	if(startTimer == true)
	{
		short secs = differnceInSeconds(aMinute,aSeconds,minute,seconds);
		sprintf(storeTime," %d ",secs);

		if(secs == 0)
		{
			startTimer = false;
			roundStart = true;
		}
	}
	else
	{
		strcpy(storeTime,"WAITING");
	}

	glRasterPos2i(310, 142);
	strcpy(instruction1, "Starting In: ");
	strcat(instruction1,storeTime);
	length = strlen(instruction1);

	for (int i = 0; i < length; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, instruction1[i]);
	}

	glRasterPos2i(width-(length+80), 0);
	strcpy(instruction1, "Confirming Round");
	if (dot <= 3)
	{
		strcat(instruction1, ".  ");
	}
	else if (dot <= 6)
	{
		strcat(instruction1, ".. ");
	}
	else if(dot <= 9)
	{
		strcat(instruction1, "...");
	}
	else
	{
		dot = 0;
	}
	//int 
	length = strlen(instruction1);

	for (int i = 0; i < length; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, instruction1[i]);
	}
	dot++;

	glFlush();
	glutSwapBuffers();
	
	if(numberOfFailures > 10)
		endGame = true;
}

//confirm
int endProgram()
{
	////make sure to destroy clientTCP
	if(clientTCP != NULL)
	{
		delete(clientTCP);
	}

	////make sure to destroy serverTCP
	if(serverTCP != NULL)
	{
		delete(serverTCP);
	}

	////make sure to destroy clientUDP
	if(clientUDP != NULL)
	{
		//wait till 
		bool empty = false;
		do 
		{
			empty = clientUDP->listEmpty();
		}
		while(empty == false);

		delete(clientUDP);
	}

	////last command
	//glutDestroyWindow(glutGetWindow());
	return 1;
}