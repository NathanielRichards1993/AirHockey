
#include "Vector2f.h"
	//construct
Vector2f::Vector2f()
{
	setX(0);
	setY(0);

}
Vector2f::Vector2f(float myX, float myY)
{ 
	setX(myX);
	setY(myY);

}
Vector2f::Vector2f(Vector2f* cpy)
{
	x = cpy->getX();
	y = cpy->getY();
}

void Vector2f::operator =(const Vector2f& cpy)
{
	x = cpy.x;
	y = cpy.y;
}

void Vector2f::operator +=(const Vector2f& other)
{
	x += other.x;
	y += other.y;
}

Vector2f Vector2f::operator +(const Vector2f& other)
{
	return(Vector2f(x+other.x,y+other.y));
}

void Vector2f::operator -=(const Vector2f& other)
{
	x-=other.x;
	y-=other.y;
}

Vector2f Vector2f::operator -(const Vector2f& other)
{
	return(Vector2f(x-other.x,y-other.y));
}

bool Vector2f::operator ==(const Vector2f& other)
{
	if(x == other.x && y == other.y)
		return true;
	else
		false;
}


void Vector2f::setX(float num)
{
	x = num;
}
void Vector2f::setY(float num)
{
	y = num;
}
//void Vector::setZ(float num)
//{
//	z = num;
//}
float Vector2f::getX()
{
	return x;
}
float Vector2f::getY()
{
	return y;
}
//float Vector::getZ()
//{
//	return z;
//}
