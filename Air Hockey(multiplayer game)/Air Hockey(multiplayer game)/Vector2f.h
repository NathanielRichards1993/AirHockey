#ifndef Vector2f_H
#define Vector2f_H

class Vector2f

{

public:

	//construct
	Vector2f();
	Vector2f(float x, float y);
	Vector2f(Vector2f*);

	void setX(float);
	void setY(float);
	float getX();
	float getY();

	void operator =(const Vector2f&);
	void operator +=(const Vector2f&);
	Vector2f operator +(const Vector2f&);
	void operator -=(const Vector2f&);
	Vector2f operator -(const Vector2f&);
	bool operator ==(const Vector2f&);

private:
	float x;
	float y;

};	


#endif