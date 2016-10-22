#include "OutOfSincPuck.h"


OutOfSincPuck::OutOfSincPuck(unsigned short hour, unsigned short minute, unsigned short seconds, Vector2f* puckLoc,Vector2f* pucksBeLocation)
{
	timeHour = hour;
	timeMinute = minute;
	timeSeconds = seconds;
	pucksLocation = Vector2f(puckLoc);
	pucksShouldBeLocation = Vector2f(pucksBeLocation);
}
OutOfSincPuck::OutOfSincPuck()
{
	timeHour = 0;
	timeMinute = 0;
	timeSeconds = 0;
	pucksLocation = Vector2f(0,0);
	pucksShouldBeLocation = Vector2f(0,0);
}

OutOfSincPuck::~OutOfSincPuck()
{
}
/*Pack preamble before packing using this function*/
/*Pucks location is client side*/
/*this means for host and client they both send their x and y location
as the first x and y*/
unsigned short OutOfSincPuck::PackPuckLocation(unsigned char* buff)
{
	buff+=4;//move buff past preamble

	//point x,y
	short puckX = pucksLocation.getX();
	float temp = pucksLocation.getX() - (float)puckX;
	unsigned long puckXM = temp/MANTISMUL;

	short puckY = pucksLocation.getY();
	temp = pucksLocation.getY() - (float)puckY;
	unsigned long puckYM = temp/MANTISMUL;
	//velocity x,y
	short puckVelX = pucksShouldBeLocation.getX();
	temp = pucksShouldBeLocation.getX() - (float)puckVelX;
	unsigned long puckVelXM = temp/MANTISMUL;

	short puckVelY = pucksShouldBeLocation.getY();
	temp = pucksShouldBeLocation.getY() - (float)puckVelY;
	unsigned long puckVelYM = temp/MANTISMUL;

	PackShort(buff, timeHour);
	PackShort(buff, timeMinute);
	PackShort(buff, timeSeconds);

	PackShort(buff, puckX);
	PackLong(buff, puckXM);
	PackShort(buff, puckY);
	PackLong(buff, puckYM);

	PackShort(buff, puckVelX);
	PackLong(buff, puckVelXM);
	PackShort(buff, puckVelY);
	PackLong(buff, puckVelYM);

	unsigned short msgLen = sizeof(unsigned short)
		+ sizeof(unsigned short)//preamble

		+ sizeof(short)
		+ sizeof(short)
		+ sizeof(short)

		+ sizeof(short)
		+ sizeof(unsigned long)
		+ sizeof(short)
		+ sizeof(unsigned long)

		+ sizeof(short)
		+ sizeof(unsigned long)
		+ sizeof(short)
		+ sizeof(unsigned long);
	return msgLen;
}

void OutOfSincPuck::UnPackPuckLocation(unsigned char* buff)
{
	buff+=4;//move buff past preamble
	//time
	UnPackShort(buff, timeHour);
	UnPackShort(buff, timeMinute);
	UnPackShort(buff, timeSeconds);

	//point x,y  //never minse short long is fine
	float temp;
	UnPackShort(buff, temp);
	unsigned long tempMantissa = 0;
	UnPackLong(buff, tempMantissa);
	temp += (float)(tempMantissa / MANTISMUL);
	pucksLocation.setX(temp);

	UnPackShort(buff, temp);
	tempMantissa = 0;
	UnPackLong(buff, tempMantissa);
	temp += (float)(tempMantissa / MANTISMUL);
	pucksLocation.setY(temp);
	
	//point x,y 
	UnPackShort(buff, temp);
	tempMantissa = 0;
	UnPackLong(buff, tempMantissa);
	temp += (float)(tempMantissa / MANTISMUL);
	pucksShouldBeLocation.setX(temp);

	UnPackShort(buff, temp);
	tempMantissa = 0;
	UnPackLong(buff, tempMantissa);
	temp += (float)(tempMantissa / MANTISMUL);
	pucksShouldBeLocation.setY(temp);
}

unsigned short OutOfSincPuck::getTimeHour()
{
	return timeHour;
}
unsigned short OutOfSincPuck::getTimeMinute()
{
	return timeMinute;
}
unsigned short OutOfSincPuck::getTimeSeconds()
{
	return timeSeconds;
}
Vector2f OutOfSincPuck::getPuckLocation()
{
	return pucksLocation;
}
Vector2f OutOfSincPuck::getPucksShouldBeLocation()
{
	return pucksShouldBeLocation;
}