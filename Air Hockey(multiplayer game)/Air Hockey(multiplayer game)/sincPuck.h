#pragma once
#ifndef SINCPUCK_H
#define SINCPUCK_H
#include"Vector2f.h"
#include"PackUnPackh.h"
class sincPuck
{
private:
	unsigned short timeHour;
	unsigned short timeMinute;
	unsigned short timeSeconds;
	Vector2f puckNewLocation;
	Vector2f puckNewVelocity;
public:
	sincPuck(unsigned short,unsigned short,unsigned short,Vector2f*,Vector2f*);
	sincPuck();
	~sincPuck();

	unsigned short PackSincPuck(
		//the buffer it self
		unsigned char*);

	void UnPackSincPuck(
		//the buffer it self
		unsigned char*);

	unsigned short getTimeHour();
	unsigned short getTimeMinute();
	unsigned short getTimeSeconds();
	Vector2f getPuckLocation();
	Vector2f getPuckVelocity();
};

#endif