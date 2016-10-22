#include "NewRound.h"

/*hour,minute,seconds,home score,opp score*/
NewRound::NewRound(unsigned short hour, unsigned short minute, unsigned short seconds,unsigned short hpScore,unsigned short opScore)
{
	timeNewRoundHour = hour;
	timeNewRoundMinute = minute;
	timeNewRoundSeconds = seconds;
	homePlayerScore = hpScore;
	opponentScore = opScore;
}
NewRound::NewRound()
{
	timeNewRoundHour = 0;
	timeNewRoundMinute = 0;
	timeNewRoundSeconds = 0;
	homePlayerScore = 0;
	opponentScore = 0;
}

NewRound::~NewRound()
{
}
/*Pack this first then preamble there wll be space for packing 2 shorts*/
unsigned short NewRound::PackNewRound(unsigned char* buff)
{
	buff+=4;//move buff past preamble

	PackShort(buff, timeNewRoundHour);
	PackShort(buff,timeNewRoundMinute);
	PackShort(buff,timeNewRoundSeconds);
	PackShort(buff, homePlayerScore);
	PackShort(buff, opponentScore);

	unsigned short msgLen = sizeof(unsigned short)
		+ sizeof(unsigned short)//preamble
		
		+ sizeof(unsigned short)
		+ sizeof(unsigned short)
		+ sizeof(unsigned short)
		+ sizeof(unsigned short)
		+ sizeof(unsigned short);
	return msgLen;
}

void NewRound::UnPackNewRound(unsigned char* buff)
{
	buff+=4;//move buff past preamble
	UnPackShort(buff, timeNewRoundHour);
	UnPackShort(buff,timeNewRoundMinute);
	UnPackShort(buff,timeNewRoundSeconds);
	UnPackShort(buff, homePlayerScore);
	UnPackShort(buff, opponentScore);
}

unsigned short NewRound::getTimeHour()
{
	return timeNewRoundHour;
}

unsigned short NewRound::getTimeMinute()
{
	return timeNewRoundMinute;
}

unsigned short NewRound::getTimeSeconds()
{
	return timeNewRoundSeconds;
}

unsigned short NewRound::getHomeScore()
{
	return homePlayerScore;
}

unsigned short NewRound::getOpponentScore()
{
	return opponentScore;
}
