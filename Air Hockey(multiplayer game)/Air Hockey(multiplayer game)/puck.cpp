#include "puck.h"


puck::puck(Vector2f* loc,Vector2f* vel ,int pSize)
{
	location = loc;
	velocity = vel;
	sizeOfPuck = pSize;
}


puck::~puck()
{
}


bool puck::updatePuck(float width,float height)
{
	if (location->getX() + velocity->getX() > width||
		location->getX() + velocity->getX() < 0)
	{
		velocity->setX((velocity->getX()*-1));
		return true;
	}
	if (location->getY() + velocity->getY() > height ||
		location->getY() + velocity->getY() < 0)
	{
		velocity->setY((velocity->getY()*-1));
		return true;
	}
	location->setX(location->getX() + velocity->getX());
	location->setY(location->getY() + velocity->getY());
	return false;
}

void puck::playerHitPuck(Vector2f* PlayerLocation)
{

}

Vector2f* puck::getPucksLocation()
{
	return location;
}

void puck::setPuckVelocity(Vector2f* vel)
{
	velocity->setX(vel->getX());
	velocity->setY(vel->getY());
}

Vector2f* puck::getPuckVelocty()
{
	return velocity;
}

void puck::setPucksLocation(Vector2f* loc)
{
	location->setX(loc->getX());
	location->setY(loc->getY());
}
/*top of screen is the higher value*/
bool puck::goalScored(float lengthOfPitch,float topPost,float bottomPost)
{

		if(location->getX() < sizeOfPuck)// less than 15
			if(location->getY() < topPost &&
				location->getY() > bottomPost)
				return true;//within goal zone
			else
			{return false;}//not near the goal zone
		else if(location->getX() > (lengthOfPitch-sizeOfPuck))
			if(location->getY() < topPost &&
				location->getY() > bottomPost)
				return true;//within goal zone
			else
			{return false;}//not near the goal zone
		else
		{return false;}//not near any
}
