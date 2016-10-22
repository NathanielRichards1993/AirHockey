#include "PlayerNewLocation.h"


PlayerNewLocation::PlayerNewLocation(unsigned short hour, unsigned short minute, 
									 unsigned short seconds,Vector2f* player,
									 Vector2f* velOfPlayer)
{
	timeHour = hour;
	timeMinute = minute;
	timeSeconds = seconds;
	locationOfPlayer = player;
	velocityOfPlayer = velOfPlayer;
}
PlayerNewLocation::PlayerNewLocation()
{
	timeHour = 0;
	timeMinute = 0;
	timeSeconds = 0;
	locationOfPlayer = Vector2f(-1, -1);
	velocityOfPlayer = Vector2f(-1,-1);
}

PlayerNewLocation::~PlayerNewLocation()
{
}

unsigned short PlayerNewLocation::PackPlayerNewLocation(unsigned char* buff)
{
	buff+=4;//move buff past preamble

	//x,y of location 
	short playerLX = locationOfPlayer.getX();
	float temp = locationOfPlayer.getX() - playerLX;
	unsigned long playerLXM = temp*MANTISMUL;

	short playerLY = locationOfPlayer.getY();
	temp = locationOfPlayer.getY() - playerLY;
	unsigned long playerLYM = temp*MANTISMUL;

	//x,y of velocity

	short playerVX = (velocityOfPlayer.getX());
	temp = (velocityOfPlayer.getX()) - (playerVX);
	if(temp < 0) temp*=-1;
	long unsigned playerVXM = temp*MANTISMUL;

	short playerVY = (velocityOfPlayer.getY());
	temp = (velocityOfPlayer.getY()) - (playerVY);
	if(temp < 0)temp*=-1;
	unsigned long playerVYM = temp*MANTISMUL;

	//if exactly 1, -1 or zero then leave else
	//state the plus or minus using -2 or 2
	if(velocityOfPlayer.getX() == 0 ||
		velocityOfPlayer.getX() == -1||
		velocityOfPlayer.getX() == 1)
	{
	}
	else
	{
		if(velocityOfPlayer.getX() < 0)
			playerVX = -2;
		else
			playerVX = 2;
	}

	//if exactly 1, -1 or zero then leave else
	//state the plus or minus using -2 or 2
	if(velocityOfPlayer.getY() == 0 ||
		velocityOfPlayer.getY() == -1||
		velocityOfPlayer.getY() == 1)
	{
	}
	else
	{
		if(velocityOfPlayer.getY() < 0)
			playerVY = -2;
		else
			playerVY = 2;
	}

	//time
	PackShort(buff, timeHour);
	PackShort(buff, timeMinute);
	PackShort(buff, timeSeconds);
	// player location x,y
	PackShort(buff, playerLX);
	PackLong(buff, playerLXM);
	PackShort(buff, playerLY);
	PackLong(buff, playerLYM);

	// player velocity x,y
	PackShort(buff, playerVX);
	PackLong(buff, playerVXM);
	PackShort(buff, playerVY);
	PackLong(buff, playerVYM);

	unsigned short msgLen = sizeof(unsigned short)
		+ sizeof(unsigned short)//preamble

		+ sizeof(unsigned short) + sizeof(unsigned short) + sizeof(unsigned short)

		+ sizeof(unsigned long) + sizeof(short)
		+ sizeof(unsigned long) + sizeof(short)

		+ sizeof(unsigned short) + sizeof(short)
		+ sizeof(unsigned short) + sizeof(short);

	return msgLen;

}

void PlayerNewLocation::UnPackPlayerNewLocation(unsigned char* buff)
{
	buff+=4;//move buff past preamble
	//time
	UnPackShort(buff, timeHour);
	UnPackShort(buff, timeMinute);
	UnPackShort(buff, timeSeconds);

	//opponent
	float temp;
	UnPackShort(buff, temp);
	unsigned long tempMantissa = 0;
	UnPackLong(buff, tempMantissa);
	temp += (float)(tempMantissa / MANTISMUL);
	locationOfPlayer.setX(temp);

	UnPackShort(buff, temp);
	tempMantissa = 0;
	UnPackLong(buff, tempMantissa);
	temp += (float)(tempMantissa / MANTISMUL);
	locationOfPlayer.setY(temp);

	UnPackShort(buff, temp);
	tempMantissa = 0;
	UnPackLong(buff, tempMantissa);
	float tempM = (float)(tempMantissa);
	tempM /= MANTISMUL;
	//values only possable if there is no 
	//mantissa. not likely to happen
	if(temp == 0 || temp == 1 || temp == -1)
	{
	}
	else
	{
		if(temp == 2)
		{
			temp = tempM;
		}
		else if(temp == -2)
		{
			temp = tempM*-1;
		}
		else
		{
			//printf("message recieved incorrectly player new unpack");
		}
	}
	velocityOfPlayer.setX(temp);

	UnPackShort(buff, temp);
	tempMantissa = 0;
	UnPackLong(buff, tempMantissa);
	tempM = (float)(tempMantissa);
	tempM /= MANTISMUL;

	if(temp == 0 || temp == 1 || temp == -1)
	{
	}
	else
	{
		if(temp == 2)
		{
			temp = tempM;
		}
		else if(temp == -2)
		{
			temp = tempM*-1;
		}
		else
		{
			//printf("message recieved incorrectly player new unpack");
		}
	}
	velocityOfPlayer.setY(temp);
}

Vector2f PlayerNewLocation::getPlayerLocation()
{
	return locationOfPlayer;
}
Vector2f PlayerNewLocation::getPlayerVelocity()
{
	return velocityOfPlayer;
}
unsigned short PlayerNewLocation::getTimeHour()
{
	return timeHour;
}
unsigned short PlayerNewLocation::getTimeMinute()
{
	return timeMinute;
}
unsigned short PlayerNewLocation::getTimeSeconds()
{
	return timeSeconds;
}