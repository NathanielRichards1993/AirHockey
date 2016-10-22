#pragma once
#ifndef PLAYERNEWLOCATION_H
#define PLAYERNEWLOCATION_H
#include"Vector2f.h"
#include"PackUnPackh.h"
class PlayerNewLocation
{
	unsigned short timeHour;
	unsigned short timeMinute;
	unsigned short timeSeconds;
	Vector2f locationOfPlayer;
	Vector2f velocityOfPlayer;
public:
	/*hour,minute,seconds,player location, player velocity*/
	PlayerNewLocation(unsigned short,unsigned short,unsigned short,
		Vector2f*,Vector2f*);
	PlayerNewLocation();
	~PlayerNewLocation();

	unsigned short PackPlayerNewLocation(unsigned char*);

	void UnPackPlayerNewLocation(unsigned char*);
	Vector2f getPlayerLocation();
	Vector2f getPlayerVelocity();
	unsigned short getTimeHour();
	unsigned short getTimeMinute();
	unsigned short getTimeSeconds();
};
#endif
