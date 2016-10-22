#pragma once
#ifndef NEWROUND_H
#define NEWROUND_H
#include"PackUnPackh.h"
class NewRound
{
private:
	unsigned short timeNewRoundHour;
	unsigned short timeNewRoundMinute;
	unsigned short timeNewRoundSeconds;
	unsigned short homePlayerScore;
	unsigned short opponentScore;
public:

	NewRound(unsigned short,unsigned short,unsigned short,
		unsigned short,unsigned short);
	NewRound();
	~NewRound();

	unsigned short PackNewRound(unsigned char*);//buff

	void UnPackNewRound(unsigned char*);//buff
	unsigned short getTimeHour();
	unsigned short getTimeMinute();
	unsigned short getTimeSeconds();
	unsigned short getHomeScore();
	unsigned short getOpponentScore();
};

#endif