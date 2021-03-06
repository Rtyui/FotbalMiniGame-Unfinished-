#pragma once
class Vector
{
public:
	float x;
	float y;

	Vector(float _x, float _y);
	Vector();
	~Vector();

	void changeCoords(float _x, float _y);
	void showVector();
	void rotate(float tetha);
	void nullify();
	void reset();
	void operator=(Vector v);
	Vector operator+(Vector v);
	Vector operator-(Vector v);
	Vector operator/(int a);
};

