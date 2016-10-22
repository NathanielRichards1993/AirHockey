#include "sincPuck.h"

sincPuck::sincPuck(unsigned short hour,unsigned short minute,
				   unsigned short seconds,Vector2f* pucksLoc,
				   Vector2f* puckVel)
{
	timeHour = hour;
	timeMinute = minute;
	timeSeconds = seconds;
	puckNewLocation = Vector2f(pucksLoc);
	puckNewVelocity = Vector2f(puckVel);
}
sincPuck::sincPuck()
{
	timeHour = 0;
	timeMinute = 0;
	timeSeconds = 0;
	puckNewLocation = Vector2f(0,0);
	puckNewVelocity = Vector2f(0,0);
}

sincPuck::~sincPuck()
{
}


unsigned short sincPuck::PackSincPuck(
	//the buffer it self
	unsigned char*buff)
{
	buff+=4;//move buff past preamble
	
	//point x,y
	short wholeLX = puckNewLocation.getX();//could do as a byte
	float temp = wholeLX - (float)wholeLX;
	unsigned long mantisLX = temp*MANTISMUL;
	

	short wholeLY = puckNewLocation.getY();
	temp = puckNewLocation.getY()-(float)wholeLY;
	unsigned long mantisLY = temp*MANTISMUL;

	//point x,y
	short wholeVX = puckNewVelocity.getX();//could do as a byte
	temp = wholeVX - (float)wholeVX;
	unsigned long mantisVX = temp*MANTISMUL;
	

	short wholeVY = puckNewLocation.getY();
	temp = puckNewLocation.getY()-(float)wholeVY;
	unsigned long mantisVY = temp*MANTISMUL;


	//time
	PackShort(buff, timeHour);
	PackShort(buff, timeMinute);
	PackShort(buff, timeSeconds);
	//x,y
	PackShort(buff, wholeLX);
	PackLong(buff, mantisLX);
	PackShort(buff, wholeLY);
	PackLong(buff, mantisLY);


	unsigned short msgLen = sizeof(unsigned short)
		+ sizeof(unsigned short)//preamble

		+ sizeof(short)
		+ sizeof(short)
		+ sizeof(short)
		+ sizeof(unsigned long)
		+ sizeof(short)
		+ sizeof(unsigned long)
		+ sizeof(short);
	return msgLen;
}

void sincPuck::UnPackSincPuck(unsigned char* buff)
{
	buff+=4;//move buff past preamble

	UnPackShort(buff, timeHour);
	UnPackShort(buff, timeMinute);
	UnPackShort(buff, timeSeconds);

	float temp;
	UnPackShort(buff, temp);
	unsigned long tempMantissa = 0;
	UnPackLong(buff, tempMantissa);
	temp += (float)(tempMantissa / MANTISMUL);
	puckNewLocation.setX(temp);

	UnPackShort(buff, temp);
	tempMantissa = 0;
	UnPackLong(buff, tempMantissa);
	temp += (float)(tempMantissa / MANTISMUL);
	puckNewLocation.setY(temp);
}

unsigned short sincPuck::getTimeHour()
{
	return timeHour;
}
unsigned short sincPuck::getTimeMinute()
{
	return timeMinute;
}
unsigned short sincPuck::getTimeSeconds()
{
	return timeSeconds;
}
Vector2f sincPuck::getPuckLocation()
{
	return puckNewLocation;
}
