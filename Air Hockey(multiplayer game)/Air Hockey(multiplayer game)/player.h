#pragma once
#ifndef PLAYER_H
#define PLAYER_H
#include"Vector2f.h"
#include"puck.h"
class player
{
private:
	Vector2f* location;
	Vector2f* velocity;
	Vector2f* goalLocation;
	int sizeOfPuck;
	unsigned short score;
	bool host;
public:
	player(Vector2f*,int);
	~player();
	bool collisonWithPuck(puck*);
	void setPlayerLocation(Vector2f*);
	void setPlayerVelocity(Vector2f*);

	Vector2f* getPlayerLocation();
	Vector2f* getPlayerVelocity();

	void setHost(bool);
	bool getHost(void);

	void setGoalLocation(Vector2f*);
	/*mouse*/
	void moveTowardsMouse(Vector2f*,float);
	bool updateLocation();
	void increamentScore();
	unsigned int getScore();
};

#endif