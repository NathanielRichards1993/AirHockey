#include "Message_HitPuck.h"

Message_HitPuck::Message_HitPuck(unsigned short time,
	Vector2f* player,Vector2f* puckLoc,Vector2f* puckVel)
{
	timeStamp = time;
	LocationOfPlayer = player;
	puckHitLocation = puckLoc;
	pucksNewVelocity = puckVel;
}
Message_HitPuck::Message_HitPuck()
{}

Message_HitPuck::~Message_HitPuck()
{
	//delete(LocationOfPlayer);
	//delete(puckHitLocation);
	//delete(pucksNewVelocity);
	//should need to do this since value will actuall be used by game
}


unsigned short Message_HitPuck::PackMessageHitPuck(unsigned char* buff)
{
	//time
	unsigned short time = timeStamp;
	float temp = timeStamp - (float)time;
	unsigned long timeMantissa = temp*MANTISMUL;

	//x,y of location Opponent
	unsigned short playerX = LocationOfPlayer->getX();
	temp = LocationOfPlayer->getX() - playerX;
	unsigned long playerXM = temp*MANTISMUL; 

	unsigned short playerY = LocationOfPlayer->getY();
	temp = LocationOfPlayer->getY() - playerY;
	unsigned long playerYM = temp*MANTISMUL;

	//x,y of puck location
	unsigned short puckLocX = puckHitLocation->getX();
	temp = puckHitLocation->getX() - puckLocX;
	unsigned long puckLocXM = temp*MANTISMUL;

	unsigned short puckLocY = puckHitLocation->getY();
	temp = puckHitLocation->getY() - puckLocY;
	unsigned long puckLocYM = temp*MANTISMUL;

	//x,y of new puck velocity
	unsigned short puckVolX = pucksNewVelocity->getX();
	temp = pucksNewVelocity->getX() - puckVolX;
	unsigned long puckVolXM = temp*MANTISMUL;

	unsigned short puckVolY = pucksNewVelocity->getY();
	temp = pucksNewVelocity->getY() - puckVolY;
	unsigned long puckVolYM = temp*MANTISMUL;

	//time
	PackShort(buff, time);
	PackLong(buff, timeMantissa);
	// player x,y
	PackShort(buff, playerX);
	PackLong(buff, playerXM);
	PackShort(buff, playerY);
	PackLong(buff, playerYM);
	// pucks x,y
	PackShort(buff, puckLocX);
	PackLong(buff, puckLocXM);
	PackShort(buff, puckLocY);
	PackLong(buff, puckLocYM);
	//pucks vel
	PackShort(buff, puckVolX);
	PackLong(buff, puckVolXM);
	PackShort(buff, puckVolY);
	PackLong(buff, puckVolYM);


	unsigned short msgLen = 
		+sizeof(unsigned long) + sizeof(unsigned short)
		+ sizeof(unsigned long) + sizeof(unsigned short)
		+ sizeof(unsigned long)+ sizeof(unsigned short)//
		+ sizeof(unsigned long) + sizeof(unsigned short)
		+ sizeof(unsigned long) + sizeof(unsigned short)
		+ sizeof(unsigned long) + sizeof(unsigned short)//
		+ sizeof(unsigned long) + sizeof(unsigned short);
	//7 lines = 2 short and long
	return msgLen;

}

void Message_HitPuck::UnPackMessageHitPuck(unsigned char* buff)
{
	//time
	UnPackShort(buff, timeStamp);
	unsigned long timeMantissa = 0;
	UnPackLong(buff, timeMantissa);
	timeStamp += (timeMantissa / MANTISMUL);

	//opponent
	float temp;
	UnPackShort(buff, temp);
	unsigned long tempMantissa = 0;
	UnPackLong(buff, tempMantissa);
	temp += (tempMantissa / MANTISMUL);
	LocationOfPlayer->setX(temp);

	UnPackShort(buff, temp);
	tempMantissa = 0;
	UnPackLong(buff, tempMantissa);
	temp += (tempMantissa / MANTISMUL);
	LocationOfPlayer->setY(temp);

	//puck loc
	UnPackShort(buff, temp);
	tempMantissa = 0;
	UnPackLong(buff, tempMantissa);
	temp += (tempMantissa / MANTISMUL);
	puckHitLocation->setX(temp);

	UnPackShort(buff, temp);
	tempMantissa = 0;
	UnPackLong(buff, tempMantissa);
	temp += (tempMantissa / MANTISMUL);
	puckHitLocation->setY(temp);

	//puck vol
	UnPackShort(buff, temp);
	tempMantissa = 0;
	UnPackLong(buff, tempMantissa);
	temp += (tempMantissa / MANTISMUL);
	pucksNewVelocity->setX(temp);

	UnPackShort(buff, temp);
	tempMantissa = 0;
	UnPackLong(buff, tempMantissa);
	temp += (tempMantissa / MANTISMUL);
	pucksNewVelocity->setY(temp);
}