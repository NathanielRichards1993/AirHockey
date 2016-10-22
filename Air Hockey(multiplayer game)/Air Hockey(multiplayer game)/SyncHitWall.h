#pragma once
#ifndef SYNCHITWALL_H
#define SYNCHITWALL_H
#include"Vector2f.h"
#include"PackUnPackh.h"
class SyncHitWall
{
private:
	unsigned short timeHour;
	unsigned short timeMinute;
	unsigned short timeSeconds;
	Vector2f pointAtCollison;
	Vector2f newVelocity;
public:
	SyncHitWall(unsigned short,unsigned short,unsigned short,Vector2f*,Vector2f*);	//createing to send
	SyncHitWall();
	~SyncHitWall();
	
	//float is 8 bits and 23 bits

	unsigned short PackSyncHitWall(unsigned char*);

	void UnPackSyncHitWall(	unsigned char*);
	unsigned short getTimeHour();
	unsigned short getTimeMinute();
	unsigned short getTimeSeconds();
	Vector2f getPointCollison();
	Vector2f getNewVelocity();
};

#endif