#include "player.h"
#include<math.h>

player::player(Vector2f* loc, int size)
{
	location = loc;
	sizeOfPuck = size;
	velocity = new Vector2f(0, 0);
	score = 0;
}


player::~player()
{
}

bool player::collisonWithPuck(puck* Puck)
{
	Vector2f* coord = Puck->getPucksLocation();
	Vector2f* vel = Puck->getPuckVelocty();
	Vector2f* temp = new Vector2f();
	bool out = false;
	if ((location->getX() + sizeOfPuck) < (coord->getX() - sizeOfPuck))
	{
		out = false;
	}
	else if ((location->getX() - sizeOfPuck) > (coord->getX() + sizeOfPuck))
	{
		out = false;
	}
	else if ((location->getY() + sizeOfPuck) < (coord->getY() - sizeOfPuck))
	{
		out = false;
	}
	else if ((location->getY() - sizeOfPuck) > (coord->getY() + sizeOfPuck))
	{
		out = false;
	}
	else
	{
		if ((location->getX() + sizeOfPuck) > (coord->getX() - sizeOfPuck))
		{
			temp->setX((vel->getX() * -1));
			temp->setY(vel->getY());
			Puck->setPuckVelocity(temp);
		}
		else if ((location->getX() - sizeOfPuck) < (coord->getX() + sizeOfPuck))
		{
			temp->setX((vel->getX() * -1));
			temp->setY(vel->getY());
			Puck->setPuckVelocity(temp);
		}
		if ((location->getY() + sizeOfPuck) > (coord->getY() - sizeOfPuck))
		{
			temp->setX((vel->getX()));
			temp->setY(vel->getY()*-1);
			Puck->setPuckVelocity(temp);
		}
		else if ((location->getY() - sizeOfPuck) < (coord->getY() + sizeOfPuck))
		{
			temp->setX((vel->getX()));
			temp->setY(vel->getY()*-1);
			Puck->setPuckVelocity(temp);
		}
		//don't think i need this line *1*
		//Puck->updatePuck(800,300);
		return true;
	}
	return false;
}

Vector2f* player::getPlayerLocation()
{
	return location;
}

Vector2f* player::getPlayerVelocity()
{
	return velocity;
}

void player::setHost(bool host)
{
	this->host = host;
}

bool player::getHost()
{
	return host;
}

void player::setGoalLocation(Vector2f* location)
{
	goalLocation = location;
}

void player::moveTowardsMouse(Vector2f* mouse,float distance)
{
	float xDistance = location->getX() - goalLocation->getX();
	float yDistance = location->getY() - goalLocation->getY();
	xDistance *= xDistance;
	yDistance *= yDistance;

	float length = sqrt((xDistance + yDistance));

	if (length < (distance*2))
	{
		xDistance = mouse->getX() - location->getX();
		yDistance = mouse->getY() - location->getY();

		length = sqrt(((xDistance*xDistance) + (yDistance*yDistance)));

		float xLoc = xDistance / length;
		float yLoc = yDistance / length;

		xDistance = location->getX() + xLoc;
		yDistance = location->getY() + yLoc;

		xDistance = xDistance - goalLocation->getX();
		yDistance = yDistance - goalLocation->getY();

		xDistance *= xDistance;
		yDistance *= yDistance;

		length = sqrt((xDistance + yDistance));
		if (length < (distance*2))
		{
			velocity->setX(xLoc);
			velocity->setY(-yLoc);
		}
	}
	else
	{
		//velocity->setX(10);
		//velocity->setY(0);
	}

}

bool player::updateLocation()
{
	
	location->setX(location->getX() + velocity->getX());
	location->setY(location->getY() + velocity->getY());
	bool move = false;
	if (velocity->getX() < 0.2)
		velocity->setX(0);
	else
	{
		float v = velocity->getX()*0.9;
		velocity->setX(v);
		move = true;
	}

	if (velocity->getY() < 0.2)
		velocity->setY(0);
	else
	{
		float v = velocity->getY()*0.9;
		velocity->setY(v);
		move = true;
	}

	return move;
	
}

void player::setPlayerLocation(Vector2f* vec)
{
	location->setX(vec->getX());
	location->setY(vec->getY());
}

void player::setPlayerVelocity(Vector2f* vel)
{
	location->setX(vel->getX());
	location->setY(vel->getY());
}

void player::increamentScore()
{
	score++;
}

unsigned int player::getScore()
{
	return score;
}