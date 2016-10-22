#pragma once
#ifndef MESSAGEHITPUCK_H
#define MESSAGEHITPUCK_H
#include"Vector2f.h"
#include"PackUnPackh.h"
class Message_HitPuck
{
private:
	float timeStamp;
	Vector2f* LocationOfPlayer;
	Vector2f* puckHitLocation;
	Vector2f* pucksNewVelocity;
public:
	Message_HitPuck(unsigned short,Vector2f*,Vector2f*,Vector2f*);
	Message_HitPuck();
	~Message_HitPuck();
	unsigned short getTimeStamp();
	Vector2f* getLocationOfPlayer();
	Vector2f* getPuckHitLocation();
	Vector2f* getpucksNewVelocity();

	unsigned short PackMessageHitPuck(unsigned char*);

	void UnPackMessageHitPuck(unsigned char*);
};

#endif