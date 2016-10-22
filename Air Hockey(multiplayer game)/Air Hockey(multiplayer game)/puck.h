#pragma once
#ifndef PUCK_H
#define PUCK_H
#include"Vector2f.h"
class puck
{
private:
	Vector2f* location;
	Vector2f* velocity;
	int sizeOfPuck;
public:
	puck(Vector2f*,Vector2f*,int);
	~puck();

	bool updatePuck(float,float);
	void playerHitPuck(Vector2f*);
	Vector2f* getPucksLocation();
	Vector2f* getPuckVelocty();
	void setPuckVelocity(Vector2f*);
	void setPucksLocation(Vector2f*);
	/*
	1st is length of x-axis
	2nd is top goal post, 3rd is the bottom goal post*/
	bool goalScored(float,float,float);

};

#endif