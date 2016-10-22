#pragma once
#ifndef CONFIRMNEWROUNDTIME_H
#define CONFIRMNEWROUNDTIME_H
#include"PackUnPackh.h"
#include<string.h>
class ConfirmNewRoundTime
{
private:
	unsigned short timeNewRoundHour;
	unsigned short timeNewRoundMinute;
	unsigned short timeNewRoundSeconds;
	bool go;
public:
	ConfirmNewRoundTime(unsigned short,unsigned short,unsigned short, bool);
	ConfirmNewRoundTime();
	~ConfirmNewRoundTime();

	unsigned short PackConfirmNewRoundTime(unsigned char*);//buff

	void UnPackConfirmNewRoundTime(unsigned char*);//buff
	unsigned short getTimeHour();
	unsigned short getTimeMinute();
	unsigned short getTimeSeconds();
	bool getGo();
};

#endif