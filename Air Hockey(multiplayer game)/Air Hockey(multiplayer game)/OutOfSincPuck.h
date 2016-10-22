#pragma once
#ifndef OUTOFSINCPUCK_H
#define OUTOFSINCPUCK_H
#include"Vector2f.h"
#include"PackUnPackh.h"
class OutOfSincPuck
{
private:
	unsigned short timeHour;
	unsigned short timeMinute;
	unsigned short timeSeconds;
	Vector2f pucksLocation;
	Vector2f pucksShouldBeLocation;
public:
	OutOfSincPuck(unsigned short,unsigned short,unsigned short,Vector2f*,Vector2f*);
	OutOfSincPuck();
	~OutOfSincPuck();

	unsigned short PackPuckLocation(
		//the buffer it self
		unsigned char*);

	void UnPackPuckLocation(unsigned char*);
	unsigned short getTimeHour();
	unsigned short getTimeMinute();
	unsigned short getTimeSeconds();
	Vector2f getPuckLocation();
	Vector2f getPucksShouldBeLocation();
};

#endif