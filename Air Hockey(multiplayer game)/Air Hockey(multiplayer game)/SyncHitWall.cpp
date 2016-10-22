#include "SyncHitWall.h"


SyncHitWall::SyncHitWall(unsigned short hour,unsigned short minute,unsigned short seconds,
						 Vector2f* point,Vector2f* vel)
{
	timeHour = hour;
	timeMinute = minute;
	timeSeconds = seconds;
	pointAtCollison = Vector2f(point);
	newVelocity = Vector2f(vel);
}
SyncHitWall::SyncHitWall()
{
	timeHour = 0;
	timeMinute = 0;
	timeSeconds = 0;
	pointAtCollison = new Vector2f();
	newVelocity = new Vector2f();
}

SyncHitWall::~SyncHitWall()
{
	//delete(pointAtCollison);
	//delete(newVelocity);
}

unsigned short SyncHitWall::PackSyncHitWall(unsigned char* buff)
{
	buff+=4;//move buff past preamble
	
	//point x,y
	short puckX = pointAtCollison.getX();
	float temp = pointAtCollison.getX() - (float)puckX;
	unsigned long puckXM = temp*MANTISMUL;

	short puckY = pointAtCollison.getY();
	temp = pointAtCollison.getY() - (float)puckY;
	unsigned long puckYM = temp*MANTISMUL;

	//velocity x,y
	short puckXVel = newVelocity.getX();
	temp = newVelocity.getX() - (float)puckXVel;
	unsigned long puckXMVel = temp*MANTISMUL;

	short puckYVel = newVelocity.getY();
	temp = newVelocity.getY() - (float)puckYVel;
	unsigned long puckYMVel = temp*MANTISMUL;

	PackShort(buff, timeHour);
	PackShort(buff, timeMinute);
	PackShort(buff, timeSeconds);

	PackShort(buff, puckX);
	PackLong(buff, puckXM);
	PackShort(buff, puckY);
	PackLong(buff, puckYM);

	PackShort(buff, puckXVel);
	PackLong(buff, puckXMVel);
	PackShort(buff, puckYVel);
	PackLong(buff, puckYMVel);

	unsigned short msgLen =sizeof(unsigned short)
		+ sizeof(unsigned short)//preamble//4
		
		+ sizeof(unsigned short)
		+ sizeof(unsigned short) + sizeof(unsigned short)//10

		+ sizeof(unsigned long) + sizeof(unsigned short)
		+ sizeof(unsigned long) + sizeof(unsigned short)//22

		+ sizeof(unsigned long) + sizeof(unsigned short)
		+ sizeof(unsigned long) + sizeof(unsigned short);//34
	return msgLen;
}

void SyncHitWall::UnPackSyncHitWall(unsigned char* buff)
{
	buff+=4;//move buff past preamble
	UnPackShort(buff, timeHour);
	UnPackShort(buff, timeMinute);
	UnPackShort(buff, timeSeconds);

	//point x,y
	float temp = 0;
	UnPackShort(buff, temp);
	unsigned long tempMantissa = 0;
	UnPackLong(buff, tempMantissa);
	temp += (float)(tempMantissa / MANTISMUL);
	pointAtCollison.setX(temp);

	temp = 0;
	UnPackShort(buff, temp);
	tempMantissa = 0;
	UnPackLong(buff, tempMantissa);
	temp += (float)(tempMantissa / MANTISMUL);
	pointAtCollison.setY(temp);

	//vel x,y
	short tempS = 0;

	temp = 0;
	UnPackShort(buff, tempS);
	temp = tempS;
	tempMantissa = 0;
	UnPackLong(buff, tempMantissa);
	temp += (float)(tempMantissa / MANTISMUL);
	newVelocity.setX(temp);


	tempS = 0;
	UnPackShort(buff, tempS);
	temp = tempS;
	tempMantissa = 0;
	UnPackLong(buff, tempMantissa);
	temp += (float)(tempMantissa / MANTISMUL);
	newVelocity.setY(temp);
}

unsigned short SyncHitWall::getTimeHour()
{
	return timeHour;
}
unsigned short SyncHitWall::getTimeMinute()
{
	return timeMinute;
}
unsigned short SyncHitWall::getTimeSeconds()
{
	return timeSeconds;
}
Vector2f SyncHitWall::getPointCollison()
{
	return pointAtCollison;
}
Vector2f SyncHitWall::getNewVelocity()
{
	return newVelocity;
}