#include "ConfirmNewRoundTime.h"


ConfirmNewRoundTime::ConfirmNewRoundTime(unsigned short hour,unsigned short minute,
										 unsigned short seconds,bool Go)
{
	timeNewRoundHour = hour;
	timeNewRoundMinute = minute;
	timeNewRoundSeconds = seconds;
	go = Go;
}

ConfirmNewRoundTime::ConfirmNewRoundTime()
{
	timeNewRoundHour = 0;
	timeNewRoundMinute = 0;
	timeNewRoundSeconds = 0;
	go = false;
}

ConfirmNewRoundTime::~ConfirmNewRoundTime()
{
}

unsigned short ConfirmNewRoundTime::PackConfirmNewRoundTime(unsigned char* buff)//buff
{
	buff+=4;

	//time
	PackShort(buff,timeNewRoundHour);
	PackShort(buff,timeNewRoundMinute);
	PackShort(buff,timeNewRoundSeconds);
	PackByte(buff, go);

	unsigned short msgLen = sizeof(unsigned short)
		+ sizeof(unsigned short)//preamble
		
		+ sizeof(unsigned short)
		+ sizeof(unsigned short)
		+ sizeof(unsigned short)
		+ sizeof(bool);

	return msgLen;
}

void ConfirmNewRoundTime::UnPackConfirmNewRoundTime(unsigned char* buff)
{
	buff+=4;
	UnPackShort(buff, timeNewRoundHour);
	UnPackShort(buff, timeNewRoundMinute);
	UnPackShort(buff, timeNewRoundSeconds);
	UnPackByte(buff, go);

}

unsigned short ConfirmNewRoundTime::getTimeHour()
{
	return timeNewRoundHour;
}

unsigned short ConfirmNewRoundTime::getTimeMinute()
{
	return timeNewRoundMinute;
}

unsigned short ConfirmNewRoundTime::getTimeSeconds()
{
	return timeNewRoundSeconds;
}

bool ConfirmNewRoundTime::getGo()
{
	return go;
}